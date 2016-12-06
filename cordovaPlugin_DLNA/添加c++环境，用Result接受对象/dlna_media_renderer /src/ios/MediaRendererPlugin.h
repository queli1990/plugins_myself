//
//  DialPlugin.h
//  Dial_cordova
//
//  Created by Holy on 16/11/23.
//
//

#import <Cordova/CDV.h>
#import <discovery.h>
#import <media-renderer.h>
#import <Foundation/Foundation.h>

@interface MediaRendererPlugin : CDVPlugin

@property (nonatomic,copy) NSString *callback_scan_id;

- (void) mediarender_init:(CDVInvokedUrlCommand *)command;
- (void) mediarender_scan:(CDVInvokedUrlCommand *)command;
- (void) mediarender_stopScan:(CDVInvokedUrlCommand *)command;
- (void) mediarender_start:(CDVInvokedUrlCommand *)command;
- (void) mediarender_play:(CDVInvokedUrlCommand *)command;
- (void) mediarender_pause:(CDVInvokedUrlCommand *)command;
- (void) mediarender_stop:(CDVInvokedUrlCommand *)command;
- (void) mediarender_seek:(CDVInvokedUrlCommand *)command;

//以下功能需要先获取系统的音量
- (void) mediarender_setVolume:(CDVInvokedUrlCommand *)command;
- (void) mediarender_getInfo:(CDVInvokedUrlCommand *)command;
- (void) mediarender_getSytVolume:(CDVInvokedUrlCommand *)command;

- (void) test:(CDVInvokedUrlCommand *)command;



@end
