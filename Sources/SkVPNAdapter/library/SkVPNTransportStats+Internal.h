//
//  SkVPNTransportStats+Internal.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import "SkVPNTransportStats.h"

#include <ovpnapi.hpp>

using namespace skvpn;

@interface SkVPNTransportStats (Internal)

- (instancetype)initWithTransportStats:(ClientAPI::TransportStats)stats;

@end
