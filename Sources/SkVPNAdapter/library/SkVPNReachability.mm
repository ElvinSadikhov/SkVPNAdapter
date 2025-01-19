//
//  SkVPNReachability.m
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 17.07.17.
//
//

#import "SkVPNReachability.h"
#import "SkVPNReachability+Internal.h"

#include <skvpn/apple/reachable.hpp>

#import "SkVPNReachabilityTracker.h"
#import "SkVPNReachabilityStatus.h"

@interface SkVPNReachability () {
    BOOL _isTracking;
}

@property (assign, nonatomic) SkVPNReachabilityTracker *tracker;
@property (assign, nonatomic) Reachability *reachability;

@property (copy, nonatomic) void (^ reachabilityStatusChangedBlock)(SkVPNReachabilityStatus);

@end

@implementation SkVPNReachability (Internal)

- (void)updateReachabilityStatus:(SkVPNReachabilityStatus)status {
    if (self.reachabilityStatusChangedBlock) { self.reachabilityStatusChangedBlock(status); }
}

@end

@implementation SkVPNReachability

- (BOOL)isTracking {
    return _isTracking;
}

- (SkVPNReachabilityStatus)reachabilityStatus {
    ReachabilityInterface::Status status = self.reachability->reachable();
    switch (status) {
        case ReachabilityInterface::NotReachable: return SkVPNReachabilityStatusNotReachable;
        case ReachabilityInterface::ReachableViaWiFi: return SkVPNReachabilityStatusReachableViaWiFi;
        case ReachabilityInterface::ReachableViaWWAN: return SkVPNReachabilityStatusReachableViaWWAN;
    }
}

- (instancetype)init {
    if (self = [super init]) {
        _isTracking = NO;

        _tracker = new SkVPNReachabilityTracker(true, false, (__bridge void *)self);
        _reachability = new Reachability(true, true);
    }
    return self;
}

- (void)startTrackingWithCallback:(void (^)(SkVPNReachabilityStatus))callback {
    self.reachabilityStatusChangedBlock = callback;
    
    dispatch_queue_t main = dispatch_get_main_queue();
    dispatch_async(main, ^{
        self.tracker->reachability_tracker_schedule();
    });
    
    _isTracking = YES;
}

- (void)stopTracking {
    dispatch_queue_t main = dispatch_get_main_queue();
    dispatch_async(main, ^{
        self.tracker->reachability_tracker_cancel();
    });
    
    _isTracking = NO;
}

- (void)dealloc {
    delete _tracker;
    delete _reachability;
}

@end
