//
//  SkVPNReachabilityTracker.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 17.07.17.
//
//

#include <skvpn/apple/reachable.hpp>

using namespace skvpn;

class SkVPNReachabilityTracker : public ReachabilityTracker {
public:
    SkVPNReachabilityTracker(const bool enable_internet, const bool enable_wifi, void* handler);
    
    virtual void reachability_tracker_event(const ReachabilityBase& rb, SCNetworkReachabilityFlags flags) override;
    
private:
    void* handler;
    
};
