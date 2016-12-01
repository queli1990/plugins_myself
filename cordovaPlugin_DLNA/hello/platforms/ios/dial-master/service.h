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

#ifndef dlna_service_h
#define dlna_service_h

#include <map>
#include <string>
#include "log.h"
#include "util.h"
#include "http.h"
#include <tinyxml2.h>

using namespace tinyxml2;

dlna_ns

class Dial;
class MediaRenderer;

/**
 * @class Service
 */
class Service {
public:

  struct Result {
    int    code;
    string error;
    JSONValue data;
    string to_json_string();
  };

  /**
   * @constructor
   */
  Service(const string& location);
  
  virtual Dial* as_dial() { return NULL; }
  virtual MediaRenderer* as_media_renderer() { return NULL; }
  
  /**
   * @func update
   */
  void update(const string& location);
  
  /**
   * Get the DIAL Server location
   *
   * @return Location of the server (http://<IP_ADDR>:<PORT>/dd.xml)
   */
  string get_location() { return m_init_uri.href(); }
  
  /**
   * Get the DIAL Server IP address
   *
   * @return IP address of the server (X.X.X.X)
   */
  string get_hostname() { return m_init_uri.hostname(); }
  
  /**
   * Get the DIAL Server port number
   *
   * @return Port number
   */
  int get_port() { return m_init_uri.port(); }
  
  /**
   * Get the DIAL friendly name
   *
   * @return true if successful, false otherwise
   */
  string get_friendly_name() { return m_friendly_name; }
  
  /**
   * Get the DIAL UUID
   *
   * @return true if successful, false otherwise
   */
  string get_uuid() { return m_uuid; }
  
  /**
   * @func is_invalid
   */
  inline bool is_invalid() { return m_is_invalid; }
  
protected:
  map<string, string> m_init_headers;
  XMLDocument m_init_xml;
  URI    m_init_uri;
  string m_friendly_name;
  string m_uuid;
  bool m_is_invalid;
};

dlna_ns_end
#endif
