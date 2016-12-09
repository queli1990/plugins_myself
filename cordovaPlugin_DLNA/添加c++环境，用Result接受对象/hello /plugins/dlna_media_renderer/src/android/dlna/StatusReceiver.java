package com.vego.tv.plugin.dlna;

import com.gochinatv.Dial;
import com.gochinatv.Util;

/**
 * Created by fq_mbp on 2016/12/8.
 */

public class StatusReceiver implements IActionReceiver {

    private String hostName;
    private String appName;


    public StatusReceiver(String hostName, String appName) {
        this.hostName = hostName;
        this.appName = appName;
    }

    @Override
    public Util.Result execute() {
        return Dial.status(hostName, appName);
    }
}
