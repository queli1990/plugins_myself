cordova-plugin-dial
===============

dial plugin for cordova

***************************
        Android 
***************************


安装： cordova plugin add http://git.vego.tv/gitbucket/git/wukong/cordova-plugin-dial.git

使用：
    
    1.初始化方法，必须首先执行
    mediaRenderer.init();
    
    2.启动扫描
    mediaRenderer.scan(function(msg){
                // 成功返回jsonArray
                var scanObj = eval("(" + msg + ")");
            }, function(error){
                alert("失败消息：" + error);
        });
        
    3.取消扫描
    mediaRenderer.stopScan();
    
    4.开始播放视频 *注意*:此方法必须是在scan之后才起作用，使用scan内的数据才能正常使用
    参数: 
        hostNameIp: scan返回数据中的hostname
        url: 视频地址
    mediaRenderer.set(hostNameIp, "http://tuxiaobei.cdn.ottcloud.tv/tuxiaobei/videos/dst/2016/11/100160-430-xiao-xue-hua-zhao-ma-mɑ/100160-430-xiao-xue-hua-zhao-ma-mɑ.m3u8");
    
    5.play/pause  *注意*:此方法必须是在set之后才起作用
    参数: 
        hostNameIp: scan返回数据中的hostname
    mediaRenderer.play(hostNameIp); 
    mediaRenderer.pause(hostNameIp);
    
    6.调节音量
    参数: 
        hostNameIp: scan返回数据中的hostname
        volume: 整形值
    mediaRenderer.volume(hostNameIp, volume);
    
    7.获取当前系统的音量，可能会出现获取不到的情况，需要设置默认值 
    mediaRenderer.getSysVolume(hostNameIp, function(msg){
                // 返回值为0-100的整形
                sysVolume = msg;
            }, function(error){
                alert("失败消息：" + error);
        });
        
    8.停止播放方法
    mediaRenderer.stop(hostNameIp);
