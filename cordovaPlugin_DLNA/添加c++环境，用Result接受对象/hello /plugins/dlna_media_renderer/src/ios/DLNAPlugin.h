//
//  DialPlugin.h
//  Dial_cordova
//
//  Created by Holy on 16/11/23.
//
//

#import <Cordova/CDV.h>
#import "discovery.h"
#import "media-renderer.h"
#import <Foundation/Foundation.h>

@interface DLNAPlugin : CDVPlugin

@property (nonatomic,copy) NSString *callback_scan_id;

- (void) init:(CDVInvokedUrlCommand *)command;
- (void) scan:(CDVInvokedUrlCommand *)command;
- (void) stopScan:(CDVInvokedUrlCommand *)command;
- (void) mediarenderer_start:(CDVInvokedUrlCommand *)command;
- (void) mediarenderer_play:(CDVInvokedUrlCommand *)command;
- (void) mediarenderer_pause:(CDVInvokedUrlCommand *)command;
- (void) mediarenderer_stop:(CDVInvokedUrlCommand *)command;
- (void) mediarenderer_seek:(CDVInvokedUrlCommand *)command;

//以下功能需要先获取系统的音量
- (void) mediarenderer_getSysVolume:(CDVInvokedUrlCommand *)command;
- (void) mediarenderer_getVideoInfo:(CDVInvokedUrlCommand *)command;
- (void) mediarenderer_volume:(CDVInvokedUrlCommand *)command;

- (void) test:(CDVInvokedUrlCommand *)command;

- (void) dial_launch:(CDVInvokedUrlCommand *)command;
- (void) dial_hide:(CDVInvokedUrlCommand *)command;
- (void) dial_stop:(CDVInvokedUrlCommand *)command;
- (void) dial_status:(CDVInvokedUrlCommand *)command;


@end
