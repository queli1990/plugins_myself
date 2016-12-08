package com.vego.tv.plugin;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ProviderInfo;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import com.gochinatv.Discovery;
import com.gochinatv.MediaRenderer;
import com.gochinatv.Util;
import com.vego.tv.ActionExecutor;
import com.vego.tv.IActionReceiver;
import com.vego.tv.InitReceiver;
import com.vego.tv.ScanReciver;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;

import rx.Observable;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

/**
 * Created by fq_mbp on 2016/11/16.
 */

public class MediaRendererPlugin extends CordovaPlugin implements Discovery.Delegate {

    private static final String TAG = "Toast";
    private CallbackContext callbackContext;
    private boolean isInit;
    private MyHandler handler = new MyHandler();

    @Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        super.initialize(cordova, webView);
        Context context = this.cordova.getActivity().getApplicationContext();
    }

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        Activity activity = cordova.getActivity();
        Toast.makeText(activity, "action..." + action, Toast.LENGTH_SHORT).show();
        Log.e("TAG", "action......" + action);
        





//        if ("init".equals(action)) {
//            // 只初始化一次
//            if (isInit)
//                return true;
//            else
//                isInit = true;
//            init();
//        } else if ("scan".equals(action)) {
//            Log.e("TAG", "正在扫描......");
//            scan(callbackContext);
//        } else if ("stopScan".equals(action)) {
//            Log.e("TAG", "停止扫描......");
//            stopScan();
//        } else if ("set".equals(action)) {
//            handler.removeMessages(0);
//            handler.setVideoCallbackContext(callbackContext);
//            Log.e("TAG", "正在准备播放......" + args.toString());
//            String hostName = args.getString(0);
//            handler.setHostName(hostName);
//            String url = args.getString(1);
//            setAVTransportURI(hostName, url);
//        } else if ("play".equals(action)) {
//            String hostName = args.getString(0);
//            Log.e("TAG", "播放......");
//            play(hostName);
//            handler.sendEmptyMessage(0);
//        } else if ("pause".equals(action)) {
//            Log.e("TAG", "暂停......");
//            String hostName = args.getString(0);
//            pause(hostName);
//            handler.removeMessages(0);
//        } else if ("volume".equals(action)) {
//            Log.e("TAG", "volume......");
//            String hostName = args.getString(0);
//            int volume = args.getInt(1);
//            setVolume(hostName, volume);
//        } else if ("getSysVolume".equals(action)) {
//            String hostName = args.getString(0);
//            Log.e("TAG", "获取系统音量......" + hostName);
//            Util.Result result = null;
//            try {
//                result = MediaRenderer.getVolume(hostName);
//                if (result != null && result.data != null) {
//                    int currentVolume = result.data.getInt("CurrentVolume");
//                    Log.e("TAG", "当前音量......" + currentVolume);
//                    callbackContext.success(result.toString());
//                }else {
//                    if(result != null){
//                        callbackContext.error(result.toString());
//                    }else{
//                        callbackContext.error("{ error: \"result or result.data is null\"}");
//                    }
//                }
//            } catch (Exception e) {
//                e.printStackTrace();
//                if(result != null){
//                    callbackContext.error(result.toString());
//                }else {
//                    callbackContext.error("{ error:" + e.getMessage() + "}");
//                }
//            }
//
//
//            Util.Result res2 = MediaRenderer.getPositionInfo(hostName);
//            Log.e("TAG", "getPositionInfo: " + res2.code);
//
//            if (res2.code == 0 && res2.data != null) {
//                String data = "";
//                try {
//                    if (res2.data.getInt("TrackDuration") == 0) {
//                        return true;
//                    }
//                    data += String.format("##MediaDuration: %s, RelTime: %d \n##TrackURI: %s",
//                            // es.data.getInt("MediaDuration"),
//                            res2.data.getInt("TrackDuration"),
//                            res2.data.getInt("RelTime"),
//                            res2.data.getString("TrackURI")
//                    );
//                    Log.e("TAG", "getPositionInfo: " + data);
//                    // 设置结束时间
//                    int duration = res2.data.getInt("TrackDuration");
////                    String durationFormat = Util.secToTime(duration);
//                    // 开始进度条
//                    int currentTime = res2.data.getInt("RelTime");
////                    String currentTimeFormat = Util.secToTime(currentTime);
//
//
//
//                } catch (JSONException e) {
//                    e.printStackTrace();
//                }
//            } else {
//                Log.e("TAG", " 链接失败。。。。。。");
//            }
//        } else if ("stop".equals(action)) {
//            String hostName = args.getString(0);
//            Log.e("TAG", "停止投屏......" + hostName);
//            stopVideo(hostName);
//            handler.removeMessages(0);
//        } else if ("seek".equals(action)) {
//            String hostName = args.getString(0);
//            int seekTo = args.getInt(1);
//            Log.e("TAG", "快进快退......" + hostName);
//            seekTo(hostName, seekTo);
//        }
        executeAction(action, callbackContext);
        return true;
    }

    private void executeAction(final String action, final CallbackContext callbackContext){
        Observable.just(action)
                .map(new Func1<String, Util.Result>() {
                    @Override
                    public Util.Result call(String s) {
                        IActionReceiver iActionReceiver = null;
                        if(IActionReceiver.ACTION_INIT.equals(action)){
                            iActionReceiver = new InitReceiver(MediaRendererPlugin.this);
                        } else if (IActionReceiver.ACTION_SCAN.equals(action)){
                            iActionReceiver = new ScanReciver();
                        }
                        return ActionExecutor.getInstances().setExecutor(iActionReceiver);
                    }
                }).subscribeOn(Schedulers.newThread())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<Util.Result>() {
                    @Override
                    public void call(Util.Result result) {
                        if(result == null){
                            sendResult(callbackContext, PluginResult.Status.ERROR, "{ error: \"result is null\" }");
                            return;
                        }

                        if(result.code != 0 && result.code != -1001){
                            sendResult(callbackContext, PluginResult.Status.ERROR, result.toString());
                            return;
                        }
                        
                        if(result.code == -1001){
                            MediaRendererPlugin.this.callbackContext = callbackContext;
                            return;
                        }

                        sendResult(callbackContext, PluginResult.Status.OK, result.toString());

                    }
                }, new Action1<Throwable>() {
                    @Override
                    public void call(Throwable throwable) {
                        sendResult(callbackContext, PluginResult.Status.ERROR, "{ error:\""+ throwable.toString() +"\" }");
                    }
                });


//        Observable.create(new Observable.OnSubscribe<String>() {
//            @Override
//            public void call(Subscriber<? super String> subscriber) {
//
//                subscriber.onNext("s");
//                subscriber.onCompleted();
//            }
//        }).subscribeOn(Schedulers.newThread())
//                .observeOn(AndroidSchedulers.mainThread())
//                .subscribe(new Action1<String>() {
//                    @Override
//                    public void call(String s) {
//
//                    }
//                }, new Action1<Throwable>() {
//                    @Override
//                    public void call(Throwable throwable) {
//
//                    }
//                });
    }

    private void sendResult(CallbackContext callbackContext, PluginResult.Status status, String msg){
        PluginResult pluginResult = new PluginResult(status, msg);
        pluginResult.setKeepCallback(true);
        callbackContext.sendPluginResult(pluginResult);
    }


    public void init() {
        Discovery.setDelegate(this);
    }


    /**
     * mediaRenderer方式扫描媒体
     */
    public void scan(CallbackContext callbackContext) {
        this.callbackContext = callbackContext;
        Discovery.scan("MEDIA_RENDERER");
    }

    private void stopScan() {
        Discovery.stop();
    }

    public void setAVTransportURI(String hostName, String url) {
//        Util.Result result = MediaRenderer.setAVTransportURI(hostName,
//                        "http://tuxiaobei.cdn.ottcloud.tv/tuxiaobei/videos/dst/2016/11/100160-430-xiao-xue-hua-zhao-ma-mɑ/100160-430-xiao-xue-hua-zhao-ma-mɑ.m3u8");
        Util.Result result = MediaRenderer.setAVTransportURI(hostName,
                url);
        if(result != null){
            Log.e("TAG", "result: " + result.code);
            Log.e("TAG", "result: " + result.data);
        }
        Log.e("TAG", "hostName: " + hostName);
//        handler.sendEmptyMessageDelayed(0, 5000);
    }

    public void play(String hostName) {
        MediaRenderer.play(hostName);
    }

    public void pause(String hostName) {
        MediaRenderer.pause(hostName);
    }

    public void stopVideo(String hostName) {
        MediaRenderer.stop(hostName);
    }

    public void setVolume(String hostName, int volume) {
        MediaRenderer.setVolume(hostName, volume);
    }

    public void seekTo(String hostName, int seekTo) {
        MediaRenderer.seek(hostName, seekTo);
    }
    

    @Override
    public void discovery_change() {
        JSONArray services = Discovery.service("MEDIA_RENDERER");
        Log.e("TAG", "discovery_change......" + services.toString());
        // 此处得到json字段，需要将内容返回给js
    }

    @Override
    public void discovery_error(String err) {
        Log.e("TAG", "discovery_error......");
    }


    private static class MyHandler extends Handler {

        private TextView tvTotalTime;
        private SeekBar mediaSeekBar;
        private String hostName;
        private CallbackContext videoCallbackContext;

        public void setTotalTime(TextView textView) {
            tvTotalTime = textView;
        }

        public void setMediaSeekBar(SeekBar mediaSeekBar) {
            this.mediaSeekBar = mediaSeekBar;
        }

        public void setHostName(String hostName) {
            this.hostName = hostName;
        }

        public void setVideoCallbackContext(CallbackContext videoCallbackContext) {
            this.videoCallbackContext = videoCallbackContext;
        }

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            int what = msg.what;
            switch (what) {
                case 0:
                    Util.Result res2 = MediaRenderer.getPositionInfo(hostName);
                    Log.e("TAG", "getPositionInfo: " + res2.code);

                    if (res2.code == 0 && res2.data != null) {
                        String data = "";
                        try {
                            if (res2.data.getInt("TrackDuration") == 0) {
                                sendEmptyMessageDelayed(0, 1000);
                                return;
                            }
                            data += String.format("##MediaDuration: %s, RelTime: %d \n##TrackURI: %s",
                                    // es.data.getInt("MediaDuration"),
                                    res2.data.getInt("TrackDuration"),
                                    res2.data.getInt("RelTime"),
                                    res2.data.getString("TrackURI")
                            );
                            Log.e("TAG", "getPositionInfo: " + data);
                            // 设置结束时间
                            int duration = res2.data.getInt("TrackDuration");
//                            String durationFormat = Util.secToTime(duration);
                            // 开始进度条
                            int currentTime = res2.data.getInt("RelTime");
//                            String currentTimeFormat = Util.secToTime(currentTime);

                            if (videoCallbackContext != null) {
                                videoCallbackContext.success("currentTimeFormat;durationFormat");
                            }

                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    } else {
                        Log.e("TAG", " 链接失败。。。。。。");
                    }
                    sendEmptyMessageDelayed(0, 1000);
                    break;
                case 1:

                    break;
            }
        }
    }

}
