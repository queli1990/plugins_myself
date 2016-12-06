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

#ifndef dlna_h
#define dlna_h

#define dlna_ns namespace dlna {
#define dlna_ns_end }
#define D_MIN(A, B)	        ((A) < (B) ? (A) : (B))
#define D_MAX(A, B)	        ((A) > (B) ? (A) : (B))

#include <string>
#include <vector>
#include <tinyxml2.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

using namespace std;
using namespace tinyxml2;

dlna_ns

typedef rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::CrtAllocator> JSONValue;
typedef rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> JSONDocument;
typedef unsigned int uint;

class Util {
public:
  static vector<string> split(const string& str, string sp_string);
  static string replace(const string& str, string org_str, string rep_str);
  static string to_json_string(const string& str);
  static string get_hostname(const string& url);
  static int    get_port(const string& url);
  static string to_string(int num);
};

class XMLElementWrap;
class XMLNodeWrap {
public:
  inline XMLNodeWrap( XMLNode* node ) : m_node(node) { }
  XMLElementWrap find_element_by_id(const string& id);
  XMLElementWrap find_element_by_tag(const string& tag);
  vector<XMLElementWrap> find_elements_by_tag(const string& tag);
  inline XMLNode* operator*() { return m_node; }
  inline XMLNode* operator->() { return m_node; }
  inline bool is_empty() { return !m_node; }
protected:
  XMLNode* m_node;
};

class XMLElementWrap: public XMLNodeWrap {
public:
  inline XMLElementWrap( XMLElement* el ) : XMLNodeWrap(el) { }
  vector<XMLNodeWrap> child_nodes();
  vector<XMLElementWrap> child_elements();
  XMLNodeWrap child_node(int index);
  XMLElementWrap child_element(int index);
  inline XMLElement* operator*() { return static_cast<XMLElement*>(m_node); }
  inline XMLElement* operator->() { return static_cast<XMLElement*>(m_node); }
};

dlna_ns_end
#endif // dlna_h
