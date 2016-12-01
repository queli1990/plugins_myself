/* ***** BEGIN LICENSE BLOCK *****
 * Distributed under the BSD license:
 *
 * Copyright (c) 2015-2016, Gochinatv
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Gochinatv nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Gochinatv BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ***** END LICENSE BLOCK ***** */

#include "http.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include "log.h"

#ifdef __native_client__
# include <pnacl/netdb.h>
# include <pnacl/netinet/in.h>
# include <pnacl/arpa/inet.h>
# include <pnacl/sys/ioctl.h>
# include <pnacl/sys/socket.h>
# include <pnacl/sys/select.h>
#else
# ifdef _WINDOWS
#  include <winsock2.h>
# else
#  include <netdb.h>
# endif
# include <net/if.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <string>
# include <algorithm>
#endif

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

dlna_ns

URI::URI(const string& uri): _protocol(Unknown), _href(uri), _port(0) {
  
  string s = _href;
  
  ::transform(s.begin(), s.end(), s.begin(), ::tolower);
  
  int start = 0;
  
  if ( s.find("http://") == 0 ) {
    start = 7;
    _protocol = HTTP;
  } else if ( s.find("https://") == 0 ) {
    start = 8;
    _protocol = HTTPS;
  } else {
    return;
  }
  
  long index = s.find("/", start);
  
  if (index == -1) {
    _pathname = '/';
    _hostname = _host = uri.substr(start);
  }
  else{
    _pathname = uri.substr(index);
    _hostname = _host = uri.substr(start, index - start);
  }
  
  index = _hostname.find_last_of(":");
  if ( index != -1 ) {
    _port = atoi(_hostname.substr(index + 1).c_str());
    _hostname = _hostname.substr(0, index);
  }
  
  _domain = _hostname;
  
  index = _domain.find_last_of(".");
  
  if (index != -1) {
    index = _domain.find_last_of(".", index);
    if (index != -1) {
      _domain = _domain.substr(index + 1);
    }
  }
}

static string hex_chars("0123456789abcdef");

typedef map<string, string>::iterator map_iterator;

class HttpRequest::_Inl: public HttpRequest {
public:
#define _inl(self) static_cast<HttpRequest::_Inl*>(self)
  
  size_t parse_hex(string s) {
    
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    size_t rv = 0;
    
    for ( int i = 0; i < s.length(); i++ ) {
      rv <<= 4;
      long j = hex_chars.find( s.substr(i, 1) );
      if ( j == -1 ) {
        return 0; // hex格式不正确
      } else {
        rv |= j;
      }
    }
    return rv;
  }
  
  bool parse_chunked(const char* buff, size_t size) {
    
    if ( m_chunked_size ) { //
      size_t c = MIN(m_chunked_size, size);
      m_resp.body.append(buff, c);
      m_chunked_size -= c; // consume
      size -= c; // consume
      buff += c; // consume
    }
    
    if ( size ) {
      m_chunked.append(buff, size);
      long index = m_chunked.find("\r\n");
      if (index != -1) {
        if ( index == 0 ) {
          return true; // end
        }
        // parse chunked length
        m_chunked_size = parse_hex(string(m_chunked.c_str(), index));
        
        if ( m_chunked_size == 0 ) {
          if ( m_chunked.find("\r\n", index + 2) == 0 ) {
            return true; // end
          }
        }
        string s( std::move(m_chunked) );
        return parse_chunked(s.c_str() + index + 2, s.length() - index - 2);
      }
    }
    return false;
  }
  
  void parse_header(string s) {
    long i = s.find(":");
    if ( i != -1 ) {
      string key = s.substr(0, i);
      transform(key.begin(), key.end(), key.begin(), ::tolower);
      m_resp.headers[key] = s.substr( i + (s[i + 1] == ' ' ? 2 : 1) );
    }
  }
  
  void parse_headers() {
    
    string& s = m_resp.header;
    
    long prev = 0, i;
    
    while ( (i = s.find("\r\n", prev)) != -1 ) {
      parse_header( s.substr(prev, i - prev) );
      prev = i + 2;
    }
    parse_header(s.substr(prev));
  }
  
#define error() perror("Err"); return 0

  int init_connect(sockaddr_in& saddr) {
    int sock;
    
    if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
      error();
    }

    // int timeout = m_timeout * 1000;
    // send timeout
    // if ( ::setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1 ){
    //   error();
    // }

    // recvfrom timeout
    // if ( ::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1 ) {
    //   error();
    // }

    struct hostent* host = gethostbyname( m_uri->hostname().c_str() );
    if ( ! host ) {
      error();
    }
    
    saddr.sin_addr = *((struct in_addr *)host->h_addr_list[0] );
    // saddr.sin_addr.s_addr = inet_addr( m_uri->hostname().c_str() );
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(m_uri->port() ? m_uri->port(): 80);
    
    unsigned long mode = 1;
    if ( ioctl(sock, FIONBIO, &mode) == -1 ) {
      error();
    }
    
    // create tcp connect
    if ( connect(sock, (struct sockaddr*)&saddr, sizeof(saddr)) == -1 ) {
      
      if (errno == EINPROGRESS) {
        int error;
        int len = sizeof(int);
        
        fd_set readfds;
        // wait until either socket has data ready to be recv()d (default timeout 5 secs)
        struct timeval tv;
        tv.tv_sec = m_timeout;
        tv.tv_usec = 0;
        
        FD_ZERO(&readfds);      // clear the set ahead of time
        FD_SET(sock, &readfds); // add our descriptors to the set
        
        int rt = ::select(sock + 1, NULL, &readfds, NULL, &tv);
        if ( rt > 0 ) {
          ::getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
          if (error != 0) {
            error();
          }
        } else {
          error();
        }
      } else {
        error();
      }
    }

    mode = 0;

    if ( ioctl(sock, FIONBIO, &mode) == -1 ) {
      error();
    }
    return sock;
  }
  
#undef error
#define error() perror("Err"); ::close(sock); return
  
	void send() {
		assert( m_uri );
		m_resp.rc = -1;
    
    sockaddr_in saddr;
    socklen_t addrlen = sizeof(saddr);
    int sock = init_connect(saddr);
    
    if ( ! sock ) {
      return;
    }
    
    // send data
    
    char headers[2048] = { 0 };
    
    sprintf(headers,
            "%s %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Accept: */*\r\n"
            "Content-Length: %d\r\n"
            "User-Agent: Mozilla/5.0 AppleWebKit Dial \r\n",
            m_method.c_str(), m_uri->pathname().c_str(),
            m_uri->host().c_str(),
            m_method == "POST" ? (int)m_data.length() : 0);
    
    if ( ::send(sock, headers, strlen(headers), 0) == -1 ) { // send headers
      error();
	  }
    if ( m_headers.size() ) { // send custom headers
      string custom_headers;
      map_iterator i = m_headers.begin();
      map_iterator e = m_headers.end();
      
      for ( ; i != e; i++ ) {
        custom_headers.append(i->first);
        custom_headers.append(": ");
        custom_headers.append(i->second);
        custom_headers.append("\r\n");
      }
      
      if ( ::send(sock, custom_headers.c_str(), custom_headers.length(), 0) == -1 ) {
        error();
      }
    }
    if ( ::send(sock, "\r\n", 2, 0) == -1 ) { // headers end
      error();
    }
    
    if ( m_method == "POST" && ! m_data.empty() ) { // send data
      const char* data = m_data.c_str();
      size_t len = m_data.length();
      
      if ( -1 == ::send(sock, data, len, 0) ) { // 一次发送全部数据
        error();
      }
    }
    
    // recvfrom data
    
    char    buff[4096];
    int     content_length = 0;
    bool    chunked = false;
    string  tmp;
    
    m_chunked_size = 0;
    m_chunked.clear();
    
    fd_set readfds;
    // wait until either socket has data ready to be recv()d (default timeout 5 secs)
    struct timeval tv;
    tv.tv_sec = m_timeout;
    tv.tv_usec = 0;

    while (1) {

      FD_ZERO(&readfds);      // clear the set ahead of time
      FD_SET(sock, &readfds); // add our descriptors to the set
      int rt = ::select(sock + 1, &readfds, NULL, NULL, &tv);

      if ( rt == -1 ) {
        error();
      }  else if ( rt == 0 ) {
        ATRACE("Timeout occurred!  No data after %ld seconds.\n", m_timeout);
        ::close(sock); return;
      }
      
      ssize_t size = ::recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr *) &saddr, &addrlen);

      if (size == -1) {
        error();
      } else if (size == 0) {
        break;
      }

      if (m_resp.headers.empty()) { // parse header
        tmp.append(buff, buff + size);

        long index = tmp.find("\r\n\r\n");
        if (index != -1) {
          m_resp.header = tmp.substr(0, index);
          parse_headers();

          map_iterator i = m_resp.headers.find("content-length");

          if (i == m_resp.headers.end()) {
            if (m_resp.headers["transfer-encoding"] == "chunked") {
              chunked = true; // 数据分块
              if (parse_chunked(tmp.c_str() + index + 4, tmp.length() - index - 4))
                break;
            } else { // 无法得知数据长度,所以抛弃数据
              break;
            }
          } else {
            content_length = atoi(i->second.c_str());
            m_resp.body = tmp.substr(index + 4);
            if (m_resp.body.length() >= content_length) break; // end
          }
        }
      } else { // parse body
        if (chunked) {
          if (parse_chunked(buff, size)) break;
        } else {
          m_resp.body.append(buff, buff + size);
          if (m_resp.body.length() >= content_length) break; // end
        }
      }
    }

    ::close(sock); // 关闭SOCK连接
    m_resp.rc = 0;
	}
};

HttpRequest::HttpRequest()
        : m_method("GET")
        , m_uri(NULL)
        , m_chunked_size(0)
        , m_timeout(5) {

}
HttpRequest::~HttpRequest() { delete m_uri; }

void HttpRequest::set_url(const string& value) {
  if ( m_uri ) {
    delete m_uri;
  }
	m_uri = new URI(value);
}

int HttpRequest::send() {
	_inl(this)->send();
	return m_resp.rc;
}

HttpReqponse Http::get(const string& url) {
	HttpRequest req;
	req.set_method("GET");
	req.set_url(url);
	req.send();
	return std::move( req.response() );
}

HttpReqponse Http::post(const string& url, const string& data) {
	HttpRequest req;
	req.set_method("POST");
	req.set_url(url);
	req.set_data(data);
	req.send();
	return std::move( req.response() );
}

dlna_ns_end
