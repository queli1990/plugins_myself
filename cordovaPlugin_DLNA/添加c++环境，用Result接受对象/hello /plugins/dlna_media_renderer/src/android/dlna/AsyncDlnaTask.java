package com.vego.tv.plugin.dlna;

import android.os.AsyncTask;
import android.util.Log;
import com.gochinatv.Util;
import com.vego.tv.plugin.DLNAPlugin;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Created by fq_mbp on 2016/12/8.
 */

public class AsyncDlnaTask extends AsyncTask<String, String, Util.Result> {

    private DLNAPlugin dlnaPlugin;
    private JSONArray args;
    private CallbackContext callbackContext;

    public AsyncDlnaTask(DLNAPlugin dlnaPlugin, JSONArray args, CallbackContext callbackContext) {
        this.dlnaPlugin = dlnaPlugin;
        this.args = args;
        this.callbackContext = callbackContext;
    }

    @Override
    protected Util.Result doInBackground(String[] params) {
        String action = params[0];
        IActionReceiver iActionReceiver = null;
        try {
            if (IActionReceiver.ACTION_INIT.equals(action)) {
                iActionReceiver = new InitReceiver(dlnaPlugin);
            } else if (IActionReceiver.ACTION_SCAN.equals(action)) {
                iActionReceiver = new ScanReceiver();
            } else if (IActionReceiver.ACTION_STOP_SCAN.equals(action)) {
                iActionReceiver = new StopScanReceiver();
            } else if (IActionReceiver.ACTION_START.equals(action)) {
                iActionReceiver = new StartReceiver(args.getString(0), args.getString(1));
            } else if (IActionReceiver.ACTION_PLAY.equals(action)) {
                iActionReceiver = new PlayReceiver(args.getString(0));
            } else if (IActionReceiver.ACTION_PAUSE.equals(action)) {
                iActionReceiver = new PauseReceiver(args.getString(0));
            } else if (IActionReceiver.ACTION_SET_VOLUME.equals(action)) {
                iActionReceiver = new VolumeReceiver(args.getString(0), Integer.parseInt(args.getString(1)));
            } else if (IActionReceiver.ACTION_GET_SYS_VOLUME.equals(action)) {
                iActionReceiver = new SysVolumeReceiver(args.getString(0));
            } else if (IActionReceiver.ACTION_GET_VIDEO_INFO.equals(action)) {
                iActionReceiver = new VideoInfoReceiver(args.getString(0));
            } else if (IActionReceiver.ACTION_STOP.equals(action)) {
                iActionReceiver = new StopVideoReceiver(args.getString(0));
            } else if (IActionReceiver.ACTION_SEEK.equals(action)) {
                iActionReceiver = new SeekReceiver(args.getString(0), Long.parseLong(args.getString(1)));
            } else if (IActionReceiver.ACTION_LAUNCH_APP.equals(action)) {
                iActionReceiver = new LaunchReceiver(args.getString(0), args.getString(1), args.getString(2));
            } else if (IActionReceiver.ACTION_STOP_APP.equals(action)) {
                iActionReceiver = new StopAppReceiver(args.getString(0), args.getString(1));
            } else if (IActionReceiver.ACTION_GET_APP_STATUS.equals(action)) {
                iActionReceiver = new StatusReceiver(args.getString(0), args.getString(1));
            } else if (IActionReceiver.ACTION_HIDE_APP.equals(action)) {
                iActionReceiver = new HideReceiver(args.getString(0), args.getString(1));
            }
            return ActionExecutor.getInstances().setExecutor(iActionReceiver);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }

    }

    @Override
    protected void onPostExecute(Util.Result result) {
        super.onPostExecute(result);

        if (result == null) {
            sendResult(callbackContext, PluginResult.Status.ERROR, "{\"code\":0,\"error\":\"result is null\",\"data\":{}}");
            return;
        }

        if (result.code != 0 && result.code != -1001) {
            sendResult(callbackContext, PluginResult.Status.ERROR, result.jsonObj);
            return;
        }

        if (result.code == -1001) {
            dlnaPlugin.callbackContext = callbackContext;
            sendResult(dlnaPlugin.callbackContext, PluginResult.Status.OK, result.error);
            return;
        }
        Log.e("TAG", "result.toString(): " + result.jsonObj.toString());
        sendResult(callbackContext, PluginResult.Status.OK, result.jsonObj);
    }



    private void sendResult(CallbackContext callbackContext, PluginResult.Status status, String msg) {
        PluginResult pluginResult = new PluginResult(status, msg);
        pluginResult.setKeepCallback(true);
        callbackContext.sendPluginResult(pluginResult);
    }

    private void sendResult(CallbackContext callbackContext, PluginResult.Status status, JSONObject msg) {
        PluginResult pluginResult = new PluginResult(status, msg);
        pluginResult.setKeepCallback(true);
        callbackContext.sendPluginResult(pluginResult);
    }

}
