//
//  NetStatus.m
//  wechatLogin
//
//  Created by Holy on 16/12/27.
//
//

#import "NetStatusPlugin.h"
#import "AFNetworkReachabilityManager.h"

@implementation NetStatusPlugin

typedef enum {
    NETWORK_TYPE_NONE= 0,
    NETWORK_TYPE_2G= 1,
    NETWORK_TYPE_3G= 2,
    NETWORK_TYPE_4G= 3,
    NETWORK_TYPE_5G= 4,//  5G目前为猜测结果
    NETWORK_TYPE_WIFI= 5,
}NETWORK_TYPE;


- (void)registerNetStatus:(CDVInvokedUrlCommand *)command{
    
    [[AFNetworkReachabilityManager sharedManager] setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        
        NSLog(@"Reachability: %@", AFStringFromNetworkReachabilityStatus(status));
        NSDictionary *dic = [self statusDic:status];
        
        CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
        [result setKeepCallbackAsBool:YES];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        
    }];
    
    [[AFNetworkReachabilityManager sharedManager] startMonitoring];
}



- (void)getNetStatus:(CDVInvokedUrlCommand *)command{
    
    [[AFNetworkReachabilityManager sharedManager] setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        
        NSLog(@"Reachability: %@", AFStringFromNetworkReachabilityStatus(status));
        NSDictionary *dic = [self statusDic:status];
        
        CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dic];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        
    }];
    
    [[AFNetworkReachabilityManager sharedManager] startMonitoring];
}


- (NSDictionary *) statusDic:(NSInteger) status {
    NSDictionary *dic = [[NSDictionary alloc] init];
    switch (status) {
        case 0:
            dic = @{@"message":@"NONE"};
            break;
        case 1:
            dic = [self dicCategory];
            break;
        case 2:
            dic = @{@"message":@"WIFI"};
            break;
        case -1:
            dic = @{@"message":@"Unknown"};
            break;
        default:
            break;
    }
    return dic;
}

- (NSDictionary *) dicCategory {
    
    NSDictionary *dic = [[NSDictionary alloc] init];
    NETWORK_TYPE num = [self getNetworkTypeFromStatusBar];
    switch (num) {
        case 1:
            dic = @{@"message":@"2G"};
            break;
        case 2:
            dic = @{@"message":@"3G"};
            break;
        case 3:
            dic = @{@"message":@"4G"};
            break;
        case 4:
            dic = @{@"message":@"5G"};
            break;
        default:
            break;
    }
    return dic;
}


////  网络类型
-(NETWORK_TYPE)getNetworkTypeFromStatusBar {
    UIApplication *app = [UIApplication sharedApplication];
    NSArray *subviews = [[[app valueForKey:@"statusBar"] valueForKey:@"foregroundView"] subviews];
    NSNumber *dataNetworkItemView = nil;
    for (id subview in subviews) {
        if([subview isKindOfClass:[NSClassFromString(@"UIStatusBarDataNetworkItemView") class]])     {
            dataNetworkItemView = subview;
            break;
        }
    }
    NETWORK_TYPE nettype = NETWORK_TYPE_NONE;
    NSNumber * num = [dataNetworkItemView valueForKey:@"dataNetworkType"];
    nettype = [num intValue];
    return nettype;
}


@end
