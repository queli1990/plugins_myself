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

#include <jni.h>
#include <stdio.h>
#include <assert.h>
#include "discovery.h"
#include "dial.h"
#include "media-renderer.h"

using namespace dlna;

typedef Service::Result Result;

class  AndroidDiscovery;
static AndroidDiscovery* g_discovery = NULL;
static JavaVM* javavm = NULL;

#define RETURN_STRING(s) return get_env()->NewStringUTF(s)
#define NEW_UTF8Chars(arg) const char* v##arg = get_env()->GetStringUTFChars(arg, NULL); assert(v##arg)
#define DEL_UTF8Chars(arg) get_env()->ReleaseStringUTFChars(arg, v##arg)
#define Verification_HOST(host, type) \
  NEW_UTF8Chars(host); \
  Service* service = g_discovery->service(v##host, type); \
  DEL_UTF8Chars(host); \
  if ( ! service ) { RETURN_STRING("{ \"code\": 0, \"error\":\"Host not found\" }"); }

struct JNIMethodInfo {
  JNIEnv *    env;
  jclass      class_id;
  jmethodID   method_id;
};

static JNIEnv* get_env() {
  JNIEnv* env = NULL;
  if ( javavm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK ) {
    if ( javavm->AttachCurrentThread(&env, NULL) != JNI_OK ) {
      return NULL;
    }
  }
  return env;
}

static bool get_method_info(JNIMethodInfo& info,
                            const char* class_name,
                            const char* method_name, const char* param_code, bool is_static = false) {
  JNIEnv* env = get_env();
  if ( env ) {
    jclass class_id = env->FindClass(class_name);
    jmethodID method_id = NULL;
    if ( is_static ) {
      method_id = env->GetStaticMethodID(class_id, method_name, param_code);
    } else {
      method_id = env->GetMethodID(class_id, method_name, param_code);
    }
    if ( method_id ) {
      info.class_id = class_id;
      info.env = env;
      info.method_id = method_id;
      return true;
    }
  }
  return false;
}

static jstring to_json_jstring( Result& result ) {
  string json = result.to_json_string();
  RETURN_STRING(json.c_str());
}

class AndroidDiscovery: public Discovery, public Discovery::Delegate {
public:

  AndroidDiscovery(): m_android_delegate(NULL), m_is_info(false) {
    JNIMethodInfo info;
    m_is_info = get_method_info(info, "com/gochinatv/Discovery", "discovery_change", "()V", true);
    m_discovery_class_id = (jclass)info.env->NewGlobalRef(info.class_id);
    m_discovery_change_method_id = info.method_id;
    set_delegate(this);
  }

  virtual void discovery_change(Discovery* dis) {
    if ( m_is_info ) {
      get_env()->CallStaticVoidMethod( m_discovery_class_id, m_discovery_change_method_id );
    }
  }

  virtual void discovery_error(Discovery* dis, const string& err) {

  }

  static jstring to_json_jstring( vector<Service*>& services ) {
    string data = "[";
    for (size_t i = 0; i < services.size(); i++) {
      if (i) data.append(",");
      data.append("{");
      data.append("\"hostname\":\""); data.append( Util::to_json_string(services[i]->get_hostname()) ); data.append("\",");
      data.append("\"port\":"); data.append( Util::to_string(services[i]->get_port()) ); data.append(",");
      data.append("\"location\":\""); data.append( Util::to_json_string(services[i]->get_location()) ); data.append("\",");
      data.append("\"friendly_name\":\""); data.append( Util::to_json_string(services[i]->get_friendly_name()) ); data.append("\",");
      data.append("\"uuid\":\""); data.append( Util::to_json_string(services[i]->get_uuid()) ); data.append("\",");
      data.append("\"type\":\""); data.append( services[i]->as_dial() ? "DIAL": "MEDIA_RENDERER" ); data.append("\",");
      data.append("\"is_invalid\":"); data.append( services[i]->is_invalid() ? "true": "false" );
      data.append("}");
    }
    data.append("]");
    RETURN_STRING(data.c_str());
  }

private:
  jobject m_android_delegate;
  jclass    m_discovery_class_id;
  jmethodID m_discovery_change_method_id;
  bool m_is_info;
};

extern "C" {

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  javavm = vm;
  g_discovery = new AndroidDiscovery(); assert(g_discovery);
  return JNI_VERSION_1_6;
}

// 以下是接口函数

void Java_com_gochinatv_Discovery_scanInl(JNIEnv* env, jclass clazz, jstring type) {
  Discovery::ServiceType service_type = Discovery::ALL;

  NEW_UTF8Chars(type);
  if ( string("DIAL") == vtype ) {
    service_type = Discovery::DIAL;
  } else if ( string("MEDIA_RENDERER") == vtype ) {
    service_type = Discovery::MEDIA_RENDERER;
  }
  DEL_UTF8Chars(type);

  g_discovery->scan( service_type );
}

void Java_com_gochinatv_Discovery_stopInl(JNIEnv* env, jclass clazz) {
  g_discovery->stop();
}

jstring Java_com_gochinatv_Discovery_allServiceInl(JNIEnv* env, jclass clazz) {
  vector<Service*> ls = g_discovery->all_service();
  return AndroidDiscovery::to_json_jstring(ls);
}

jstring Java_com_gochinatv_Discovery_serviceInl(JNIEnv* env, jclass clazz, jstring type) {
  Discovery::ServiceType service_type = Discovery::ALL;

  NEW_UTF8Chars(type);
  if ( string("DIAL") == vtype ) {
    service_type = Discovery::DIAL;
  } else if ( string("MEDIA_RENDERER") == vtype ) {
    service_type = Discovery::MEDIA_RENDERER;
  }
  DEL_UTF8Chars(type);

  vector<Service*> ls = g_discovery->service(service_type);
  return AndroidDiscovery::to_json_jstring(ls);
}

void Java_com_gochinatv_Discovery_setSendMcastIntervalInl(JNIEnv* env, jclass clazz, jint second) {
  g_discovery->set_send_mcast_interval(second);
}

// Dial

jstring Java_com_gochinatv_Dial_launchInl(JNIEnv* env, jclass clazz, jstring host, jstring app, jstring payload) {
  Verification_HOST(host, Discovery::DIAL);
  NEW_UTF8Chars(app);
  NEW_UTF8Chars(payload);
  Result res = service->as_dial()->launch(vapp, vpayload);
  DEL_UTF8Chars(app);
  DEL_UTF8Chars(payload);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_Dial_hideInl(JNIEnv* env, jclass clazz, jstring host, jstring app) {
  Verification_HOST(host, Discovery::DIAL);
  NEW_UTF8Chars(app);
  Result res = service->as_dial()->hide(vapp);
  DEL_UTF8Chars(app);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_Dial_stopInl(JNIEnv* env, jclass clazz, jstring host, jstring app) {
  Verification_HOST(host, Discovery::DIAL);
  NEW_UTF8Chars(app);
  Result res = service->as_dial()->stop(vapp);
  DEL_UTF8Chars(app);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_Dial_statusInl(JNIEnv* env, jclass clazz, jstring host, jstring app) {
  Verification_HOST(host, Discovery::DIAL);
  NEW_UTF8Chars(app);
  Result res = service->as_dial()->status(vapp);
  DEL_UTF8Chars(app);
  return to_json_jstring(res);
}

// MediaRenderer

jstring Java_com_gochinatv_MediaRenderer_setAVTransportURIInl(JNIEnv* env, jclass clazz, jstring host, jstring uri) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  NEW_UTF8Chars(uri);
  Result res = service->as_media_renderer()->set_av_transport_uri(vuri);
  DEL_UTF8Chars(uri);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_getMediaInfoInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->get_media_info();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_getDeviceCapabilitiesInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->get_device_capabilities();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_getTransportInfoInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->get_transport_info();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_getPositionInfoInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->get_position_info();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_getTransportSettingsInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->get_transport_settings();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_stopInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->stop();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_playInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->play();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_pauseInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->pause();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_seekInl(JNIEnv* env, jclass clazz, jstring host, jlong time) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->seek(time);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_nextInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->next();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_previousInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->previous();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_setRecordQualityModeInl(JNIEnv* env, jclass clazz, jstring host, jstring record_quality_mode) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  NEW_UTF8Chars(record_quality_mode);
  Result res = service->as_media_renderer()->set_record_quality_mode(vrecord_quality_mode);
  DEL_UTF8Chars(record_quality_mode);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_recordInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->record();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_getCurrentTransportActionsInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->get_current_transport_actions();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_setPlayModeInl(JNIEnv* env, jclass clazz, jstring host, jstring play_mode) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  NEW_UTF8Chars(play_mode);
  Result res = service->as_media_renderer()->set_play_mode(vplay_mode);
  DEL_UTF8Chars(play_mode);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_setNextAVTransportURIInl(JNIEnv* env, jclass clazz, jstring host, jstring uri) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  NEW_UTF8Chars(uri);
  Result res = service->as_media_renderer()->set_next_av_transport_uri(vuri);
  DEL_UTF8Chars(uri);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_listPresetsInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->list_presets();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_selectPresetInl(JNIEnv* env, jclass clazz, jstring host, jstring preset_name) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  NEW_UTF8Chars(preset_name);
  Result res = service->as_media_renderer()->select_preset(vpreset_name);
  DEL_UTF8Chars(preset_name);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_getMuteInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->get_mute();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_setMuteInl(JNIEnv* env, jclass clazz, jstring host, jboolean mute) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->set_mute(mute);
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_getVolumeInl(JNIEnv* env, jclass clazz, jstring host) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->get_volume();
  return to_json_jstring(res);
}

jstring Java_com_gochinatv_MediaRenderer_setVolumeInl(JNIEnv* env, jclass clazz, jstring host, jint volume) {
  Verification_HOST(host, Discovery::MEDIA_RENDERER);
  Result res = service->as_media_renderer()->set_volume(volume);
  return to_json_jstring(res);
}

}