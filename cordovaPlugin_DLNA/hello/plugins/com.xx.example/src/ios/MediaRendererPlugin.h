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

- (void) dial_scan:(CDVInvokedUrlCommand *)command;
- (void) dial_start:(CDVInvokedUrlCommand *)command;
- (void) dial_stop:(CDVInvokedUrlCommand *)command;
- (void) dial_play:(CDVInvokedUrlCommand *)command;
- (void) dial_pause:(CDVInvokedUrlCommand *)command;
- (void) dial_seek:(CDVInvokedUrlCommand *)command;

//一下功能需要先获取系统的音量
- (void) dial_volumeHeigh:(CDVInvokedUrlCommand *)command;
- (void) dial_volumeLow:(CDVInvokedUrlCommand *)command;
- (void) dial_getInfo:(CDVInvokedUrlCommand *)command;


- (void) test:(CDVInvokedUrlCommand *)command;



@end
