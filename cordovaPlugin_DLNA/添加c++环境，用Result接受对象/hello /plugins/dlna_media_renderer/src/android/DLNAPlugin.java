package com.vego.tv.plugin;

import android.app.Activity;
import android.util.Log;
import android.widget.Toast;
import com.gochinatv.Discovery;
import com.vego.tv.plugin.dlna.AsyncDlnaTask;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;


/**
 * Created by fq_mbp on 2016/11/16.
 */

public class DLNAPlugin extends CordovaPlugin implements Discovery.Delegate {

    public CallbackContext callbackContext;


    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        Log.e("TAG", "action......" + action);
        executeAction(action, args, callbackContext);
        return true;
    }

    private void executeAction(final String action, final JSONArray args, final CallbackContext callbackContext) {
        new AsyncDlnaTask(DLNAPlugin.this, args, callbackContext).execute(action);
    }



    @Override
    public void discovery_change() {
        try {
            JSONArray services = Discovery.allService();
            // 此处得到json字段，需要将内容返回给js
            if (services == null) {
                sendResult(callbackContext, PluginResult.Status.ERROR, "{\"code\":0,\"error\":\"result is null\",\"data\":{}}");
            } else {
                sendResult(callbackContext, PluginResult.Status.OK, services);
            }
            Log.e("TAG", "discovery_change......" + services.toString());
        } catch (Exception e) {
            sendResult(callbackContext, PluginResult.Status.ERROR, "{\"code\":0,\"error\":\"" + e.getLocalizedMessage() + "\",\"data\":{}}");
        }
    }

    @Override
    public void discovery_error(String err) {
        Log.e("TAG", "discovery_error......");
        sendResult(callbackContext, PluginResult.Status.ERROR, "{\"code\":0,\"error\":\"" + err + "\",\"data\":{}}");
    }

    private void sendResult(CallbackContext callbackContext, PluginResult.Status status, String msg) {
        PluginResult pluginResult = new PluginResult(status, msg);
        pluginResult.setKeepCallback(true);
        callbackContext.sendPluginResult(pluginResult);
    }

    private void sendResult(CallbackContext callbackContext, PluginResult.Status status, JSONArray msg) {
        PluginResult pluginResult = new PluginResult(status, msg);
        pluginResult.setKeepCallback(true);
        callbackContext.sendPluginResult(pluginResult);
    }



}
