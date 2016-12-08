package com.gochinatv;


public class Dial {
  private static native String launchInl(String host, String app, String payload);
  private static native String hideInl(String host, String app);
  private static native String stopInl(String host, String app);
  private static native String statusInl(String host, String app);
  //
  public static Util.Result launch(String host, String app, String payload) {
    return Util.parse_result(launchInl(host, app, payload));
  }
  public static Util.Result hide(String host, String app) {
    return Util.parse_result(hideInl(host, app));
  }
  public static Util.Result stop(String host, String app) {
    return Util.parse_result(stopInl(host, app));
  }
  public static Util.Result status(String host, String app) {
    return Util.parse_result(statusInl(host, app));
  }
}

