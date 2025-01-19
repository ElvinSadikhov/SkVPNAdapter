//
//  SkVPNConnectionInfo+Internal.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import "SkVPNConnectionInfo.h"

#include <ovpnapi.hpp>

using namespace skvpn;

@interface SkVPNConnectionInfo (Internal)

- (instancetype)initWithConnectionInfo:(ClientAPI::ConnectionInfo)info;

@end
