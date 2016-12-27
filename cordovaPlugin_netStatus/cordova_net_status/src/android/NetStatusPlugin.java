package com.vego.vegotv.plugin.netstatus;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by fq_mbp on 2016/12/26.
 */

public class NetStatusPlugin extends CordovaPlugin {

    static CallbackContext callbackContext;

    @Override
    public boolean execute(String action, JSONArray args, final CallbackContext callbackContext) throws JSONException {
        Log.e("TAG", "action: " + action);
        if ("getNetStatus".equals(action)) {
            // 获取当前的网络状态
            JSONObject json = NetUtils.getNetworkType(cordova.getActivity());
            callbackContext.success(json);
        } else {
            // 注册网络广播，监听实时网络状态
            NetStatusPlugin.callbackContext = callbackContext;

        }
        return true;
    }

    public static class NetStatusBroadCast extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent == null) {
                return;
            }
            if(callbackContext != null){
                Log.e("TAG", "callbackContext: ");
                PluginResult pluginResult = new PluginResult(PluginResult.Status.OK, NetUtils.getNetworkType(context));
                callbackContext.sendPluginResult(pluginResult);
            }
            
        }


    }

}


