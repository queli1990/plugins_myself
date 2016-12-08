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

#ifndef dlna_discovery
#define dlna_discovery

#include <map>
#include <thread>
#include <mutex>
#include "service.h"

using namespace std;

dlna_ns

/**
 * @class Discovery
 */
class Discovery {
public:
  
  enum ServiceType {
    ALL,            // ssdp:all
    MEDIA_RENDERER, // urn:schemas-upnp-org:device:MediaRenderer:1
    DIAL,           // urn:dial-multiscreen-org:service:dial:1
  };
  
  class Delegate {
  public:
    virtual void discovery_change(Discovery* dis) { }
    virtual void discovery_error(Discovery* dis, const string& err) { }
  };
  
  /**
   * @constructor
   */
  Discovery();
  
  /**
   * @destructor
   */
  virtual ~Discovery();
  
  /**
   * @func set_delegate
   */
  void set_delegate(Delegate* delegate);
  
  /**
   * Get the list of servers that have been discovered.
   *
   * are no hosts.
   */
  vector<Service*> all_service();
  
  /**
   * get service by host and type
   */
  Service* service(const string& host, ServiceType type);
  
  /**
   * get service by type
   */
  vector<Service*> service(ServiceType type);
  
  /**
   * @func start # Start scan device
   */
  void scan(ServiceType type = ALL);
  
  /**
   * @func stop # Stop scan
   */
  void stop();

  /**
   * @func clear
   * */
  void clear();
  
  /**
   * @func set_send_mcast_interval # Setting send mcast message interval default 20 second
   */
  void set_send_mcast_interval(int second);
  
private:
  typedef map<string, map<ServiceType, Service*>> ServiceMap;
  ServiceMap      m_all_service;
  bool            m_runing;
  int             m_run_id;
  std::mutex      m_mutex;
  std::mutex      m_mutex_loop;
  ServiceType     m_ssdp_st;
  int             m_send_interval;
  Delegate*       m_delegate;
  class _Inl; friend class _Inl;
};

dlna_ns_end
#endif
