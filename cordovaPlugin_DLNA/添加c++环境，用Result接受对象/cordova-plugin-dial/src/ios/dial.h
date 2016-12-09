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

#ifndef dlna_dial_h
#define dlna_dial_h

#include "service.h"

dlna_ns

/**
 * @class Dial
 */
class Dial: public Service {
public:
  
  /**
   * Dial Server ctor
   *
   */
  Dial( const string& location );
  
  virtual ~Dial() { ATRACE("%s\n", __FUNCTION__); }
  
  /**
   * @overwrite
   */
  virtual Dial* as_dial() { return this; }
  
  /**
   * Launch a DIAL application
   *
   * @param[in] app Name of the application to launch
   * @param[in] payload launch POST data
   *
   * @return 0 if successful, !0 otherwise
   */
  Result launch(const string& app, const string& payload );

  /**
   * Hide a DIAL application
   *
   * @param[in] app Name of the application to launch
   *
   * @return 0 if successful, !0 otherwise
   */
  Result hide( const string& app );
  
  /**
   * Stop an application.
   *
   * @param[in] app Name of the application to stop
   *
   * @return 0 if successful, !0 otherwise
   */
  Result stop( const string& app );
  
  /**
   * Get the status of a DIAL application
   *
   * @param[in] app Name of the application to query
   *
   * @return 0 if successful, !0 otherwise
   */
  Result status( const string& app );
  
private:
  string m_apps_url;
  string m_stop_end_point;
};

dlna_ns_end
#endif
