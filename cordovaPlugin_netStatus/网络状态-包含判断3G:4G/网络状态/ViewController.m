//
//  ViewController.m
//  网络状态
//
//  Created by Holy on 16/12/29.
//  Copyright © 2016年 QL. All rights reserved.
//

#import "ViewController.h"
#import "Reachability.h"
#import <CoreTelephony/CTTelephonyNetworkInfo.h>


@interface ViewController ()
@property (nonatomic,strong) Reachability *reachability;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    UIButton *btn1 = [UIButton buttonWithType:UIButtonTypeSystem];
    [btn1 setTitle:@"注册" forState:UIControlStateNormal];
    btn1.frame = CGRectMake(50, 50, 50, 50);
    btn1.backgroundColor = [UIColor redColor];
    [btn1 addTarget:self action:@selector(regist) forControlEvents:UIControlEventTouchUpInside];
    
    
    UIButton *btn2 = [UIButton buttonWithType:UIButtonTypeSystem];
    [btn2 setTitle:@"remove" forState:UIControlStateNormal];
    btn2.frame = CGRectMake(150, 150, 150, 150);
    btn2.backgroundColor = [UIColor redColor];
    [btn2 addTarget:self action:@selector(remove) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:btn1];
    [self.view addSubview:btn2];
}

- (void) regist {
    
    //创建Reachability
    _reachability = [Reachability reachabilityForInternetConnection];
    
    // 开始监控网络(一旦网络状态发生改变, 就会发出通知kReachabilityChangedNotification)
    [self.reachability startNotifier];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(networkStateChange) name:kReachabilityChangedNotification object:nil];
}

- (void)networkStateChange
{
    // 1.检测网络状态
    Reachability *wifi = [Reachability reachabilityForLocalWiFi];
    
    // 2.检测手机是否能上网络
    Reachability *connect = [Reachability reachabilityForInternetConnection];
    
    // 3.判断网络状态
    if ([wifi currentReachabilityStatus] != NotReachable) {
        if ([connect currentReachabilityStatus] == ReachableViaWWAN) {
            
            NSArray *typeStrings2G = @[CTRadioAccessTechnologyEdge,
                                       CTRadioAccessTechnologyGPRS,
                                       CTRadioAccessTechnologyCDMA1x];
            
            NSArray *typeStrings3G = @[CTRadioAccessTechnologyHSDPA,
                                       CTRadioAccessTechnologyWCDMA,
                                       CTRadioAccessTechnologyHSUPA,
                                       CTRadioAccessTechnologyCDMAEVDORev0,
                                       CTRadioAccessTechnologyCDMAEVDORevA,
                                       CTRadioAccessTechnologyCDMAEVDORevB,
                                       CTRadioAccessTechnologyeHRPD];
            
            NSArray *typeStrings4G = @[CTRadioAccessTechnologyLTE];
            
            NSString *str ;
            
            if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0) {
                CTTelephonyNetworkInfo *teleInfo= [[CTTelephonyNetworkInfo alloc] init];
                NSString *accessString = teleInfo.currentRadioAccessTechnology;
                if ([typeStrings4G containsObject:accessString]) {
                    str = @"4G网络";
                } else if ([typeStrings3G containsObject:accessString]) {
                    str =@"3G网络";
                } else if ([typeStrings2G containsObject:accessString]) {
                    str = @"2G网络";
                } else {
                    str = @"未知网络";
                }
            }
            
            
            UIAlertController *vc = [UIAlertController alertControllerWithTitle:str message:@"4g" preferredStyle:UIAlertControllerStyleAlert];
            
            UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
                NSLog(@"点击了cancel");
            }];
            UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"ok" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                NSLog(@"点击了ok");
            }];
            [vc addAction:okAction];
            [vc addAction:cancelAction];
            [self presentViewController:vc animated:YES completion:nil];
            
            return;
        }
        UIAlertController *vc = [UIAlertController alertControllerWithTitle:@"wifi" message:@"wifi" preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
            NSLog(@"点击了cancel");
        }];
        UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"ok" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            NSLog(@"点击了ok");
        }];
        [vc addAction:okAction];
        [vc addAction:cancelAction];
        [self presentViewController:vc animated:YES completion:nil];
    }
    else {
        UIAlertController *vc = [UIAlertController alertControllerWithTitle:@"none" message:@"none" preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
            NSLog(@"点击了cancel");
        }];
        UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"ok" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            NSLog(@"点击了ok");
        }];
        [vc addAction:okAction];
        [vc addAction:cancelAction];
        [self presentViewController:vc animated:YES completion:nil];
    }
}

- (void) remove {
    [_reachability stopNotifier];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kReachabilityChangedNotification object:nil];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
