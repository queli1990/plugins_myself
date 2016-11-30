//
//  YouTubeUrl.m
//  cordova_youtube
//
//  Created by Holy on 16/11/30.
//
//

#import "YouTubeUrl.h"
#import <XCDYouTubeKit/XCDYouTubeKit.h>

@implementation YouTubeUrl

- (void) getYouTubeUrl:(CDVInvokedUrlCommand *)command {
    NSString *youtubeID = command.arguments[0];
    if (youtubeID.length > 0) {
        [[XCDYouTubeClient defaultClient] getVideoWithIdentifier:youtubeID completionHandler:^(XCDYouTubeVideo * _Nullable video, NSError * _Nullable error) {
            //点播内容，区分码率
            if (video) {
//                NSLog(@"title:%@",video.title);
//                NSLog(@"duration:%f",video.duration);
//                NSLog(@"smallThumbnailURL:%@",video.smallThumbnailURL);
//                NSLog(@"mediumThumbnailURL:%@",video.mediumThumbnailURL);
//                NSLog(@"largeThumbnailURL:%@",video.largeThumbnailURL);
                NSLog(@"streamURLs:%@",[video.streamURLs objectForKey:@36]);
//                NSLog(@"expirationDate:%@",video.streamURLs[XCDYouTubeVideoQualityHTTPLiveStreaming]);
                NSString *normalUrlStr = [NSString stringWithFormat:@"%@", [video.streamURLs objectForKey:@36]];

                CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:normalUrlStr];
                [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
                
            } else {
                CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"failure or time out"];
                [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
            }
            
        }];
    } else {
        CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"have not got YouTubeID"];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
    }
    
    
}

- (void) getYouTubeLiveUrl:(CDVInvokedUrlCommand *)command {
    
    NSString *youtubeID = command.arguments[0];
    
    if (youtubeID.length > 0) {
        [[XCDYouTubeClient defaultClient] getVideoWithIdentifier:youtubeID completionHandler:^(XCDYouTubeVideo * _Nullable video, NSError * _Nullable error) {
            //直播内容，只有一个返回的URL
            if (video) {
                NSString *livingUrlStr = [NSString stringWithFormat:@"%@", [video.streamURLs objectForKey:@"HTTPLiveStreaming"]];
                
                NSLog(@"Liveing:%@",livingUrlStr);
                
                CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:livingUrlStr];
                [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
            }else {
                CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"failure or time out"];
                [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
            }
        }];

    }else{
        CDVPluginResult *result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"have not got YouTubeID"];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
    }
    
}

@end
