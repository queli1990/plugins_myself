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

#include "dial.h"
#include "http.h"

dlna_ns

typedef Service::Result Result;

enum DIAL_COMMAND {
  COMMAND_LAUNCH,
  COMMAND_STATUS,
  COMMAND_KILL,
  COMMAND_HIDE
};
const char* DIAL_COMMAND_STR[] = { "LAUNCH", "STATUS", "KILL", "HIDE" };

Dial::Dial( const string& location ): Service( location ) {
  m_apps_url = m_init_headers["application-url"];
  if ( ! m_apps_url.empty() ) {
    if (m_apps_url.substr(m_apps_url.length() - 1, 1) != "/") {
      m_apps_url.append("/");
    }
  }
}

static int send_command(const string& url,
                       int command,
                       const string& payload,
                       string& responseHeaders, string& responseBody ) {
  HttpRequest req;
  
  if (command == COMMAND_LAUNCH || command == COMMAND_HIDE) {
    req.set_method("POST");

    if ( payload.length() ) {
      // req.set_headers("Expect", "");
      req.set_data(payload);
    }
  } else if (command == COMMAND_KILL) {
    req.set_method("DELETE");
  }
  
  req.set_url(url);
  req.send();
  
  responseHeaders = req.response().header;
  responseBody = req.response().body;
  return req.response().rc;
}

Result Dial::launch(const string& app, const string& payload )
{
  ATRACE("%s: Launch %s", __FUNCTION__, app.c_str());
  string appUrl = m_apps_url;
  string responseHeaders, responseBody;
  int status = send_command( appUrl.append(app),
                            COMMAND_LAUNCH, payload, responseHeaders, responseBody);

  if ( status ) {
    return { -102, "Send http request error" };
  } else {
    return { 0 };
  }
}

Result Dial::status( const string& app )
{
  ATRACE("%s: GetStatus %s", __FUNCTION__, app.c_str());
  string emptyPayload;
  string url = m_apps_url + app;
  string responseHeaders, responseBody;
  int status = send_command( url, COMMAND_STATUS, emptyPayload, responseHeaders, responseBody );

  if ( ! status ) return { -102, "Send http request error" };
  
  ATRACE("Body: %s", responseBody.c_str());
  long found = responseBody.find("href=");
  
  if( found != -1 ) {
    // The start of href is after href= and the quote
    long href_start = found + 5 + 1;

    // get the body from the start of href to the end, then find 
    // the last quote delimiter.
    string tmp = responseBody.substr( href_start );
    long href_end = tmp.find("\"");
    m_stop_end_point = responseBody.substr( href_start, href_end );
  }

  JSONValue data( rapidjson::kObjectType );
  data["headers"] = JSONValue(responseHeaders.c_str(), responseHeaders.length());
  data["body"] = JSONValue(responseBody.c_str(), responseBody.length());

  return { 0, "", std::move(data) };
}

Result Dial::stop( const string& app )
{
  ATRACE("%s: Stop %s", __FUNCTION__, app.c_str());
  string emptyPayload;  // dropping this
  // just call status to update the run endpoint
  string responseHeaders, responseBody;
  status( app );
  
  string url = m_apps_url + app + "/" + m_stop_end_point;
  int status = send_command(url, COMMAND_KILL, emptyPayload, responseHeaders, responseBody );

  if ( status ) {
    return { -102, "Send http request error" };
  } else {
    return { 0 };
  }
}

Result Dial::hide( const string& app )
{
  ATRACE("%s: Hide %s", __FUNCTION__, app.c_str());
  string emptyPayload;
  // just call app to update the run endpoint
  status( app );
  
  string url = m_apps_url + app + "/" + m_stop_end_point + "/hide";
  string responseHeaders, responseBody;
  int status = send_command(url, COMMAND_HIDE, emptyPayload, responseHeaders, responseBody );

  if ( status ) {
    return { -102, "Send http request error" };
  } else {
    return { 0 };
  }
}

dlna_ns_end
