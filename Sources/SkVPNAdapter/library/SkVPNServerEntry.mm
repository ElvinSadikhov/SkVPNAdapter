//
//  SkVPNServerEntry.mm
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import "SkVPNServerEntry.h"
#import "SkVPNServerEntry+Internal.h"

@implementation SkVPNServerEntry

- (instancetype)initWithServerEntry:(ClientAPI::ServerEntry)entry {
    if (self = [super init]) {
        _server = !entry.server.empty() ? [NSString stringWithUTF8String:entry.server.c_str()] : nil;
        _friendlyName = !entry.friendlyName.empty() ? [NSString stringWithUTF8String:entry.friendlyName.c_str()] : nil;
    }
    return self;
}

@end
