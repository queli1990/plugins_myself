cordova-plugin-dial
===============

dial plugin for cordova


安装： cordova plugin add http://git.vego.tv/gitbucket/git/wukong/cordova-plugin-dial.git

使用:
    
*******************************************
				 Android
*******************************************

Js对象为：dlna
1.初始化方法，启动DLNA，必须优先初始化
		
		dlna.init();
		参数：

2.扫描方法

	dlna.scan(function(jsonArray){
	            
		}, function(errJsonObj){

    });
	参数：success成功回调，如果扫描成功，会通过succss的callback返回JsonArray的对象，里面包含设备信息
		 注意：每当发现新设备时，都会有新数据集合，包含之前发现的设备数据，所以注意出现重复数据
		JSONArray的格式
		[
			{
				"hostname":"10.2.0.123", 	// 主机域名，关键字段，后续的方法基本上都会用到
				"port":2869,	
				"location":"http:\/\/10.2.0.123:2869\/upnphost\/udhisapi.dll?content=uuid:72542333-6d3c-4356-8456-33119f27fb07",
				"friendly_name":"MR_CLOUD",		// 设备的名称，可能会为""
				"uuid":"uuid:72542333-6d3c-4356-8456-33119f27fb07",
				"type":"MEDIA_RENDERER",     // 设备投屏支持的协议，目前为MEDIA_RENDERER或DIAL两种方式
				"is_invalid":false	// 当前设备是否无用，可用：false，反之
			}
		]
		fail失败回调，如果扫描失败，会返回包含error错误信息的JSONObject对象

3.停止扫描方法， 建议当退出DLNA功能的时候，就调用，否则会一直接受消息，后台也会一直执行
	
	dlna.stopScan();
	参数：

**************************************************************************************
				 注意：根据scan方法返回的设备信息中的type，分成2种执行方式
**************************************************************************************

MEDIA_RENDERER 投屏方式：
	
	1.开始投屏视频 注意：该方法只能在扫描到可用设备后，使用扫描到的设备的hostname，才能起作用
		
		dlna.start(hostName, url, function(successJsonObj){
	                
	        }, function(errJsonObj){

	    });

	    参数：hostName: String 通过scan方法扫描的设备的hostname字段
	    	 url: String 投屏的视频地址
	    	 success回调，由于底层方法是异步，所以该方法返回可能会有延迟，返回执行结果
	    	 fail回调，由于执行失败返回的callback
	    	 一般返回的数据形势如下:
	    	 	{"code":0,"error":"","data":{}}
	    	 	data的信息根据执行操作而变动

	2.停止投屏功能
		
		dlna.stop(hostNameIp, function(successJsonObj){

	      	}, function(errJsonObj){
	    });

	    参数：参照4

	3.暂停功能
		
		dlna.pause(hostNameIp, function(successJsonObj){

	      	}, function(errJsonObj){
	    });

	    参数：参照4
	4.播放功能  注意：是用于暂停之后的播放
		
		dlna.play(hostNameIp, function(successJsonObj){

	      	}, function(errJsonObj){
	    });

	    参数：参照4

	5.（暂不可用）获取系统当前音量    
		
		dlna.getSysVolume(hostNameIp, function(successJsonObj){

	      	}, function(errJsonObj){
	    });

	    参数：参照4
	    当前的success回调 json格式：{"code":0,"error":"","data":{"CurrentVolume":100}}

	6.设置音量   注意：调用该方法前，建议volume设置预设值
		
		dlna.setVolume(hostNameIp, volume, function(successJsonObj){

	      	}, function(errJsonObj){

	    });

	    参数：参照4
	    	volume String 音量数值范围0-100

	7.获取视频信息方法
		
		dlna.getVideoInfo(hostNameIp, function(successJsonObj){
	            var info = msg;
	            duration = parseInt(info.data.TrackDuration);
	            seekTo = parseInt(info.data.RelTime);
	        }, function(errJsonObj){
	                
	    });
	    参数：参照4
	    	successJsonObj：{"code":0,"error":"","data":{"TrackDuration":274,"RelTime":233}}

	8.seek方法
		
		dlna.seek(hostNameIp, seekTo, function(successJsonObj){

	        }, function(errJsonObj){

	    });
		参数：
			seekTo：String 取值范围从10的返回数据的 0-TrackDuration，是一个long值


DIAL 投屏方式
	1.启动app
		
		dlna.launchApp(hostNameIp, appName, appParams, function(successJsonObj){

            }, function(errJsonObj){

        });
        参数：
       		appName：app在设备中的名称，区分大小写
       		appParams：需要传递的参数
			successJsonObj：{"code":0,"error":"","data":{"header":{"cache-control":"no-cache","content-l	ength":"79","content-type":"text\/plain; charset=\"utf-8\"","location":"http:\/\/192.168.43.244:8060\/dial\/dev\/run","server":"Roku UPnP\/1.0 MiniUPnPd\/1.4"},"body":"additionalDataUrl=http%3A%2F%2F192.168.43.244%3A8060%2Fdial_extra_data%2Fdev%3F","status":201}}

      	注意：该方法在某些设备上（如：ruku）第一次调用启动app后，再次调用无效，app也接受不到参数值

    2.停止app
    	
    	dlna.stopApp(hostNameIp, appName, function(msg){

            }, function(errMsg){

        });
        successJsonObj：{"code":0,"error":"","data":{"header":{"content-length":"0","server":"Roku UPnP\/1.0 MiniUPnPd\/1.4"},"body":"","status":200}}

    3.获取app状态
    	
    	dlna.getAppStatus(hostNameIp, appName, function(msg){

            }, function(errMsg){

        });
        successJsonObj：{"code":0,"error":"","data":{"header":{"cache-control":"no-cache","content-lengt	h":"191","content-type":"text\/xml","server":"Roku UPnP\/1.0 MiniUPnPd\/1.4"},"body":"<?xml version=\"1.0\" encoding=\"UTF-8\" ?><service dialVer=\"1.7\" xmlns=\"urn:dial-multiscreen-org:schemas:dial\">\n\t<name>dev<\/name>\n\t<options allowStop=\"true\"\/>\n\t<state>running<\/state>\n<\/service>\n","status":200}}
