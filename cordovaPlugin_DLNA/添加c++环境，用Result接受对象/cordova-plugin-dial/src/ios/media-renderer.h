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

#ifndef dlna_media_renderer_h
#define dlna_media_renderer_h

#include "service.h"

dlna_ns

/**
 * @class MediaRenderer
 */
class MediaRenderer: public Service {
public:

  /**
   * @constructor
   */
  MediaRenderer( const string& location );
  
  /**
   * @destructor
   */
  virtual ~MediaRenderer();
  
  /**
   * @overwrite
   */
  virtual MediaRenderer* as_media_renderer() { return this; }
  
  /************************urn:schemas-upnp-org:service:AVTransport:1************************/
  
  /**
   *  <action>
   *    <name>SetAVTransportURI</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentURI</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>AVTransportURI</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentURIMetaData</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>AVTransportURIMetaData</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func set_av_transport_uri
   */
  Result set_av_transport_uri(const string& uri);
  
  /**
   *  <action>
   *    <name>GetMediaInfo</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>NrTracks</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>NumberOfTracks</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>MediaDuration</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>CurrentMediaDuration</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentURI</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>AVTransportURI</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentURIMetaData</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>AVTransportURIMetaData</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>NextURI</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>NextAVTransportURI</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>NextURIMetaData</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>NextAVTransportURIMetaData</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>PlayMedium</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>PlaybackStorageMedium</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>RecordMedium</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>RecordStorageMedium</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>WriteStatus</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>RecordMediumWriteStatus</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func get_media_info
   */
  Result get_media_info();
  
  /**
   *  <action>
   *    <name>GetDeviceCapabilities</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>PlayMedia</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>PossiblePlaybackStorageMedia</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>RecMedia</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>PossibleRecordStorageMedia</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>RecQualityModes</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>PossibleRecordQualityModes</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func get_device_capabilities
   */
  Result get_device_capabilities();
  
  /**
   *  <action>
   *    <name>GetTransportInfo</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentTransportState</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>TransportState</relatedStateVariable>
   *        </argument>
   *      <argument>
   *        <name>CurrentTransportStatus</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>TransportStatus</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentSpeed</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>TransportPlaySpeed</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func get_transport_info
   */
  Result get_transport_info();
  
  /**
   *  <action>
   *    <name>GetPositionInfo</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Track</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>CurrentTrack</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>TrackDuration</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>CurrentTrackDuration</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>TrackMetaData</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>CurrentTrackMetaData</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>TrackURI</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>CurrentTrackURI</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>RelTime</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>RelativeTimePosition</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>AbsTime</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>AbsoluteTimePosition</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>RelCount</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>RelativeCounterPosition</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>AbsCount</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>AbsoluteCounterPosition</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func get_position_info
   */
  Result get_position_info();
  
  /**
   *  <action>
   *    <name>GetTransportSettings</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>PlayMode</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>CurrentPlayMode</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>RecQualityMode</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>CurrentRecordQualityMode</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func get_transport_settings
   */
  Result get_transport_settings();
  
  /**
   *  <action>
   *  <name>Stop</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func stop
   */
  Result stop();
  
  /**
   *  <action>
   *    <name>Play</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Speed</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>TransportPlaySpeed</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func play
   */
  Result play();
  
  /**
   *  <action>
   *    <name>Pause</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func pause
   */
  Result pause();
  
  /**
   *  <action>
   *    <name>Seek</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Unit</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_SeekMode</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Target</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_SeekTarget</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func seek
   */
  Result seek(long time);
  
  /**
   *  <action>
   *    <name>Next</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func next
   */
  Result next();
  
  /**
   *  <action>
   *    <name>Previous</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func previous
   */
  Result previous();
  
  //##########
  
  /**
   *  <action>
   *    <name>SetRecordQualityMode</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>NewRecordQualityMode</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>CurrentRecordQualityMode</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func set_record_quality_mode
   * @arg record_quality_mode {const string&} # EP|LP|SP|BASIC|MEDIUM|HIGH|NOT_IMPLEMENTED
   */
  Result set_record_quality_mode(const string& record_quality_mode);
  
  /**
   *  <action>
   *    <name>Record</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func record
   */
  Result record();
  
  /**
   *  <action>
   *    <name>GetCurrentTransportActions</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Actions</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>CurrentTransportActions</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func get_current_transport_actions
   */
  Result get_current_transport_actions();
  
  /**
   *  <action>
   *    <name>SetPlayMode</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>NewPlayMode</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>CurrentPlayMode</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func set_play_mode
   * @arg play_mode {const string&} NORMAL|SHUFFLE|REPEAT_ONE|REPEAT_ALL|RANDOM|DIRECT_1|INTRO
   */
  Result set_play_mode(const string& play_mode);
  
  /**
   *  <action>
   *    <name>SetNextAVTransportURI</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>NextURI</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>AVTransportURI</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>NextURIMetaData</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>AVTransportURIMetaData</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func set_next_av_transport_uri
   */
  Result set_next_av_transport_uri(const string& uri);
  
  /***********************urn:schemas-upnp-org:service:RenderingControl:1***********************/
  
  /**
   *  <action>
   *    <name>ListPresets</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentPresetNameList</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>PresetNameList</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func list_presets
   */
  Result list_presets();
  
  /**
   *  <action>
   *    <name>SelectPreset</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>PresetName</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_PresetName</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func select_preset
   */
  Result select_preset(const string& preset_name);
  
  /**
   *  <action>
   *    <name>GetMute</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Channel</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentMute</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>Mute</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func get_mute
   */
  Result get_mute();
  
  /**
   *  <action>
   *    <name>SetMute</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Channel</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>DesiredMute</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>Mute</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func set_mute
   */
  Result set_mute(bool mute);
  
  /**
   *  <action>
   *    <name>GetVolume</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Channel</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>CurrentVolume</name>
   *        <direction>out</direction>
   *        <relatedStateVariable>Volume</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func get_volume
   */
  Result get_volume();
  
  /**
   *  <action>
   *    <name>SetVolume</name>
   *    <argumentList>
   *      <argument>
   *        <name>InstanceID</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>Channel</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
   *      </argument>
   *      <argument>
   *        <name>DesiredVolume</name>
   *        <direction>in</direction>
   *        <relatedStateVariable>Volume</relatedStateVariable>
   *      </argument>
   *    </argumentList>
   *  </action>
   *
   * @func set_volume
   * @arg volume {int} 0-100
   */
  Result set_volume(int volume);
  
private:
  string m_av_transport_mask;
  string m_av_transport_service_id;
  string m_av_transport_control_url;
  string m_rendering_control_mask;
  string m_rendering_control_service_id;
  string m_rendering_control_control_url;
  class _Inl; friend _Inl;
};

dlna_ns_end
#endif
