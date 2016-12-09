package com.gochinatv;


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
  private static native String setRecordQualityModeInl(String host, String record_quality_mode);
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
  public static Util.Result setAVTransportURI(String host, String uri) {
    return Util.parse_result( setAVTransportURIInl(host, uri) );
  }
  public static Util.Result getMediaInfo(String host) {
    return Util.parse_result( getMediaInfoInl(host) );
  }
  public static Util.Result getDeviceCapabilities(String host) {
    return Util.parse_result( getDeviceCapabilitiesInl(host) );
  }
  public static Util.Result getTransportInfo(String host)  {
    return Util.parse_result( getTransportInfoInl(host) );
  }
  public static Util.Result getPositionInfo(String host) {
    return Util.parse_result( getPositionInfoInl(host) );
  }
  public static Util.Result getTransportSettings(String host) {
    return Util.parse_result( getTransportSettingsInl(host) );
  }
  public static Util.Result stop(String host) {
    return Util.parse_result( stopInl(host) );
  }
  public static Util.Result play(String host) {
    return Util.parse_result( playInl(host) );
  }
  public static Util.Result pause(String host) {
    return Util.parse_result( pauseInl(host) );
  }
  public static Util.Result seek(String host, long time) {
    return Util.parse_result( seekInl(host, time) );
  }
  public static Util.Result next(String host) {
    return Util.parse_result( nextInl(host) );
  }
  public static Util.Result previous(String host) {
    return Util.parse_result( previousInl(host) );
  }
  public static Util.Result setRecordQualityMode(String host, String record_quality_mode) {
    return Util.parse_result( setRecordQualityModeInl(host, record_quality_mode) );
  }
  public static Util.Result record(String host) {
    return Util.parse_result( recordInl(host) );
  }
  public static Util.Result getCurrentTransportActions(String host) {
    return Util.parse_result( getCurrentTransportActionsInl(host) );
  }
  public static Util.Result setPlayMode(String host, String play_mode) {
    return Util.parse_result( setPlayModeInl(host, play_mode) );
  }
  public static Util.Result setNextAVTransportURI(String host, String uri) {
    return Util.parse_result( setNextAVTransportURIInl(host, uri) );
  }
  public static Util.Result listPresets(String host) {
    return Util.parse_result( listPresetsInl(host) );
  }
  public static Util.Result selectPreset(String host, String preset_name) {
    return Util.parse_result( selectPresetInl(host, preset_name) );
  }
  public static Util.Result getMute(String host) {
    return Util.parse_result( getMuteInl(host) );
  }
  public static Util.Result setMute(String host, boolean mute) {
    return Util.parse_result( setMuteInl(host, mute) );
  }
  public static Util.Result getVolume(String host) {
    return Util.parse_result( getVolumeInl(host) );
  }
  public static Util.Result setVolume(String host, int volume) {
    return Util.parse_result( setVolumeInl(host, volume) );
  }
}