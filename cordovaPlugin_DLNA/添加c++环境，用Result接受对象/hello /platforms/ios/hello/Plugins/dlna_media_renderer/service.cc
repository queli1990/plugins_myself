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

#include "service.h"

dlna_ns

string Service::Result::to_json_string() {

  string json = "{\"code\":" + Util::to_string(code) +
                ",\"error\":\"" + Util::to_json_string(error) + "\"" +
                ",\"data\":";

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  data.Accept(writer);
  json.append(buffer.GetString(), buffer.GetSize());
  json.append("}");
  return json;
}

Service::Service(const string& location): m_is_invalid(true) {
  update( location );
  HttpReqponse resp = Http::get(m_init_uri.href());
  if ( resp.rc == 0 ) {
    m_is_invalid = false;
    m_init_headers = std::move(resp.headers);
    if (m_init_xml.Parse(resp.body.c_str()) != XML_NO_ERROR) {
      ATRACE("Parse xml error `%s`", m_init_uri.href().c_str());
      return;
    }

    XMLElementWrap friendlyName = XMLNodeWrap(&m_init_xml).find_element_by_tag("friendlyName");
    XMLElementWrap UDN = XMLNodeWrap(&m_init_xml).find_element_by_tag("UDN");

    if (!friendlyName.is_empty()) {
      m_friendly_name = friendlyName->GetText();
    }
    if (!UDN.is_empty()) {
      m_uuid = UDN->GetText();
    }
    ATRACE("****Friendly name=`%s`, UUID=`%s`", m_friendly_name.c_str(), m_uuid.c_str());
  }
}

void Service::update(const string& location) {
  m_init_uri = URI( location );
}

dlna_ns_end
