package com.vego.tv.plugin.dlna;

import com.gochinatv.Discovery;
import com.gochinatv.Util;

/**
 * Created by fq_mbp on 2016/12/7.
 */

public class StopScanReceiver implements IActionReceiver {

    @Override
    public Util.Result execute() {
        Discovery.stop();
        return null;
    }
}
