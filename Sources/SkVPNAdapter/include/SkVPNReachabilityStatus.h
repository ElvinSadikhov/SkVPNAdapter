//
//  SkVPNReachabilityStatus.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 17.07.17.
//
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, SkVPNReachabilityStatus) {
    SkVPNReachabilityStatusNotReachable,
    SkVPNReachabilityStatusReachableViaWiFi,
    SkVPNReachabilityStatusReachableViaWWAN
};
