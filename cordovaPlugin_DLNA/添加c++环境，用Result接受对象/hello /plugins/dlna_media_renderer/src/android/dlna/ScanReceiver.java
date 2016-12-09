package com.vego.tv.plugin.dlna;

import android.util.Log;

import com.gochinatv.Discovery;
import com.gochinatv.Util;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Created by fq_mbp on 2016/12/7.
 */

public class ScanReceiver implements IActionReceiver {
    @Override
    public Util.Result execute() {
        Discovery.scan("ALL");
        Util.Result result = new Util.Result();
        result.error = Discovery.allService().toString();
        result.code = -1001;
        return result;
    }
}
