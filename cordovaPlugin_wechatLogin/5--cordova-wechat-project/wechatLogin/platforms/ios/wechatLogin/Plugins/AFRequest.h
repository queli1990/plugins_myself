//
//  AFRequest.h
//  AFRequest
//
//  Created by Holy on 16/12/21.
//  Copyright © 2016年 QL. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AFRequest : NSObject

typedef void (^AFResultBlock)(NSDictionary *dic);

- (void) requestDataWithParam:(NSDictionary *)param andBackResult:(AFResultBlock)block;

@end
