package com.gochinatv;

import android.os.Handler;
import android.os.Looper;
import android.support.annotation.UiThread;

import org.json.JSONArray;

public class Discovery {

  public interface Delegate {
    void discovery_change();
    void discovery_error(String err);
  }

  private static Delegate g_delegate = null;

  public static void discovery_change() {
    if ( g_delegate != null ) {
      new Handler( Looper.getMainLooper() ).post(new Runnable() {
        @Override
        public void run() {
          g_delegate.discovery_change();
        }
      });
    }
  }

  public static void discovery_error( final String err ) {
    if ( g_delegate != null ) {
      new Handler( Looper.getMainLooper() ).post(new Runnable() {
        @Override
        public void run() {
          g_delegate.discovery_error(err);
        }
      });
    }
  }

  private static native void scanInl(String type);
  private static native void stopInl();
  private static native String allServiceInl();
  private static native String serviceInl(String type);
  private static native void setSendMcastIntervalInl(int second);

  public static void scan(String type) {
    scanInl(type);
  }
  public static void stop() {
    stopInl();
  }
  public static JSONArray allService() {
    String s = allServiceInl();
    return (JSONArray) Util.parse_json( s );
  }
  public static JSONArray service(String type) {
    String s = serviceInl(type);
    return (JSONArray) Util.parse_json( s );
  }
  public static void setSendMcastInterval(int second) {
    setSendMcastIntervalInl(second);
  }
  public static void setDelegate(Delegate delegate) {
    g_delegate = delegate;
  }

  static {
    System.loadLibrary("dlnaclient");
  }
}
