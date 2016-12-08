package com.gochinatv;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

public class Util {

  static public class Result {
    public int code;
    public String error;
    public JSONObject data;
  }

  public static Object parse_json(String json) {
    JSONTokener parser = new JSONTokener(json);
    try {
      return parser.nextValue();
    } catch (JSONException ex) { }
    return null;
  }

  public static Result parse_result(String json)  {
    JSONObject o = (JSONObject)parse_json(json);
    assert o != null;
    Result r = new Result();

    try {
      if(o == null){
        return null;
      }
      r.code = o.getInt("code");
      r.error = o.getString("error");
//      r.data = o.getJSONObject("data");
    } catch (JSONException e) {
      e.printStackTrace();
    }

    return r;
  }

  public static String secToTime(long paramLong) {
    int time = new Long(paramLong).intValue();
    String timeStr = null;
    int hour = 0;
    int minute = 0;
    int second = 0;
    if (time <= 0)
      return "00:00:00";
    else {
      minute = time / 60;
      if (minute < 60) {
        second = time % 60;
        timeStr = "00:" + unitFormat(minute) + ":" + unitFormat(second);
      } else {
        hour = minute / 60;
        if (hour > 99)
          return "99:59:59";
        minute = minute % 60;
        second = time - hour * 3600 - minute * 60;
        timeStr = unitFormat(hour) + ":" + unitFormat(minute) + ":"
                + unitFormat(second);
      }
    }
    return timeStr;
  }

  private static String unitFormat(int i) {
    String retStr = null;
    if (i >= 0 && i < 10)
      retStr = "0" + Integer.toString(i);
    else
      retStr = "" + i;
    return retStr;
  }

}
