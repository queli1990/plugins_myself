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

#include "log.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define string_format(format, str)              \
string str;                                     \
va_list __arg;                                  \
va_start(__arg, format);                        \
char* __buf = NULL;                             \
vasprintf(&__buf, format, __arg);               \
if (__buf) {                                    \
  str = string( __buf );                        \
  ::free(__buf);                                \
}                                               \
va_end(__arg)

#ifdef __native_client__

#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/var_dictionary.h>

using namespace pp;

extern Instance* g_instance;

void console_log(const char* s, ...) {
	string_format(s, log);
	VarDictionary rv;
	rv.Set("tag", "CONSOLE_LOG");
	rv.Set("data", log);
	g_instance->PostMessage(rv);
}

void console_err(const char* s, ...) {
	string_format(s, log);
	VarDictionary rv;
	rv.Set("tag", "CONSOLE_ERR");
	rv.Set("data", log);
	g_instance->PostMessage(rv);
}

#elif defined(__ANDROID__)

#include <android/log.h>

void console_log(const char* s, ...) {
  string_format(s, log);
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s\n", log.c_str());
}

void console_err(const char* s, ...) {
  string_format(s, log);
	__android_log_print(ANDROID_LOG_ERROR, "ERR", "%s\n", log.c_str());
}

#else

void console_log(const char* s, ...) {
	string_format(s, log);
	printf("%s\n", log.c_str());
}

void console_err(const char* s, ...) {
	string_format(s, log);
	fprintf(stderr, "%s\n", log.c_str());
}

#endif
