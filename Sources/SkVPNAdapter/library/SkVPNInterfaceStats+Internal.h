//
//  SkVPNInterfaceStats+Internal.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import "SkVPNInterfaceStats.h"

#include <ovpnapi.hpp>

using namespace skvpn;

@interface SkVPNInterfaceStats (Internal)

- (instancetype)initWithInterfaceStats:(ClientAPI::InterfaceStats)stats;

@end
