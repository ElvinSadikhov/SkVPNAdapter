//
//  SkVPNReachability.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 17.07.17.
//
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, SkVPNReachabilityStatus);

@interface SkVPNReachability : NSObject

@property (readonly, nonatomic, getter=isTracking) BOOL tracking;
@property (readonly, nonatomic) SkVPNReachabilityStatus reachabilityStatus;

- (nonnull instancetype)init;

- (void)startTrackingWithCallback:(nonnull void (^)(SkVPNReachabilityStatus))callback;
- (void)stopTracking;

@end
