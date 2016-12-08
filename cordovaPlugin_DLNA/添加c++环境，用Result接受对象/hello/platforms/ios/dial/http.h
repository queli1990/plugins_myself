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


#ifndef __dial_http__
#define __dial_http__

#include <string>
#include <map>
#include "util.h"

using namespace std;

dlna_ns


/**
 * @class URI
 */
class URI {
public:
  enum Protocol {
    HTTP,
    HTTPS,
    Unknown,
  };
  
  URI(): _protocol(Unknown), _port(0) { }
  URI(const string& uri);
  
  // attributes
  inline Protocol protocol() const { return _protocol; }
  inline const string& href() const { return _href; }
  inline const string& host() const { return _host; }
  inline int     port() const { return _port; }
  inline const string& hostname() const { return _hostname; }
  inline const string& domain() const { return _domain; }
  inline const string& pathname() const { return _pathname; }
  
private:
  Protocol _protocol;
  string _href;
  string _host;
  int    _port;
  string _hostname;
  string _domain;
  string _pathname;
};

struct HttpReqponse {
  int 		rc;
  string  header;
  map<string, string> headers;
  string 	body;
};

class HttpRequest {
public:
  HttpRequest();
  ~HttpRequest();
  inline void set_method(const string& value) { m_method = value; }
  void set_url(const string& value);
  inline void set_headers(const string& key, const string& value) {
    m_headers[key] = value;
  }
  inline void set_data(const string& value) { m_data = value; }
  inline void set_timtout(int timeout) { m_timeout = timeout; }
  inline HttpReqponse& response() { return m_resp; }
  int send();
private:
  class _Inl;  friend class _Inl;
  string m_method;  // GET POST DELETE ...
  URI*   m_uri;
  map<string, string> m_headers;
  string m_data;
  size_t m_chunked_size;
  string m_chunked;
  int m_timeout;
  HttpReqponse m_resp;
};

class Http {
public:
  static HttpReqponse get(const string& url);
  static HttpReqponse post(const string& url, const string& data = "");
};

dlna_ns_end
#endif
