//
//  AFRequest.m
//  AFRequest
//
//  Created by Holy on 16/12/21.
//  Copyright © 2016年 QL. All rights reserved.
//

#import "AFRequest.h"
#import "AFNetworking.h"
#import "AFURLSessionManager.h"

#define requestTockenBaseURL @"https://api.weixin.qq.com/sns/oauth2/access_token"  //请求tocken
#define userInfoURL @"https://api.weixin.qq.com/sns/userinfo"  //请求用户头像等具体信息

@implementation AFRequest

- (void) requestDataWithParam:(NSDictionary *)param andBackResult:(AFResultBlock)block{
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    
    // 2.申明返回的结果是text/html类型
    mgr.responseSerializer = [AFHTTPResponseSerializer serializer];
    // 3.设置超时时间为10s
    mgr.requestSerializer.timeoutInterval = 10;
    
    NSDictionary *dic = @{@"appid":[param objectForKey:@"appid"],@"secret":[param objectForKey:@"secret"],@"code":[param objectForKey:@"code"],@"grant_type":@"authorization_code"};
    
    NSString *url = [self buildUrlStr:dic andTransactionSuffix:requestTockenBaseURL];
    
    [mgr GET:url parameters:nil progress:^(NSProgress * _Nonnull downloadProgress) {
        
    } success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        
        NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:responseObject options:NSJSONReadingMutableContainers error:nil];
        
        if ([dic objectForKey:@"errcode"]) {
            block(dic);
        }else{
            NSDictionary *requestDic = @{@"access_token":[dic objectForKey:@"access_token"],@"openid":[dic objectForKey:@"openid"]};
            
            NSString *url = [self buildUrlStr:requestDic andTransactionSuffix:userInfoURL];
            [mgr GET:url parameters:nil progress:^(NSProgress * _Nonnull downloadProgress) {
                
            } success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
                NSDictionary *succsDic = [NSJSONSerialization JSONObjectWithData:responseObject options:NSJSONReadingMutableContainers error:nil];
                block(succsDic);
                
                NSLog(@"success\n");
                
            } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
                NSLog(@"fail");
                NSDictionary *failDic = @{@"errcode":@40029,@"errmsg":@"invalid code"};
                block(failDic);
            }];
        }
        
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSLog(@"fail");
        NSDictionary *failDic = @{@"errcode":@40029,@"errmsg":@"invalid code"};
        block(failDic);
    }];
}


-(NSString*)buildUrlStr:(NSDictionary *)params andTransactionSuffix:(NSString *) urlSuffix{
    
    NSMutableString *urlString =[NSMutableString string];
    
    [urlString appendString:urlSuffix];
    
    NSString *escapedString;
    NSInteger keyIndex = 0;
    
    for (id key in params) {
        if(keyIndex == 0){
            
            escapedString =(NSString*)CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,(CFStringRef)[params valueForKey:key], NULL, CFSTR(":/?#[]@!$&’()*+,;="), kCFStringEncodingUTF8));
            [urlString appendFormat:@"?%@=%@",key,escapedString];
        }else{
            
            escapedString =(NSString*)CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,(CFStringRef)[params valueForKey:key], NULL, CFSTR(":/?#[]@!$&’()*+,;="), kCFStringEncodingUTF8));
            [urlString appendFormat:@"&%@=%@",key,escapedString];
            
        }
        
        keyIndex ++;
    }
    //    NSLog(@"urlstring:%@",urlString);
    return urlString;
}

@end
