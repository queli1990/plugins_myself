//
//  NetStatus.h
//  wechatLogin
//
//  Created by Holy on 16/12/27.
//
//

#import <Cordova/CDV.h>

@interface NetStatusPlugin : CDVPlugin

- (void)registerNetStatus:(CDVInvokedUrlCommand *)command;

- (void)getNetStatus:(CDVInvokedUrlCommand *)command;

@end
