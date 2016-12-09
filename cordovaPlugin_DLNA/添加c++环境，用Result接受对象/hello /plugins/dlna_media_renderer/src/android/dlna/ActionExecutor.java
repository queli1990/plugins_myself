package com.vego.tv.plugin.dlna;

import com.gochinatv.Util;


/**
 * Created by fq_mbp on 2016/12/5.
 */

public class ActionExecutor {


    private ActionExecutor(){
    }

    private static class SingletonHolder {
        private static final ActionExecutor INSTANCE = new ActionExecutor();
    }

    public static final ActionExecutor getInstances(){
        return SingletonHolder.INSTANCE;
    }

    public Util.Result setExecutor(IActionReceiver iActionReceiver){
        return iActionReceiver.execute();
    }



}
