//
//  SkVPNServerEntry+Internal.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import "SkVPNServerEntry.h"

#include <ovpnapi.hpp>

using namespace skvpn;

@interface SkVPNServerEntry (Internal)

- (instancetype)initWithServerEntry:(ClientAPI::ServerEntry)entry;

@end
