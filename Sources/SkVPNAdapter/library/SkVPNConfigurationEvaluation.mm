//
//  SkVPNConfigurationEvaluation.m
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import "SkVPNConfigurationEvaluation.h"
#import "SkVPNConfigurationEvaluation+Internal.h"

#include <skvpn/common/number.hpp>

#import "SkVPNConfiguration+Internal.h"
#import "SkVPNServerEntry+Internal.h"

using namespace skvpn;

@implementation SkVPNConfigurationEvaluation

- (instancetype)initWithEvalConfig:(ClientAPI::EvalConfig)eval {
    if (self = [super init]) {
        _username = !eval.userlockedUsername.empty() ? [NSString stringWithUTF8String:eval.userlockedUsername.c_str()] : nil;
        
        _profileName = !eval.profileName.empty() ? [NSString stringWithUTF8String:eval.profileName.c_str()] : nil;
        _friendlyName = !eval.friendlyName.empty() ? [NSString stringWithUTF8String:eval.friendlyName.c_str()] : nil;
        
        _autologin = eval.autologin;
        _externalPki = eval.externalPki;
        
        _staticChallenge = !eval.staticChallenge.empty() ? [NSString stringWithUTF8String:eval.staticChallenge.c_str()] : nil;
        _staticChallengeEcho = eval.staticChallengeEcho;
        
        _privateKeyPasswordRequired = eval.privateKeyPasswordRequired;
        _allowPasswordSave = eval.allowPasswordSave;
        
        _remoteHost = !eval.remoteHost.empty() ? [NSString stringWithUTF8String:eval.remoteHost.c_str()] : nil;
        
        uint16_t port = 0;
        parse_number(eval.remotePort, port);
        
        _remotePort = port;
        
        NSString *currentProto = [[[NSString stringWithUTF8String:eval.remoteProto.c_str()] componentsSeparatedByString:@"-"] firstObject];
        _remoteProto = [SkVPNConfiguration getTransportProtocolFromValue:currentProto];
        
        _servers = nil;
        
        if (!eval.serverList.empty()) {
            NSMutableArray *servers = [NSMutableArray new];
            
            for (ClientAPI::ServerEntry entry : eval.serverList) {
                SkVPNServerEntry *serverEntry = [[SkVPNServerEntry alloc] initWithServerEntry:entry];
                [servers addObject:serverEntry];
            }
            
            _servers = servers;
        }
    }
    return self;
}

@end
