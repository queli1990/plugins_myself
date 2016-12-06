package com.gochinatv;

import com.gochinatv.Util.Result;

public class MediaRenderer {
  private static native String setAVTransportURIInl(String host, String uri);
  private static native String getMediaInfoInl(String host) ;
  private static native String getDeviceCapabilitiesInl(String host);
  private static native String getTransportInfoInl(String host);
  private static native String getPositionInfoInl(String host);
  private static native String getTransportSettingsInl(String host);
  private static native String stopInl(String host);
  private static native String playInl(String host);
  private static native String pauseInl(String host);
  private static native String seekInl(String host, long time);
  private static native String nextInl(String host);
  private static native String previousInl(String host);
  private static native String setRecordQualityModeInl(String host,String record_quality_mode);
  private static native String recordInl(String host);
  private static native String getCurrentTransportActionsInl(String host);
  private static native String setPlayModeInl(String host, String play_mode);
  private static native String setNextAVTransportURIInl(String host, String uri);
  private static native String listPresetsInl(String host);
  private static native String selectPresetInl(String host, String preset_name);
  private static native String getMuteInl(String host);
  private static native String setMuteInl(String host, boolean mute);
  private static native String getVolumeInl(String host);
  private static native String setVolumeInl(String host, int volume);
  //
  public static Result setAVTransportURI(String host, String uri) {
    return Util.parse_result( setAVTransportURIInl(host, uri) );
  }
  public static Result getMediaInfo(String host) {
    return Util.parse_result( getMediaInfoInl(host) );
  }
  public static Result getDeviceCapabilities(String host) {
    return Util.parse_result( getDeviceCapabilitiesInl(host) );
  }
  public static Result getTransportInfo(String host)  {
    return Util.parse_result( getTransportInfoInl(host) );
  }
  public static Result getPositionInfo(String host) {
    return Util.parse_result( getPositionInfoInl(host) );
  }
  public static Result getTransportSettings(String host) {
    return Util.parse_result( getTransportSettingsInl(host) );
  }
  public static Result stop(String host) {
    return Util.parse_result( stopInl(host) );
  }
  public static Result play(String host) {
    return Util.parse_result( playInl(host) );
  }
  public static Result pause(String host) {
    return Util.parse_result( pauseInl(host) );
  }
  public static Result seek(String host, long time) {
    return Util.parse_result( seekInl(host, time) );
  }
  public static Result next(String host) {
    return Util.parse_result( nextInl(host) );
  }
  public static Result previous(String host) {
    return Util.parse_result( previousInl(host) );
  }
  public static Result setRecordQualityMode(String host, String record_quality_mode) {
    return Util.parse_result( setRecordQualityModeInl(host, record_quality_mode) );
  }
  public static Result record(String host) {
    return Util.parse_result( recordInl(host) );
  }
  public static Result getCurrentTransportActions(String host) {
    return Util.parse_result( getCurrentTransportActionsInl(host) );
  }
  public static Result setPlayMode(String host, String play_mode) {
    return Util.parse_result( setPlayModeInl(host, play_mode) );
  }
  public static Result setNextAVTransportURI(String host, String uri) {
    return Util.parse_result( setNextAVTransportURIInl(host, uri) );
  }
  public static Result listPresets(String host) {
    return Util.parse_result( listPresetsInl(host) );
  }
  public static Result selectPreset(String host, String preset_name) {
    return Util.parse_result( selectPresetInl(host, preset_name) );
  }
  public static Result getMute(String host) {
    return Util.parse_result( getMuteInl(host) );
  }
  public static Result setMute(String host, boolean mute) {
    return Util.parse_result( setMuteInl(host, mute) );
  }
  public static Result getVolume(String host) {
    return Util.parse_result( getVolumeInl(host) );
  }
  public static Result setVolume(String host, int volume) {
    return Util.parse_result( setVolumeInl(host, volume) );
  }
}