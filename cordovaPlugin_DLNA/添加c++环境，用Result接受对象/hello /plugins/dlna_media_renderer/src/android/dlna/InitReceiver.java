package com.vego.tv.plugin.dlna;

import com.gochinatv.Discovery;
import com.gochinatv.Util.*;
import com.vego.tv.plugin.DLNAPlugin;

/**
 * Created by fq_mbp on 2016/12/5.
 */

public class InitReceiver implements IActionReceiver {

    private DLNAPlugin dlnaPlugin;

    public InitReceiver(DLNAPlugin dlnaPlugin) {
        this.dlnaPlugin = dlnaPlugin;
    }

    @Override
    public Result execute() {
        Discovery.setDelegate(dlnaPlugin);
        return null;
    }
}
