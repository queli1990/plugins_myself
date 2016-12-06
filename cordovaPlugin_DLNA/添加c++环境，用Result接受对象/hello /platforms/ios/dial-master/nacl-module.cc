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

#include <string>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/var_array.h>
#include <ppapi/cpp/var_dictionary.h>
#include <ppapi/utility/completion_callback_factory.h>
#include <ppapi/utility/threading/simple_thread.h>
#include <nacl_io/nacl_io.h>
#include "discovery.h"
#include "log.h"
#include "http.h"

using namespace pp;
using namespace std;

#ifdef WIN32
#undef PostMessage
// Allow 'this' in initializer list
#pragma warning(disable : 4355)
#endif

static DialDiscovery* g_discovery = NULL;
Instance* g_instance = NULL;

class DialInstance : public Instance {
public:
 	
  explicit DialInstance(PP_Instance instance)
    : Instance(instance)
    , m_thread(this)
    , m_cb_factory(this) 
  { 
    if ( ! g_instance ) { g_instance = this; }
  }

  virtual ~DialInstance() { 
    m_thread.Join(); 
    if ( g_instance == this ) {
      g_instance = NULL;
    } 
  }

private:
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]) {
    nacl_io_init_ppapi(pp_instance(), Module::Get()->get_browser_interface());
    m_thread.Start();
    if ( ! g_discovery ) {
      g_discovery = DialDiscovery::create();
    }
		return Instance::Init(argc, argn, argv);
  }

  virtual void HandleMessage(const Var& msg) {
    if ( ! msg.is_array() )
      return;

    VarArray args(msg);
    string cmd = args.Get(0).AsString();

    if (cmd == "scan") {  // scan LAN host
      m_thread.message_loop().PostWork( m_cb_factory.NewCallback(&DialInstance::scan) );
    } else if (cmd == "host_list") {  // get host list
      m_thread.message_loop().PostWork( m_cb_factory.NewCallback(&DialInstance::host_list) );
    } else {
      string host = args.Get(1).AsString();
      string app = args.Get(2).AsString();

      if (cmd == "launch") {   // launch host app
        string payload = args.Get(3).AsString();
        m_thread.message_loop().PostWork( 
          m_cb_factory.NewCallback(&DialInstance::launch, host, app, payload)
        );
      } else if (cmd == "hide") {     // hide host app
        m_thread.message_loop().PostWork( 
          m_cb_factory.NewCallback(&DialInstance::hide, host, app)
        );
      } else if (cmd == "stop") {     // stop host app
        m_thread.message_loop().PostWork( 
          m_cb_factory.NewCallback(&DialInstance::stop, host, app)
        );
      } else if (cmd == "status") {   // get  host app status
        m_thread.message_loop().PostWork( 
          m_cb_factory.NewCallback(&DialInstance::status, host, app)
        );
      }
    }
  }

  void ret(const string& tag, const Var& data) {
    VarDictionary rv;
    rv.Set("tag", tag);
    rv.Set("data", data);    
    PostMessage(rv);
  }
  
  void return_hosts(const string& tag) {
    vector<DialServer*> servers = g_discovery->get_server_list();
    VarArray data;
    for (int i = 0; i < servers.size(); i++) {
      VarDictionary item;
      item.Set("host", servers[i]->get_ip_address());
      item.Set("location", servers[i]->get_location());
      item.Set("app_url", servers[i]->get_apps_url());
      item.Set("friendly_name", servers[i]->get_friendly_name());
      item.Set("uuid", servers[i]->get_uuid());
      item.Set("mac_address", servers[i]->get_mac_address());
      data.Set(i, item);
    }
    ret(tag, data);
  }

  void scan(int32_t /* result */) {
    g_discovery->scan_host();
    return_hosts("scan");
  }

  void host_list(int32_t /* result */) {
    return_hosts("host_list");
  }

  void launch(int32_t /* result */, const string& host, const string& app, const string& payload) {
    DialServer* server = g_discovery->get_server(host);
    if ( server ) {
      string headers, body;

      if ( server->launch_application(app, payload, headers, body) == 0) {
        ret("launch", body);
      } else {
        ret("launch", "ERR:");
      }
    } else {
      ret("launch", "ERR:Host not found");
    }
  }

  void hide(int32_t /* result */, const string& host, const string& app) {
    DialServer* server = g_discovery->get_server(host);
    if ( server ) {
      string headers, body;

      if ( server->hide_application(app, headers, body) == 0) {
        ret("hide", body);
      } else {
        ret("hide", "ERR:");
      }
    } else {
      ret("hide", "ERR:Host not found");
    }
  }

  void stop(int32_t /* result */, const string& host, const string& app) {
    DialServer* server = g_discovery->get_server(host);
    if ( server ) {
      string headers, body;

      if ( server->stop_application(app, headers, body) == 0) {
        ret("stop", body);
      } else {
        ret("stop", "ERR:");
      }
    } else {
      ret("stop", "ERR:Host not found");
    }
  }

  void status(int32_t /* result */, const string& host, const string& app) {
    DialServer* server = g_discovery->get_server(host);
    if ( server ) {
      string headers, body;

      if ( server->get_status(app, headers, body) == 0) {
        ret("status", body);
      } else {
        ret("status", "ERR:");
      }
    } else {
      ret("status", "ERR:Host not found");
    }
  }

private:
  SimpleThread m_thread;
  CompletionCallbackFactory<DialInstance> m_cb_factory;
};

class DialModule : public Module {
 public:
  virtual Instance* CreateInstance(PP_Instance instance) {
    return new DialInstance(instance);
  }
};

namespace pp {
	Module* CreateModule() { return new DialModule(); }
}  // namespace pp
