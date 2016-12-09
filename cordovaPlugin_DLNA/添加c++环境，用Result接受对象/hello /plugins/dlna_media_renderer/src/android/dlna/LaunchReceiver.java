package com.vego.tv.plugin.dlna;

import com.gochinatv.Dial;
import com.gochinatv.Util;

/**
 * Created by fq_mbp on 2016/12/8.
 */

public class LaunchReceiver implements IActionReceiver {

    private String hostName;
    private String appName;
    private String params;


    public LaunchReceiver(String hostName, String appName, String params) {
        this.hostName = hostName;
        this.appName = appName;
        this.params = params;
    }

    @Override
    public Util.Result execute() {
        return Dial.launch(hostName, appName, params);
    }
}
