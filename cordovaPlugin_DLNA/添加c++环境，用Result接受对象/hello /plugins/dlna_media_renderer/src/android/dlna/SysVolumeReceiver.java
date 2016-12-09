package com.vego.tv.plugin.dlna;

import com.gochinatv.MediaRenderer;
import com.gochinatv.Util;

/**
 * Created by fq_mbp on 2016/12/7.
 */

public class SysVolumeReceiver implements IActionReceiver {

    private String hostName;
    public SysVolumeReceiver(String hostName) {
        this.hostName = hostName;
    }

    @Override
    public Util.Result execute() {
        return MediaRenderer.getVolume(hostName);
    }
}
