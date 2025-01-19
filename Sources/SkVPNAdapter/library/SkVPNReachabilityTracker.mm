//
//  SkVPNReachabilityTracker.m
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 17.07.17.
//
//

#import "SkVPNReachabilityTracker.h"

#import "SkVPNReachability+Internal.h"
#import "SkVPNReachabilityStatus.h"

SkVPNReachabilityTracker::SkVPNReachabilityTracker(const bool enable_internet, const bool enable_wifi, void* handler) :
    ReachabilityTracker(enable_internet, enable_wifi)
{
    this->handler = handler;
}

void SkVPNReachabilityTracker::reachability_tracker_event(const ReachabilityBase& rb, SCNetworkReachabilityFlags flags) {
    SkVPNReachability* handler = (__bridge SkVPNReachability* )this->handler;
    
    ReachabilityInterface::Status status = rb.status();
    switch (status) {
        case ReachabilityInterface::NotReachable:
            [handler updateReachabilityStatus:SkVPNReachabilityStatusNotReachable];
            break;
            
        case ReachabilityInterface::ReachableViaWiFi:
            [handler updateReachabilityStatus:SkVPNReachabilityStatusReachableViaWiFi];
            break;
            
        case ReachabilityInterface::ReachableViaWWAN:
            [handler updateReachabilityStatus:SkVPNReachabilityStatusReachableViaWWAN];
            break;
    }
}
