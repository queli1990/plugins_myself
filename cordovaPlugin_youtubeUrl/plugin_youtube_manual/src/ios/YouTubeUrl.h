//
//  YouTubeUrl.h
//  cordova_youtube
//
//  Created by Holy on 16/11/30.
//
//

#import <Cordova/CDV.h>

@interface YouTubeUrl : CDVPlugin

- (void) getYouTubeLiveUrl:(CDVInvokedUrlCommand *)command;

- (void) getYouTubeUrl:(CDVInvokedUrlCommand *)command;

@end
