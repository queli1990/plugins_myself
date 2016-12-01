package com.gochinatv;

import android.support.annotation.Nullable;
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

  @Nullable
  public static Result parse_result(String json)  {
    JSONObject o = (JSONObject)parse_json(json);
    assert o != null;
    Result r = new Result();

    try {
      r.code = o.getInt("code");
      r.error = o.getString("error");
      r.data = o.getJSONObject("data");
    } catch (JSONException e) {
      e.printStackTrace();
    }

    return r;
  }

}
