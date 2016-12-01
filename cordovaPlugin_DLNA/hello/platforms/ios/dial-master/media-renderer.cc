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

#include "media-renderer.h"
#include "http.h"
#include <tinyxml2.h>

dlna_ns

typedef MediaRenderer::Result Result;

static rapidjson::CrtAllocator share_allocator;

MediaRenderer::MediaRenderer( const string& location ): Service( location ) {
  vector<XMLElementWrap> ls = XMLNodeWrap(&m_init_xml).find_elements_by_tag("service");
  
  for ( int i = 0; i < ls.size(); i++ ) {
    XMLElementWrap serviceType = ls[i].find_element_by_tag("serviceType");
    if ( ! serviceType.is_empty() ) {
      string value = serviceType->GetText();
      XMLElementWrap serviceId = ls[i].find_element_by_tag("serviceId");
      XMLElementWrap controlURL = ls[i].find_element_by_tag("controlURL");
      
      if ( !serviceId.is_empty() && !controlURL.is_empty() ) {
        string url = controlURL->GetText();
        if (url[0] != '/') {
          url = "/" + url;
        }
        if ( value.find("urn:schemas-upnp-org:service:AVTransport:") == 0 ) {
          m_av_transport_mask = value;
          m_av_transport_service_id = serviceId->GetText();
          m_av_transport_control_url = url;
        } else if ( value.find("urn:schemas-upnp-org:service:RenderingControl:") == 0 ) {
          m_rendering_control_mask = value;
          m_rendering_control_service_id = serviceId->GetText();
          m_rendering_control_control_url = url;
        }
      }
    }
  }
}

MediaRenderer::~MediaRenderer() { }

// 动作调用（UPnP Action Request）
/**
 *  POST <control URL> HTTP/1.0
 *  Host: hostname:portNumber
 *  Content-Lenght: byte in body
 *  Content-Type: text/xml; charset="utf-8"
 *  SOAPACTION: "urn:schemas-upnp-org:service:serviceType:v#actionName"
 *  SOAPACTION: "urn:upnp-org:serviceId:serviceType:v#actionName"
 *
 *  <!--必有字段-->
 *  <?xml version="1.0" encoding="utf-8"?>
 *  <!--SOAP必有字段-->
 *  <s:Envelope s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"
 *              xmlns:s="http://schemas.xmlsoap.org/soap/envelope/">
 *    <s:Body>
 *      <!--Body内部分根据不同动作不同-->
 *      <!--动作名称-->
 *      <u:actionName xmlns:u="urn:schemas-upnp-org:service:serviceType:v">
 *        <!--输入参数名称和值-->
 *        <argumentName>in arg values</argumentName>
 *        <!--若有多个参数则需要提供-->
 *      </u:actionName>
 *    </s:Body>
 *  </s:Envelope>
 */

// 动作响应（UPnP Action Response-Succes）
/**
 *  HTTP/1.0 200 OK                             // 响应成功响应头
 *  Content-Type: text/xml; charset="utf-8"
 *  Date: Tue, 01 Mar 2016 10:00:36 GMT+00:00
 *  Content-Length: byte in body
 *
 *  <?xml version="1.0" encoding="utf-8" standalone="no"?>
 *  <s:Envelope  xmlns:s="http://schemas.xmlsoap.org/soap/envelope/"
 *               s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
 *    <s:Body>
 *      <!--之前部分为固定字段-->
 *      <!--之前部分为固定字段-->
 *      <u:actionNameResponse xmlns:u="urn:schemas-upnp-org:service:serviceType:v">
 *        <!--输出变量名称和值-->
 *        <arugumentName>out arg value</arugumentName>
 *        <!--若有多个输出变量则继续写，没有可以不存在输出变量-->
 *      </u:actionNameResponse>
 *    </s:Body>
 *  </s:Envelope>
 */

// 动作错误响应（UPnP Action Response-Succes）
/**
 *  HTTP/1.0 500 Internal Server Error          // 响应成功响应头
 *  Content-Type: text/xml; charset="utf-8"
 *  Date: Tue, 01 Mar 2016 10:00:36 GMT+00:00
 *  Content-Length: byte in body
 
 *  <?xml version="1.0" encoding="utf-8" standalone="no"?>
 *  <s:Envelope  xmlns:s="http://schemas.xmlsoap.org/soap/envelope/"
 *               s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
 *    <s:Body>
 *      <u:Fault>
 *        <!--之前部分为固定字段-->
 *        <faultcode>s:Client</faultcode>
 *        <faultstring>UPnPError</faultstring>
 *        <detail>
 *          <UPnPError xmlns="urn:schemas-upnp-org:control-1-0">
 *            <errorCode>402</errorCode>
 *            <errorDescription>Invalid or Missing Args</errorDescription>
 *          </UPnPError>
 *        </detail>
 *      </u:Fault>
 *    </s:Body>
 *  </s:Envelope>
 */

/************************urn:schemas-upnp-org:service:AVTransport:1************************/

#define verification_is_support(name) \
if ( m_av_transport_control_url.empty() ) return { -101, string("Not support the API ") + name }

enum {
  AVTransport,
  RenderingControl,
};

class MediaRenderer::_Inl: public MediaRenderer {
public:
#define _inl_mr(self) static_cast<MediaRenderer::_Inl*>(self)
  
  static inline bool is_in_range(int value, int lower_limit, int higher_limit) {
    assert(lower_limit <= higher_limit);
    return (uint)(value - lower_limit) <= (uint)(higher_limit - lower_limit);
  }
  
  static inline bool is_decimal_digit(int c) {
    return is_in_range(c, '0', '9');
  }
  
  static inline int char_to_number(int c) {
    return c - '0';
  }
  
  static JSONValue parse_to_json(const string& str) {
    bool is_num = true;
    bool is_float = false;
    vector<int> st_cut;
    
    for ( int i = 0 ; i < str.length(); i++ ) {
      char c = str[i];
      if ( c == '.' ) {
        if ( is_float || st_cut.size() ) {
          is_num = false; break;
        } else {
          is_float = true;
        }
      } else if (c == ':') { // st cut
        if ( is_float ) {
          is_num = false; break;
        }
        if ( st_cut.size() == 0 || i - st_cut[st_cut.size() - 1] > 1 ) {
          st_cut.push_back(i);
        } else {
          is_num = false; break;
        }
      } else {
        if ( ! is_decimal_digit(str[i]) ) {
          is_num = false; break;
        }
      }
    }
    
    if ( is_num ) {
      if (st_cut.size() > 0 && st_cut.size() < 3) {
        int i = st_cut[st_cut.size() - 1];
        unsigned int num;
        sscanf( str.substr(i + 1).c_str(), "%u", &num ); // second
        
        int64_t st = num;
        
        if ( st_cut.size() == 1 ) { // 00:00
          sscanf( str.substr(0, i).c_str(), "%u", &num );
          st += (num * 60);
        } else { // 00:00:00
          sscanf( str.substr(st_cut[0] + 1, i - st_cut[0] - 1).c_str(), "%u", &num );
          st += (num * 60);
          sscanf( str.substr(0, st_cut[0]).c_str(), "%u", &num );
          st += (num * 3600);
        }
        return JSONValue( st );
      }
      double num;
      sscanf( str.c_str(), "%lf", &num );
      return JSONValue( num );
    }
    
    return JSONValue( str.c_str(), (unsigned)str.length(), share_allocator );
  }
  
  Result get_call_error(XMLElementWrap fault) {
    XMLElementWrap errorCode = fault.find_element_by_tag("errorCode");
    XMLElementWrap errorDescription = fault.find_element_by_tag("errorDescription");
    
    if ( errorCode.is_empty() || errorDescription.is_empty() ) {
      return { -106, "Return XML fault structure error" };
    }
    int code;
    sscanf( errorCode->GetText(), "%d", &code);
    
    return { code, errorDescription->GetText() };
  }
  
  Result get_call_result(XMLElementWrap response) {
    JSONValue data(rapidjson::kObjectType);
    vector<XMLElementWrap> ls = response.child_elements();
    for ( int i = 0; i < ls.size(); i++ ) {
      XMLElementWrap& el = ls[i];

      const char* text = el->GetText();
      if ( text ) {
        JSONValue::MemberIterator member = data.FindMember(el->Name());
        if (member != data.MemberEnd()) {
          member->value = parse_to_json( text );
        } else {
          data.AddMember(JSONValue(el->Name(), strlen(el->Name()) ),
                         parse_to_json( text ),
                         share_allocator);
        }
      }
    }
    return { 0, "", std::move(data) };
  }
  
  Result call(int type,
              const string& name,
              const map<string, string>& args = map<string, string>()) {
    verification_is_support(name);
    
    string service_mask, url;
    char host[50];
    sprintf(host, "%s://%s:%d",
            m_init_uri.protocol() == URI::HTTP ? "http" : "https",
            m_init_uri.hostname().c_str(), m_init_uri.port());
    if ( type == AVTransport ) {
      service_mask = m_av_transport_mask;
      url = host + m_av_transport_control_url;
    } else {
      service_mask = m_rendering_control_mask;
      url = host + m_rendering_control_control_url;
    }
    
    HttpRequest req;
    req.set_method("POST");
    req.set_url(url);
    req.set_headers("SOAPACTION", "\"" + service_mask + "#" + name + "\"");
    req.set_headers("Content-Type", "text/xml; charset=\"utf-8\"");
    
    const char* templ =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\""
    "            xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">"
    "  <s:Body>"
    "    <u:%s xmlns:u=\"%s\">"
    "       <InstanceID>0</InstanceID>";
    const char* templ2 =
    "    </u:%s>"
    "  </s:Body>"
    "</s:Envelope>";
    
    char start[1024]; sprintf(start, templ, name.c_str(), service_mask.c_str());
    char   end[1024]; sprintf(end, templ2, name.c_str());
    
    string param;
    for ( auto it = args.begin(), e = args.end(); it != e; it++ ) {
      param += "<" + it->first + ">" + it->second + "</" + it->first + ">";
    }
    req.set_data( start + param + end );
    
    if ( req.send() != 0 ) {
      return { -102, "Send http request error" };
    }
    
    XMLDocument xml;
    if ( xml.Parse(req.response().body.c_str()) != XML_NO_ERROR ) {
      return { -103, "Parse xml error" };
    }
    
    XMLElementWrap body = XMLNodeWrap(&xml).find_element_by_tag("s:Body");
    
    if ( body.is_empty() ) {
      return { -104, "Return XML structure error" };
    }
    
    XMLElementWrap el = body.find_element_by_tag(string("u:") + name + "Response");
    if ( el.is_empty() ) {
      el = body.find_element_by_tag("s:Fault");
      if ( el.is_empty() ) {
        return { -105, "Return XML structure error" };
      } else {
        return get_call_error( el );
      }
    }
    return get_call_result( el );
  }
};

Result MediaRenderer::set_av_transport_uri(const string& uri) {
  map<string, string> args;
  args["CurrentURI"] = uri;
  args["CurrentURIMetaData"] = "";
  Result res = _inl_mr(this)->call(AVTransport, "SetAVTransportURI", args);
  if ( res.code == 0 ) {
    return play();
  }
  return res;
}

Result MediaRenderer::get_media_info() {
  return _inl_mr(this)->call(AVTransport, "GetMediaInfo");
}

Result MediaRenderer::get_device_capabilities() {
  return _inl_mr(this)->call(AVTransport, "GetDeviceCapabilities");
}

Result MediaRenderer::get_transport_info() {
  return _inl_mr(this)->call(AVTransport, "GetTransportInfo");
}

Result MediaRenderer::get_position_info() {
  return _inl_mr(this)->call(AVTransport, "GetPositionInfo");
}

Result MediaRenderer::get_transport_settings() {
  return _inl_mr(this)->call(AVTransport, "GetTransportSettings");
}

Result MediaRenderer::stop() {
  return _inl_mr(this)->call(AVTransport, "Stop");
}

Result MediaRenderer::play() {
  map<string, string> args;
  args["Speed"] = "1";
  return _inl_mr(this)->call(AVTransport, "Play", args);
}

Result MediaRenderer::pause() {
  return _inl_mr(this)->call(AVTransport, "Pause");
}

static string format_time_number(long num) {
  if (num) {
    char s[10];
    string str_num = s;
    sprintf(s, num < 10 ? "0%ld" : "%ld", num);
    return s;
  }
  return "00";
}

static string format_time_span(long time_span) {
  long second = time_span;
  long minute = time_span / 60;
  long hour   = minute / 60;
  string rv = format_time_number(hour) + ":" +
              format_time_number(minute % 60) + ":" +
              format_time_number(second % 60);
  return rv;
}

Result MediaRenderer::seek(long time) {
  map<string, string> args;
  args["Unit"] = "REL_TIME";
  args["Target"] = format_time_span(time);
  return _inl_mr(this)->call(AVTransport, "Seek", args);
}

Result MediaRenderer::next() {
  return _inl_mr(this)->call(AVTransport, "Next");
}

Result MediaRenderer::previous() {
  return _inl_mr(this)->call(AVTransport, "Previous");
}

//##########

Result MediaRenderer::set_record_quality_mode(const string& record_quality_mode) {
  map<string, string> args;
  args["NewRecordQualityMode"] = record_quality_mode;
  return _inl_mr(this)->call(AVTransport, "SetRecordQualityMode", args);
}

Result MediaRenderer::record() {
  return _inl_mr(this)->call(AVTransport, "Record");
}

Result MediaRenderer::get_current_transport_actions() {
  return _inl_mr(this)->call(AVTransport, "GetCurrentTransportActions");
}

Result MediaRenderer::set_play_mode(const string& play_mode) {
  map<string, string> args;
  args["NewPlayMode"] = play_mode;
  return _inl_mr(this)->call(AVTransport, "SetPlayMode", args);
}

Result MediaRenderer::set_next_av_transport_uri(const string& uri) {
  map<string, string> args;
  args["NextURI"] = uri;
  args["NextURIMetaData"] = "";
  return _inl_mr(this)->call(AVTransport, "SetNextAVTransportURI", args);
}

/***********************urn:schemas-upnp-org:service:RenderingControl:1***********************/

Result MediaRenderer::list_presets() {
  return _inl_mr(this)->call(RenderingControl, "ListPresets");
}

Result MediaRenderer::select_preset(const string& preset_name) {
  map<string, string> args;
  args["PresetName"] = preset_name;
  return _inl_mr(this)->call(RenderingControl, "SelectPreset", args);
}

Result MediaRenderer::get_mute() {
  map<string, string> args;
  args["Channel"] = "Master";
  return _inl_mr(this)->call(RenderingControl, "GetMute", args);
}

Result MediaRenderer::set_mute(bool mute) {
  map<string, string> args;
  args["Channel"] = "Master";
  args["DesiredMute"] = mute ? "0" : "1";
  return _inl_mr(this)->call(RenderingControl, "SetMute", args);
}

Result MediaRenderer::get_volume() {
  map<string, string> args;
  args["Channel"] = "Master";
  return _inl_mr(this)->call(RenderingControl, "GetVolume", args);
}

Result MediaRenderer::set_volume(int volume) {
  map<string, string> args;
  args["Channel"] = "Master";
  args["DesiredVolume"] = Util::to_string( D_MAX(0, D_MIN(volume, 100)) );
  return _inl_mr(this)->call(RenderingControl, "SetVolume", args);
}

dlna_ns_end
