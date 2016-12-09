package com.vego.tv.plugin.dlna;

import com.gochinatv.MediaRenderer;
import com.gochinatv.Util;

/**
 * Created by fq_mbp on 2016/12/7.
 */

public class SeekReceiver implements IActionReceiver {

    private String hostName;
    private long seekTo;
    public SeekReceiver(String hostName, long seekTo) {
        this.hostName = hostName;
        this.seekTo = seekTo;
    }

    @Override
    public Util.Result execute() {
        return MediaRenderer.seek(hostName, seekTo);
    }
}
