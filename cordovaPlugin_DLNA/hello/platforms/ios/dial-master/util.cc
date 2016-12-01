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

#include "util.h"
#include "http.h"

dlna_ns

vector<string> Util::split(const string& str, string sp_string) {
  vector<string> rv;
  long sp_stringLen = sp_string.size();
  long lastPosition = 0;
  long index = -1;
  while( -1 != (index = str.find(sp_string , lastPosition)) ) {
    rv.push_back(str.substr(lastPosition, index - lastPosition));
    lastPosition = index + sp_stringLen;
  }
  string lastStr = str.substr(lastPosition);
  if ( !lastStr.empty() ) {
    rv.push_back(lastStr);
  }
  return rv;
}

string Util::replace(const string& str, string org_str, string rep_str) {
  
  vector<string> delimVec = split(str, org_str);
  if (delimVec.size() < 2) {
    return str;
  }
  
  vector<string>::iterator it = delimVec.begin();
  string target = *it; it++;
  
  for (; it != delimVec.end(); ++it) {
    target = target + (rep_str + (*it));
  }
  return target;
}

string Util::to_json_string(const string& str) {
  return replace(replace(str, "\\", "\\\\"), "\"", "\\\"");
}

string Util::get_hostname(const string& url) {
  // http://192.168.1.103:36269/apps/
  return URI(url).hostname();
}

int Util::get_port(const string& url) {
  // http://192.168.1.103:36269/apps/
  return URI(url).port();
}

string Util::to_string(int num) {
  char s[21];
  sprintf(s, "%d", num);
  return s;
}

// find xml

class FindXMLVisitor: public XMLVisitor {
public:
  XMLElementWrap result() {
    return elemebts.size() ? elemebts[0] : NULL;
  }
  vector<XMLElementWrap> results() {
    return std::move( elemebts );
  }
  vector<XMLElementWrap> elemebts;
  string find_value;
};

class FindElementByIdXMLVisitor: public FindXMLVisitor {
public:
  virtual bool VisitEnter( const XMLElement& element, const XMLAttribute* firstAttribute )	{
    const XMLAttribute* id = element.FindAttribute("id");
    if ( id && this->find_value == id->Value() ) {
      elemebts.push_back( const_cast<XMLElement*>(&element) );
      return false; // end find
    }
    return true;
  }
};

class FindElementsByTagXMLVisitor: public FindXMLVisitor {
public:
  virtual bool VisitEnter( const XMLElement& element, const XMLAttribute* firstAttribute )	{
    if ( this->find_value == element.Name() ) {
      elemebts.push_back( const_cast<XMLElement*>(&element) );
      if ( elemebts.size() == max_result ) {
        return false; // end find
      }
    }
    return true;
  }
  int max_result;
};

XMLElementWrap XMLNodeWrap::find_element_by_id(const string& id) {
  FindElementByIdXMLVisitor visitor;
  visitor.find_value = id;
  m_node->Accept(&visitor);
  return visitor.result();
}
XMLElementWrap XMLNodeWrap::find_element_by_tag(const string& tag) {
  FindElementsByTagXMLVisitor visitor;
  visitor.find_value = tag;
  visitor.max_result = 1;
  m_node->Accept(&visitor);
  return visitor.result();
}
vector<XMLElementWrap> XMLNodeWrap::find_elements_by_tag(const string& tag) {
  FindElementsByTagXMLVisitor visitor;
  visitor.find_value = tag;
  visitor.max_result = 0;
  m_node->Accept(&visitor);
  return visitor.results();
}
vector<XMLNodeWrap> XMLElementWrap::child_nodes() {
  XMLElement* el = static_cast<XMLElement*>(m_node);
  vector<XMLNodeWrap> rv;
  XMLNode* node = el->FirstChild();
  while ( node ) {
    rv.push_back( node );
    node = node->NextSibling();
  }
  return rv;
}
vector<XMLElementWrap> XMLElementWrap::child_elements() {
  XMLElement* el = static_cast<XMLElement*>(m_node);
  vector<XMLElementWrap> rv;
  XMLElement* node = el->FirstChildElement();
  while ( node ) {
    rv.push_back( node );
    node = node->NextSiblingElement();
  }
  return rv;
}
XMLNodeWrap XMLElementWrap::child_node(int index) {
  XMLElement* el = static_cast<XMLElement*>(m_node);
  int i = 0;
  XMLNode* node = el->FirstChild();
  while ( node ) {
    if ( i == index ) {
      return node;
    }
    node = node->NextSibling(); i++;
  }
  return NULL;
}
XMLElementWrap XMLElementWrap::child_element(int index) {
  XMLElement* el = static_cast<XMLElement*>(m_node);
  int i = 0;
  XMLElement* node = el->FirstChildElement();
  while ( node ) {
    if ( i == index ) {
      return node;
    }
    node = node->NextSiblingElement(); i++;
  }
  return NULL;
}

dlna_ns_end
