//
//  SkVPNCredentials+Internal.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 24.04.17.
//
//
#import "SkVPNCredentials.h"

#include <ovpnapi.hpp>

using namespace skvpn;

@interface SkVPNCredentials (Internal)

@property (readonly) ClientAPI::ProvideCreds credentials;

@end
