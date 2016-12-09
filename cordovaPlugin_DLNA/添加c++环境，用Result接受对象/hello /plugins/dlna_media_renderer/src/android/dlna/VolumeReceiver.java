package com.vego.tv.plugin.dlna;

import com.gochinatv.MediaRenderer;
import com.gochinatv.Util;

/**
 * Created by fq_mbp on 2016/12/7.
 */

public class VolumeReceiver implements IActionReceiver {

    private String hostName;
    private int volume;
    public VolumeReceiver(String hostName, int volume) {
        this.hostName = hostName;
        this.volume = volume;
    }

    @Override
    public Util.Result execute() {
        return MediaRenderer.setVolume(hostName, volume);
    }
}
