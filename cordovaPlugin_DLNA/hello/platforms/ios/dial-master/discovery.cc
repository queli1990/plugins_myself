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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <ctime>
#include <map>
#include <mutex>
#include <thread>

#ifdef __native_client__
# include <pnacl/netinet/in.h>
# include <pnacl/arpa/inet.h>
# include <pnacl/sys/ioctl.h>
# include <pnacl/sys/socket.h>
# include <pnacl/sys/select.h>
#else
# include <net/if.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <sys/select.h>
#endif

#include "discovery.h"
#include "http.h"
#include "dial.h"
#include "media-renderer.h"
#include <algorithm>

#define SSDP_PORT                   (1900)
#define SSDP_MULTICAST_ADDR         ("239.255.255.250")

dlna_ns

typedef std::lock_guard<std::mutex> ScopeLocker;

class Discovery::_Inl: public Discovery {
public:
#define _inl_dis(self) static_cast<Discovery::_Inl*>(self)
  
  void parse_buffer(char* buff, ssize_t size) {
    string str = string(buff, size);
    vector<string> ls = Util::split(string(buff, size), "\r\n");
    
    if (ls.size() == 0) {
      return;
    }
    if ( ls[0].find("HTTP/1.1") != 0 ) {
      return;
    }
    
    map<string, string> headers;
    
    for ( int i = 1; i < ls.size(); i++ ) {
      long index = ls[i].find(":");
      
      if ( index != -1 ) {
        string key = ls[i].substr(0, index);
        ::transform(key.begin(), key.end(), key.begin(), ::tolower);
        string value;
        if ( ls[i][index + 1] == ' ' ) {
          value = ls[i].substr(index + 2);
        } else {
          value = ls[i].substr(index + 1);
        }
        headers[key] = value;
      }
    }
    
    string value = headers["st"];
    string location = headers["location"];
    string hostname = Util::get_hostname( location );
    
    if ( value.empty() ) {
      return;
    }
    if ( hostname.empty() ) {
      return;
    }

    // ATRACE("\n------Received------ \n%s", buff);

    bool change = false;

    map<ServiceType, Service *>* services = NULL;

    {
      ScopeLocker lock(m_mutex);
      services = &m_all_service[hostname];
    }

    if (value == "urn:dial-multiscreen-org:service:dial:1") {
      auto it = services->find(DIAL);
      if (it == services->end()) {
        ATRACE("\n------Received------ \n%s", buff);
        Service* service = new Dial(location);
        {
          ScopeLocker lock(m_mutex);
          (*services)[DIAL] = service;
        }
        change = true;
      } else {
        it->second->update(location);
      }
    } else if (value == "urn:schemas-upnp-org:device:MediaRenderer:1") {
      auto it = services->find(MEDIA_RENDERER);
      if (it == services->end()) {
        ATRACE("\n------Received------ \n%s", buff);
        Service* service = new MediaRenderer( location );
        {
          ScopeLocker lock(m_mutex);
          (*services)[MEDIA_RENDERER] = service;
        }
        change = true;
      } else {
        it->second->update(location);
      }
    }
    if ( change ) {
      m_delegate->discovery_change(this);
    }
  }
  
  void send_mcast(int sock) {
    
    static const char pkg_format[] =
    "M-SEARCH * HTTP/1.1\r\n"
    "HOST: %s:%d\r\n"
    "MAN: \"ssdp:discover\"\r\n"
    "MX: 5\r\n"
    "ST: %s\r\n"
    "\r\n";
    
    string st_s;
    
    /*
     upnp:rootdevice
     urn:schemas-upnp-org:device:MediaServer:1
     urn:schemas-upnp-org:service:AVTransport:1
     urn:schemas-upnp-org:service:ConnectionManager:1
     urn:schemas-upnp-org:service:RenderingControl:1
     urn:schemas-upnp-org:device:InternetGatewayDevice:1
     */
    switch (m_ssdp_st) {
      case ALL:             st_s = "ssdp:all"; break;
      case MEDIA_RENDERER:  st_s = "urn:schemas-upnp-org:device:MediaRenderer:1"; break;
      case DIAL:            st_s = "urn:dial-multiscreen-org:service:dial:1"; break;
      default: return;
    }
    
    char buf[1024];
    sprintf(buf, pkg_format, SSDP_MULTICAST_ADDR, SSDP_PORT, st_s.c_str());
    
    ATRACE("------Sending mcast for discovery------");
    ATRACE("%s", buf);
    
    size_t send_size = strlen(buf);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SSDP_PORT);
    addr.sin_addr.s_addr = inet_addr(SSDP_MULTICAST_ADDR);
    
    // Send udp data pkg
    if ( sendto(sock, buf, send_size, 0, (struct sockaddr*)&addr, sizeof(addr)) == -1 ) {
      ATRACE("sendto: %s", strerror(errno));
    }
  }
  
  int socket_init() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
      ATRACE("socket create error: %s", strerror(errno));
      return 0;
    }
    
    int opt = 1;
    /* Must be set before bind*/
    int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (ret < 0) ATRACE("setsockopt(SO_REUSEADDR) fail: %s", strerror(errno));
    
#if defined(__FreeBSD__) || defined(__OSX__) || defined(__APPLE__)
    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if (ret < 0) ATRACE("setsockopt(SO_REUSEPORT) fail: %s", strerror(errno));
#endif
    
    return sock;
  }
  
  void loop() {
    ScopeLocker lock(m_mutex_loop);
    
    int sock = socket_init();
    
    if ( sock < 0 ) {
      ScopeLocker lock(m_mutex);
      m_runing = false;
      ATRACE("socket: %s", strerror(errno));
      return;
    }
    
    fd_set readfds;
    struct timeval tv;
    
    // wait until either socket has data ready to be recv()d (timeout 5.5 secs)
    tv.tv_sec = 5;
    tv.tv_usec = 500000; // 0.5 secs
    char buff[4096];
    
    send_mcast(sock); // send mcast
    
    time_t prev = std::time(NULL);
    
    while ( 1 ) {
      {
        ScopeLocker lock(m_mutex);
        if ( ! m_runing ) {
          break;
        }
      }

      if ( std::time(NULL) - prev >= m_send_interval ) {
        send_mcast(sock); // send mcast
        prev = std::time(NULL);
      }
      
      FD_ZERO(&readfds);      // clear the set ahead of time
      FD_SET(sock, &readfds); // add our descriptors to the set
      
      int rt = ::select(sock + 1, &readfds, NULL, NULL, &tv);
      
      if ( rt == -1 ) {
        ATRACE("select, %s", strerror(errno)); // error occurred in select()
        break;
      }
      else if ( rt == 0 ) {
        // printf("Timeout occurred!  No data after 5.5 seconds.\n");
      }
      else {
        ssize_t size = recvfrom(sock, buff, 4096 - 1, 0, NULL, NULL);
        if ( size == -1 ) {
          perror("recvfrom");
          break;
        }
        buff[size] = 0;
        parse_buffer(buff, size);
      }
    }
    
    ::close(sock);
    
    if ( sock < 0 ) {
      ScopeLocker lock(m_mutex);
      m_runing = false;
    }
  }
};

static Discovery::Delegate default_delegate;

Discovery::Discovery()
: m_runing(false)
, m_ssdp_st(ALL)
, m_send_interval(20)
, m_delegate(&default_delegate)
{
 
}

Discovery::~Discovery() {
  stop();
  ScopeLocker lock(m_mutex_loop);
  ScopeLocker lock2(m_mutex);
  for( auto i = m_all_service.begin(); i != m_all_service.end(); i++ ) {
    for ( auto it = i->second.begin(), e = i->second.end(); it != e; it++) {
      delete it->second;
    }
  }
  m_all_service.clear();
}

vector<Service*> Discovery::all_service() {
  ScopeLocker lock(m_mutex);
  vector<Service*> rv;
  for( auto i = m_all_service.begin(); i != m_all_service.end(); i++ ) {
    for ( auto it = i->second.begin(), e = i->second.end(); it != e; it++) {
      rv.push_back( it->second );
    }
  }
  return rv;
}

Service* Discovery::service(const string& host, ServiceType type) {
  ScopeLocker lock(m_mutex);
  auto i = m_all_service.find(host);
  if ( i != m_all_service.end() ) {
    auto it = i->second.find(type);
    if ( it != i->second.end() ) {
      return it->second;
    }
  }
  return NULL;
}

vector<Service*> Discovery::service(ServiceType type) {
  ScopeLocker lock(m_mutex);
  vector<Service*> rv;
  for( auto i = m_all_service.begin(); i != m_all_service.end(); ++i ) {
    auto it = i->second.find( type );
    if ( it != i->second.end() ) {
      rv.push_back( it->second );
    }
  }
  return rv;
}

void Discovery::set_delegate(Delegate* delegate) {
  if ( delegate ) {
    ScopeLocker lock(m_mutex);
    m_delegate = delegate;
  }
}

void Discovery::scan(ServiceType st) {
  // Http::get("http://192.168.116.1:49152/TxMediaRenderer_desc.xml");
  {
    ScopeLocker lock(m_mutex);
    m_ssdp_st = st;
    if (m_runing) {
      return;
    }
    m_runing = true;
  }
  std::thread worker(&Discovery::_Inl::loop, _inl_dis(this));
  worker.detach();
}

void Discovery::stop() {
  ScopeLocker lock(m_mutex);
  if ( m_runing ) {
    m_runing = false;
  }
}

void Discovery::set_send_mcast_interval(int second) {
  ScopeLocker lock(m_mutex);
  m_send_interval = D_MAX( second, 5 );
}

dlna_ns_end
