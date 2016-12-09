package com.vego.tv.plugin.dlna;

import com.gochinatv.MediaRenderer;
import com.gochinatv.Util;

/**
 * Created by fq_mbp on 2016/12/7.
 */

public class StartReceiver implements IActionReceiver {

    private String url;
    private String hostName;
    public StartReceiver(String hostName, String url) {
        this.hostName = hostName;
        this.url = url;
    }

    @Override
    public Util.Result execute() {
        return MediaRenderer.setAVTransportURI(hostName, url);
    }
}
