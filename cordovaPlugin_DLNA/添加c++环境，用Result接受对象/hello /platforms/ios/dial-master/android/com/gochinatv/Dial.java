package com.gochinatv;

import com.gochinatv.Util.Result;

public class Dial {
  private static native String launchInl(String host, String app, String payload);
  private static native String hideInl(String host, String app);
  private static native String stopInl(String host, String app);
  private static native String statusInl(String host, String app);
  //
  public static Result launch(String host, String app, String payload) {
    return Util.parse_result(launchInl(host, app, payload));
  }
  public static Result hide(String host, String app) {
    return Util.parse_result(hideInl(host, app));
  }
  public static Result stop(String host, String app) {
    return Util.parse_result(stopInl(host, app));
  }
  public static Result status(String host, String app) {
    return Util.parse_result(statusInl(host, app));
  }
}
