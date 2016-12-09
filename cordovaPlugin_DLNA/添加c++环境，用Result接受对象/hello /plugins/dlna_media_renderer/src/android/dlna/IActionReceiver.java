package com.vego.tv.plugin.dlna;

import com.gochinatv.Util;

/**
 * Created by fq_mbp on 2016/12/5.
 */

public interface IActionReceiver {

    String ACTION_INIT = "init";
    String ACTION_SCAN = "scan";
    String ACTION_STOP_SCAN = "stopScan";
    String ACTION_START = "mediarenderer_start";
    String ACTION_PLAY = "mediarenderer_play";
    String ACTION_PAUSE = "mediarenderer_pause";
    String ACTION_SET_VOLUME = "mediarenderer_volume";
    String ACTION_GET_SYS_VOLUME = "mediarenderer_getSysVolume";
    String ACTION_GET_VIDEO_INFO = "mediarenderer_getVideoInfo";
    String ACTION_STOP = "mediarenderer_stop";
    String ACTION_SEEK = "mediarenderer_seek";
    String ACTION_LAUNCH_APP = "dial_launch";
    String ACTION_STOP_APP = "dial_stop";
    String ACTION_HIDE_APP = "dial_hide";
    String ACTION_GET_APP_STATUS = "dial_status";

    Util.Result execute();

}
