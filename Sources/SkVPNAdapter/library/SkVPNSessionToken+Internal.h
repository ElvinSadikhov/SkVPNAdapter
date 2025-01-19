//
//  SkVPNSessionToken+Internal.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 28.04.17.
//
//

#import "SkVPNSessionToken.h"

#include <ovpnapi.hpp>

using namespace skvpn;

@interface SkVPNSessionToken (Internal)

- (instancetype)initWithSessionToken:(ClientAPI::SessionToken)token;

@end
