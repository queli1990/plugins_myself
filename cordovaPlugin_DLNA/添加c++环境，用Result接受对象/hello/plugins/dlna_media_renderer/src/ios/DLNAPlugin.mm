//
//  DialPlugin.m
//  Dial_cordova
//
//  Created by Holy on 16/11/23.
//
//

#import "DLNAPlugin.h"
#include <dial.h>

using namespace dlna;

typedef Service::Result Result;

class MyDiscovery;

@interface DLNAPlugin () {
@private
    bool m_is_scan;
    MyDiscovery * m_discovery;
    int m_volume;
}


@end

class MyDiscovery:public Discovery, public Discovery::Delegate{
public:
    MyDiscovery(DLNAPlugin *ctr): m_ctr(ctr) {
        set_delegate(this);
    }
    
    virtual void discovery_change(Discovery * dis) {
        vector<Service *> ls = this -> all_service();
        
        NSString *str = [NSString string];
        NSMutableArray *arr = [NSMutableArray array];
        
        for (int i = 0; i<ls.size(); i++) {
            NSString *scanType = [NSString string];
            NSDictionary *dic = [NSDictionary dictionary];
            
            if (ls[i]->as_dial()) {
                scanType = @"DIAL";
            }else{
                scanType = @"MEDIA_RENDERER";
            }
            
            NSString *friendly_name = [NSString stringWithUTF8String:ls[i]->get_friendly_name().c_str()];
            NSString *hostName = [NSString stringWithFormat:@"%s",ls[i]->get_hostname().c_str()];
            NSString *location = [NSString stringWithFormat:@"%s",ls[i]->get_location().c_str()];
            NSString *port = [NSString stringWithFormat:@"%d",ls[i]->get_port()];
            NSString *uuid = [NSString stringWithFormat:@"%s",ls[i]->get_uuid().c_str()];
            BOOL isInvalid = ls[i]->is_invalid();
            NSString *type = scanType;
            
            dic = @{@"hostname":hostName,@"port":port,@"location":location,@"friendly_name":friendly_name,@"uuid":uuid,@"type":type,@"is_invalid":@(isInvalid)};
            [arr addObject:dic];
            
            //            str = [str stringByAppendingFormat:
            //                   @"friendly_name:%@, host:%s, port:%d\n",
            //                   [NSString stringWithUTF8String:ls[i]->get_friendly_name().c_str()],
            //                   ls[i]->get_hostname().c_str(),
            //                   ls[i]->get_port()
            //                   ];
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            
            CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsArray:arr ];
            [result setKeepCallbackAsBool:YES];
            [m_ctr.commandDelegate sendPluginResult:result callbackId:m_ctr.callback_scan_id];
        });
    }
    
    virtual void discovery_error(Discovery * dis, const string&err){
        
        NSString *errStr = [NSString stringWithFormat:@"%s",err.c_str()];
        NSData *jsonData = [errStr dataUsingEncoding:NSUTF8StringEncoding];
        NSError *error;
        NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&error];
        CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
        [result setKeepCallbackAsBool:YES];
        [m_ctr.commandDelegate sendPluginResult:result callbackId:m_ctr.callback_scan_id];
    }
    
private:
    DLNAPlugin * m_ctr;
};

@implementation DLNAPlugin

- (void) init:(CDVInvokedUrlCommand *)command{
    m_discovery = new MyDiscovery(self);
    NSLog(@"native ----- init methodĂÂĂÂÄÂĂÂ");
}

- (void) scan:(CDVInvokedUrlCommand *)command {
    if (!self.callback_scan_id) {
        self.callback_scan_id = command.callbackId;
    }
    if (!m_discovery) {
        m_discovery = new MyDiscovery(self);
    }
    NSLog(@"native ----- scan methodĂÂĂÂÄÂĂÂ");
    m_discovery -> scan(Discovery::ALL);
}

- (void) stopScan:(CDVInvokedUrlCommand *)command{
    m_discovery ->stop();
    NSLog(@"native ----- stop methodĂÂĂÂÄÂĂÂ");
}

- (void) mediarenderer_start:(CDVInvokedUrlCommand *)command {
    NSLog(@"native ----- start methodĂÂĂÂÄÂĂÂ");
    
    NSArray *arr = command.arguments[0];
    NSString *hostName = arr[0];
    NSString *playUrl = arr[1];
    
    vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
    for (int i = 0; i < ls.size(); i++) {
        MediaRenderer* mr = ls[i]->as_media_renderer();
        if ( mr ) {
            const char* name = mr->get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            NSLog(@"%s",name);
            if ([hostName isEqualToString:scanName]) {
                const char *url = [playUrl cStringUsingEncoding:NSASCIIStringEncoding];
                
                Result re = mr ->set_av_transport_uri(url);
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                NSLog(@"string:%@",string);
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic ];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
            
            //            http://huace.cdn.ottcloud.tv/huace/videos/dst/2016/08/14461-ai-de-zhui-zong-01-ji_201608180551/14461-ai-de-zhui-zong-01-ji.m3u8
            
            //            https://manifest.googlevideo.com/api/manifest/hls_variant/upn/d-WyZxATzEU/signature/6978A1F587E4EFD0219C233BE8683FFBF82BF819.8D2939A83935128E63630E194D5C175D5B5D2552/sparams/ei%2Cgcr%2Cgo%2Chfr%2Cid%2Cip%2Cipbits%2Citag%2Cmaudio%2Cplaylist_type%2Cratebypass%2Crequiressl%2Csource%2Cexpire/id/zUnjf_W3GWQ.0/go/1/gcr/us/ratebypass/yes/playlist_type/DVR/ipbits/0/ei/TUokWJHwMMLH8gSa_5aQCQ/requiressl/yes/ip/45.55.84.189/key/yt6/maudio/1/keepalive/yes/itag/0/source/yt_live_broadcast/dover/6/hfr/1/expire/1478794925/file/index.m3u8
        }
    }
}

#define CALL(exec) \
if (!m_discovery) { \
m_discovery = new MyDiscovery(self); \
} \
vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER ); \
for (int i = 0; i < ls.size(); i++) { \
MediaRenderer* mr = ls[i]->as_media_renderer(); \
if ( mr ) { \
exec; \
} \
}

- (void) mediarenderer_play:(CDVInvokedUrlCommand *)command{
    NSString *hostName = command.arguments[0];
    
    vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
    for (int i = 0; i < ls.size(); i++) {
        MediaRenderer* mr = ls[i]->as_media_renderer();
        if ( mr ) {
            const char* name = mr->get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            NSLog(@"%s",name);
            if ([hostName isEqualToString:scanName]) {
                
                Result re = mr ->play();
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
                
            }
        }
    }
}

- (void) mediarenderer_pause:(CDVInvokedUrlCommand *)command{
    NSString *hostName = command.arguments[0];
    
    vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
    for (int i = 0; i < ls.size(); i++) {
        MediaRenderer* mr = ls[i]->as_media_renderer();
        if ( mr ) {
            const char* name = mr->get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            NSLog(@"%s",name);
            if ([hostName isEqualToString:scanName]) {
                
                Result re = mr ->pause();
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
        }
    }
}

- (void) mediarenderer_seek:(CDVInvokedUrlCommand *)command{
    NSArray *arr  = command.arguments[0];
    NSString *hostName = arr[0];
    NSString *secondStr = arr[1];
    long second = secondStr.longLongValue;
    
    vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
    for (int i = 0; i < ls.size(); i++) {
        MediaRenderer* mr = ls[i]->as_media_renderer();
        if ( mr ) {
            const char* name = mr->get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            NSLog(@"%s",name);
            if ([hostName isEqualToString:scanName]) {
                
                Result re = mr ->seek(second);
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
                
            }
        }
    }
}

- (void) mediarenderer_stop:(CDVInvokedUrlCommand *)command{
    NSString *hostName = command.arguments[0];
    
    vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
    for (int i = 0; i < ls.size(); i++) {
        MediaRenderer* mr = ls[i]->as_media_renderer();
        if ( mr ) {
            const char* name = mr->get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            NSLog(@"%s",name);
            if ([hostName isEqualToString:scanName]) {
                
                Result re = mr ->stop();
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
        }
    }
}

- (void) mediarenderer_volume:(CDVInvokedUrlCommand *)command{
    NSArray *arr = command.arguments[0];
    NSString *hostName = arr[0];
    NSString *volumeStr = arr[1];
    int volume = volumeStr.intValue;
    
    vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
    for (int i = 0; i < ls.size(); i++) {
        MediaRenderer* mr = ls[i]->as_media_renderer();
        if ( mr ) {
            const char* name = mr->get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            NSLog(@"%s",name);
            if ([hostName isEqualToString:scanName]) {
                
                Result re = mr ->set_volume(volume);
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
        }
    }
}

- (void) mediarenderer_getVideoInfo:(CDVInvokedUrlCommand *)command{
    NSString *hostName = command.arguments[0];
    
    vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
    for (int i = 0; i < ls.size(); i++) {
        MediaRenderer* mr = ls[i]->as_media_renderer();
        if ( mr ) {
            const char* name = mr->get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            NSLog(@"%s",name);
            if ([hostName isEqualToString:scanName]) {
                
                Result re = mr -> get_position_info();
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
        }
    }
}

- (void) mediarenderer_getSysVolume:(CDVInvokedUrlCommand *)command{
    NSString *hostName = command.arguments[0];
    
    vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
    for (int i = 0; i < ls.size(); i++) {
        MediaRenderer* mr = ls[i]->as_media_renderer();
        if ( mr ) {
            const char* name = mr->get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            NSLog(@"%s",name);
            if ([hostName isEqualToString:scanName]) {
                
                Result re = mr -> get_volume();
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
        }
    }
}

- (void) dial_launch:(CDVInvokedUrlCommand *)command{
    NSArray *arr = command.arguments[0];
    NSString *hostName = arr[0];
    NSString *appName = arr[1];
    NSString *param = arr[2];
    vector<Service*> ls = m_discovery ->all_service();
    for (int i = 0; i<ls.size(); i++) {
        Dial *dial = ls[i] -> as_dial();
        if (dial) {
            const char *name = dial -> get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            if ([hostName isEqualToString:scanName]) {
                
                const char *appnameC = [appName cStringUsingEncoding:NSASCIIStringEncoding];
                const char *paramC = [param cStringUsingEncoding:NSASCIIStringEncoding];
                Result re = dial ->launch(appnameC,paramC);
                
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
        }
    }
}

- (void) dial_hide:(CDVInvokedUrlCommand *)command{
    NSLog(@"did not realize");
}

- (void) dial_stop:(CDVInvokedUrlCommand *)command{
    NSArray *arr = command.arguments[0];
    NSString *hostName = arr[0];
    NSString *appName = arr[1];
    
    vector<Service*> ls = m_discovery ->all_service();
    for (int i = 0; i<ls.size(); i++) {
        Dial *dial = ls[i] -> as_dial();
        if (dial) {
            const char *name = dial -> get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            if ([hostName isEqualToString:scanName]) {
                
                const char *appnameC = [appName cStringUsingEncoding:NSASCIIStringEncoding];
                Result re = dial ->stop(appnameC);
                
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
        }
    }
}

- (void) dial_status:(CDVInvokedUrlCommand *)command{
    NSArray *arr = command.arguments[0];
    NSString *hostName = arr[0];
    NSString *appName = arr[1];
    
    vector<Service*> ls = m_discovery ->all_service();
    for (int i = 0; i<ls.size(); i++) {
        Dial *dial = ls[i] -> as_dial();
        if (dial) {
            const char *name = dial -> get_hostname().c_str();
            NSString *scanName = [NSString stringWithFormat:@"%s",name];
            if ([hostName isEqualToString:scanName]) {
                
                const char *appnameC = [appName cStringUsingEncoding:NSASCIIStringEncoding];
                Result re = dial ->status(appnameC);
                
                string code = re.to_json_string().c_str();
                NSString *string  = [NSString stringWithFormat:@"%s",code.c_str()];
                
                NSData *jsonData = [string dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
                
                if (re.code == 0) {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                } else {
                    CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:dic];
                    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                }
            }
        }
    }
}


- (void) test:(CDVInvokedUrlCommand *)command {
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"ĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂÄÂĂÂÄÂĂÂ§ĂÂĂÂÄÂĂÂ¤ÄÂÄšÄÄÂĂÂ" message:@"124" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    [alert show];
    
    
    NSDictionary * argums = command.arguments[0];
    
    //ĂÂĂÂÄÂĂÂÄÂÄšÄÄÂĂÂĂÂĂÂÄÂĂÂÄÂĂÂÄÂĂÂ¤ÄÂÄšÄĂĹĄĂÂĂÂĂÂÄÂĂÂ ÄÂĂÂĂĹĄĂÂ ĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂÄÂĂÂÄÂĂÂ§ĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂĂĹĄĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂ°
    NSDictionary * ret = @{@"success":@"1",@"message":@"ĂÂĂÂĂĹĄĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂ°ÄÂĂÂÄÂĂÂ¤ĂÂĂÂÄÂĂÂ¸ĂÂĂÂÄÂĂÂÄÂĂÂÄÂĂÂ¤ĂÂĂÂÄÂĂÂ¸ÄÂÄšÄÄÂĂÂÄÂĂÂÄÂĂÂ§ÄÂÄšÄÄÂĂÂ ÄÂÄšÄÄÂĂÂ!",@"code":@(200),@"data":@{}};
    
    CDVPluginResult* result = [CDVPluginResult resultWithStatus: CDVCommandStatus_OK messageAsDictionary:ret];
    //ĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂ ÄÂÄšÄÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂÄÂÄšÄÄÂĂÂcallbackIdĂÂĂÂĂĹĄĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂ°ĂÂĂÂÄÂĂÂÄÂĂÂÄÂĂÂ§ĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂidĂÂĂÂÄÂĂÂÄÂÄšÄĂĹĄÄšÂĂÂĂÂÄÂĂÂĂÂĂÂĂĹĄĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂĂÂĂÂĂĹĄĂÂĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂÄÂĂÂÄÂĂÂ§ĂÂĂÂÄÂĂÂĂÂĂÂÄÂĂÂÄÂĂÂÄÂĂÂ¤ÄÂÄšÄÄÂĂÂÄÂÄšÄÄÂĂÂÄÂĂÂÄÂĂÂ§ĂÂĂÂÄÂĂÂ ĂÂĂÂÄÂĂÂ
    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
    
    
    //    if (command.arguments[0] != nil) {
    //        CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:@"oh yeah"];
    //        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
    //    }else {
    //        CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"oh no"];
    //        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
    //    }
    
}


@end
