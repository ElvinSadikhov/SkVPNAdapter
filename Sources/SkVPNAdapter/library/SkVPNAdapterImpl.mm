//
//  SkVPNAdapter.m
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 11.02.17.
//
//

#define SKVPN_EXTERN extern

#define TUNNEL_CONFIGURATION_TIMEOUT 30

#import "SkVPNAdapterImpl.h"

#import <NetworkExtension/NetworkExtension.h>

#import "SkVPNClient.h"
#import "SkVPNError.h"
#import "SkVPNAdapterEvent.h"
#import "SkVPNPacketFlowBridge.h"
#import "SkVPNNetworkSettingsBuilder.h"
#import "SkVPNAdapterPacketFlow.h"
#import "SkVPNCredentials+Internal.h"
#import "SkVPNConfiguration+Internal.h"
#import "SkVPNConnectionInfo+Internal.h"
#import "SkVPNInterfaceStats+Internal.h"
#import "SkVPNConfigurationEvaluation+Internal.h"
#import "SkVPNSessionToken+Internal.h"
#import "SkVPNTransportStats+Internal.h"
#import "NSError+SkVPNError.h"

@interface SkVPNAdapter () <SkVPNClientDelegate>

@property (nonatomic) SkVPNClient *vpnClient;

@property (nonatomic) SkVPNPacketFlowBridge *packetFlowBridge;
@property (nonatomic) SkVPNNetworkSettingsBuilder *networkSettingsBuilder;

@end

@implementation SkVPNAdapter

- (instancetype)init {
    if (self = [super init]) {
        _vpnClient = new SkVPNClient(self);
        _packetFlowBridge = [[SkVPNPacketFlowBridge alloc] init];
    }
    return self;
}

#pragma mark - SkVPNClient Lifecycle

+ (nullable SkVPNConfigurationEvaluation *)evaluateConfiguration:(SkVPNConfiguration *)configuration error:(NSError **)error {
    ClientAPI::EvalConfig eval = SkVPNClient::eval_config_static(configuration.config);
    
    if (eval.error) {
        if (error) {
            NSString *message = [NSString stringWithUTF8String:eval.message.c_str()];
            *error = [NSError ovpn_errorObjectForAdapterError:SkVPNAdapterErrorConfigurationFailure
                                                  description:@"Failed to evaluate SkVPN configuration."
                                                      message:message
                                                        fatal:YES];
        }
        
        return nil;
    }
    
    return [[SkVPNConfigurationEvaluation alloc] initWithEvalConfig:eval];
}

- (SkVPNConfigurationEvaluation *)applyConfiguration:(SkVPNConfiguration *)configuration error:(NSError * __autoreleasing *)error {
    ClientAPI::EvalConfig eval = self.vpnClient->apply_config(configuration.config);
    
    if (eval.error) {
        if (error) {
            NSString *message = [NSString stringWithUTF8String:eval.message.c_str()];
            *error = [NSError ovpn_errorObjectForAdapterError:SkVPNAdapterErrorConfigurationFailure
                                                  description:@"Failed to apply SkVPN configuration."
                                                      message:message
                                                        fatal:YES];
        }
        
        return nil;
    }
    
    return [[SkVPNConfigurationEvaluation alloc] initWithEvalConfig:eval];
}

- (BOOL)provideCredentials:(SkVPNCredentials *)credentials error:(NSError * __autoreleasing *)error {
    ClientAPI::Status status = self.vpnClient->provide_creds(credentials.credentials);
    
    if (status.error) {
        if (error) {
            NSString *message = [NSString stringWithUTF8String:status.message.c_str()];
            *error = [NSError ovpn_errorObjectForAdapterError:SkVPNAdapterErrorCredentialsFailure
                                          description:@"Failed to provide SkVPN credentials."
                                              message:message
                                                fatal:YES];
        }
        
        return NO;
    }
    
    return YES;
}

- (void)connectUsingPacketFlow:(id<SkVPNAdapterPacketFlow>)packetFlow {
    NSAssert(self.delegate != nil, @"delegate property shouldn't be nil, set it before trying to establish connection.");
    
    self.packetFlowBridge.packetFlow = packetFlow;
    
    dispatch_queue_t connectQueue = dispatch_queue_create("me.ss-abramchuk.skvpn-adapter.connection.", DISPATCH_QUEUE_SERIAL);
    
    dispatch_async(connectQueue, ^{
        ClientAPI::Status status = self.vpnClient->connect();
        [self handleConnectionStatus:status];
    });
}

- (void)reconnectAfterTimeInterval:(NSTimeInterval)timeInterval {
    self.vpnClient->reconnect(timeInterval);
}

- (void)disconnect {
    self.vpnClient->stop();
}

- (void)pauseWithReason:(NSString *)reason {
    self.vpnClient->pause(std::string(reason.UTF8String));
}

- (void)resume {
    self.vpnClient->resume();
}

- (void)handleConnectionStatus:(ClientAPI::Status)status {
    if (!status.error) { return; }
    
    SkVPNAdapterError adapterError = !status.status.empty() ?
        [NSError ovpn_adapterErrorByName:[NSString stringWithUTF8String:status.status.c_str()]] :
        SkVPNAdapterErrorUnknown;
    
    NSString *message = [NSString stringWithUTF8String:status.message.c_str()];
    NSError *error = [NSError ovpn_errorObjectForAdapterError:adapterError
                                          description:@"Failed to establish connection with SkVPN server."
                                              message:message
                                                fatal:YES];

    [self.delegate skVPNAdapter:self handleError:error];
}

#pragma mark - SkVPNClient Information

+ (NSString *)copyright {
    return [NSString stringWithUTF8String:SkVPNClient::copyright().c_str()];
}

+ (NSString *)platform {
    return [NSString stringWithUTF8String:SkVPNClient::platform().c_str()];
}

- (SkVPNConnectionInfo *)connectionInformation {
    ClientAPI::ConnectionInfo information = self.vpnClient->connection_info();
    return information.defined ? [[SkVPNConnectionInfo alloc] initWithConnectionInfo:information] : nil;
}

- (SkVPNInterfaceStats *)interfaceStatistics {
    return [[SkVPNInterfaceStats alloc] initWithInterfaceStats:self.vpnClient->tun_stats()];
}

- (SkVPNSessionToken *)sessionToken {
    ClientAPI::SessionToken token;
    return self.vpnClient->session_token(token) ? [[SkVPNSessionToken alloc] initWithSessionToken:token] : nil;
}

- (SkVPNTransportStats *)transportStatistics {
    return [[SkVPNTransportStats alloc] initWithTransportStats:self.vpnClient->transport_stats()];
}

#pragma mark - Lazy Initialization

- (SkVPNNetworkSettingsBuilder *)networkSettingsBuilder {
    if (!_networkSettingsBuilder) { _networkSettingsBuilder = [[SkVPNNetworkSettingsBuilder alloc] init]; }
    return _networkSettingsBuilder;
}

#pragma mark - SkVPNClientDelegate

- (BOOL)setRemoteAddress:(NSString *)address {
    self.networkSettingsBuilder.remoteAddress = address;
    return YES;
}

- (BOOL)addIPV4Address:(NSString *)address subnetMask:(NSString *)subnetMask gateway:(NSString *)gateway {
    self.networkSettingsBuilder.ipv4DefaultGateway = gateway;
    
    [self.networkSettingsBuilder.ipv4LocalAddresses addObject:address];
    [self.networkSettingsBuilder.ipv4SubnetMasks addObject:subnetMask];
    
    return YES;
}

- (BOOL)addIPV6Address:(NSString *)address prefixLength:(NSNumber *)prefixLength gateway:(NSString *)gateway {
    self.networkSettingsBuilder.ipv6DefaultGateway = gateway;
    
    [self.networkSettingsBuilder.ipv6LocalAddresses addObject:address];
    [self.networkSettingsBuilder.ipv6NetworkPrefixLengths addObject:prefixLength];
    
    return YES;
}

- (BOOL)addIPV4Route:(NEIPv4Route *)route {
    route.gatewayAddress = self.networkSettingsBuilder.ipv4DefaultGateway;
    
    NSUInteger index = [self.networkSettingsBuilder.ipv4IncludedRoutes indexOfObjectPassingTest:^BOOL(NEIPv4Route *obj, NSUInteger idx, BOOL *stop) {
        return [obj.destinationAddress isEqualToString:route.destinationAddress] &&
            [obj.destinationSubnetMask isEqualToString:route.destinationSubnetMask];
    }];
    
    if (index == NSNotFound) {
        [self.networkSettingsBuilder.ipv4IncludedRoutes addObject:route];
        return YES;
    } else {
        return NO;
    }
}

- (BOOL)addIPV6Route:(NEIPv6Route *)route {
    route.gatewayAddress = self.networkSettingsBuilder.ipv6DefaultGateway;
    
    NSUInteger index = [self.networkSettingsBuilder.ipv6IncludedRoutes indexOfObjectPassingTest:^BOOL(NEIPv6Route *obj, NSUInteger idx, BOOL *stop) {
        return [obj.destinationAddress isEqualToString:route.destinationAddress] &&
            obj.destinationNetworkPrefixLength == route.destinationNetworkPrefixLength;
    }];
    
    if (index == NSNotFound) {
        [self.networkSettingsBuilder.ipv6IncludedRoutes addObject:route];
        return YES;
    } else {
        return NO;
    }
}

- (BOOL)excludeIPV4Route:(NEIPv4Route *)route {
    NSUInteger index = [self.networkSettingsBuilder.ipv4ExcludedRoutes indexOfObjectPassingTest:^BOOL(NEIPv4Route *obj, NSUInteger idx, BOOL *stop) {
        return [obj.destinationAddress isEqualToString:route.destinationAddress] &&
        [obj.destinationSubnetMask isEqualToString:route.destinationSubnetMask];
    }];
    
    if (index == NSNotFound) {
        [self.networkSettingsBuilder.ipv4ExcludedRoutes addObject:route];
        return YES;
    } else {
        return NO;
    }
}

- (BOOL)excludeIPV6Route:(NEIPv6Route *)route {
    NSUInteger index = [self.networkSettingsBuilder.ipv6ExcludedRoutes indexOfObjectPassingTest:^BOOL(NEIPv6Route *obj, NSUInteger idx, BOOL *stop) {
        return [obj.destinationAddress isEqualToString:route.destinationAddress] &&
        obj.destinationNetworkPrefixLength == route.destinationNetworkPrefixLength;
    }];
    
    if (index == NSNotFound) {
        [self.networkSettingsBuilder.ipv6ExcludedRoutes addObject:route];
        return YES;
    } else {
        return NO;
    }
}

- (BOOL)addDNS:(NSString *)dns {
    if ([self.networkSettingsBuilder.dnsServers containsObject:dns]) {
        return NO;
    }
    
    [self.networkSettingsBuilder.dnsServers addObject:dns];
    return YES;
}

- (BOOL)addSearchDomain:(NSString *)domain {
    if ([self.networkSettingsBuilder.searchDomains containsObject:domain]) {
        return NO;
    }
    
    [self.networkSettingsBuilder.searchDomains addObject:domain];
    return YES;
}

- (BOOL)setMTU:(NSNumber *)mtu {
    self.networkSettingsBuilder.mtu = mtu;
    return YES;
}

- (BOOL)setSessionName:(NSString *)name {
    _sessionName = name;
    return YES;
}

- (BOOL)addProxyBypassHost:(NSString *)bypassHost {
    if ([self.networkSettingsBuilder.proxyExceptionList containsObject:bypassHost]) {
        return NO;
    }
    
    [self.networkSettingsBuilder.proxyExceptionList addObject:bypassHost];
    return YES;
}

- (BOOL)setProxyAutoConfigurationURL:(NSURL *)url {
    self.networkSettingsBuilder.autoProxyConfigurationEnabled = YES;
    self.networkSettingsBuilder.proxyAutoConfigurationURL = url;
    
    return YES;
}

- (BOOL)setProxyServer:(NEProxyServer *)server protocol:(SkVPNProxyServerProtocol)protocol {
    switch (protocol) {
        case SkVPNProxyServerProtocolHTTP:
            self.networkSettingsBuilder.httpProxyServerEnabled = YES;
            self.networkSettingsBuilder.httpProxyServer = server;
            break;
        
        case SkVPNProxyServerProtocolHTTPS:
            self.networkSettingsBuilder.httpsProxyServerEnabled = YES;
            self.networkSettingsBuilder.httpsProxyServer = server;
            break;
    }
    
    return YES;
}

- (BOOL)establishTunnel {
    NEPacketTunnelNetworkSettings *networkSettings = [self.networkSettingsBuilder networkSettings];
    
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
    
    __block NSError *configurationError;
    void (^completionHandler)(NSError *error) = ^(NSError *error) {
        configurationError = error;
        dispatch_semaphore_signal(semaphore);
    };
    
    [self.delegate skVPNAdapter:self configureTunnelWithNetworkSettings:networkSettings completionHandler:completionHandler];
    
    long timeout = dispatch_semaphore_wait(semaphore, dispatch_time(DISPATCH_TIME_NOW, TUNNEL_CONFIGURATION_TIMEOUT * NSEC_PER_SEC));
    if (timeout) { return NO; }
    
    if (configurationError) {
        NSDictionary *userInfo = @{
            NSLocalizedDescriptionKey: @"Failed to configure tunnel using provided settings. Check underlying error for more details.",
            NSUnderlyingErrorKey: configurationError,
            SkVPNAdapterErrorFatalKey: @(YES)
        };
        
        NSError *error = [NSError errorWithDomain:SkVPNAdapterErrorDomain
                                             code:SkVPNAdapterErrorTUNSetupFailed
                                         userInfo:userInfo];
        
        [self.delegate skVPNAdapter:self handleError:error];
        
        return NO;
    }
    
    NSError *socketError;
    if ([self.packetFlowBridge configureSocketsWithError:&socketError]) {
        [self.packetFlowBridge startReading];
        return YES;
    } else {
        if (socketError) { [self.delegate skVPNAdapter:self handleError:socketError]; }
        return NO;
    }
}

- (CFSocketNativeHandle)socketHandle {
    return CFSocketGetNative(self.packetFlowBridge.skVPNSocket);
}

- (void)clientEventName:(NSString *)eventName message:(NSString *)message {
    NSDictionary *events = @{
        @"DISCONNECTED": @(SkVPNAdapterEventDisconnected),
        @"CONNECTED": @(SkVPNAdapterEventConnected),
        @"RECONNECTING": @(SkVPNAdapterEventReconnecting),
        @"AUTH_PENDING": @(SkVPNAdapterEventAuthPending),
        @"RESOLVE": @(SkVPNAdapterEventResolve),
        @"WAIT": @(SkVPNAdapterEventWait),
        @"WAIT_PROXY": @(SkVPNAdapterEventWaitProxy),
        @"CONNECTING": @(SkVPNAdapterEventConnecting),
        @"GET_CONFIG": @(SkVPNAdapterEventGetConfig),
        @"ASSIGN_IP": @(SkVPNAdapterEventAssignIP),
        @"ADD_ROUTES": @(SkVPNAdapterEventAddRoutes),
        @"ECHO": @(SkVPNAdapterEventEcho),
        @"INFO": @(SkVPNAdapterEventInfo),
        @"WARN": @(SkVPNAdapterEventWarn),
        @"PAUSE": @(SkVPNAdapterEventPause),
        @"RESUME": @(SkVPNAdapterEventResume),
        @"RELAY": @(SkVPNAdapterEventRelay),
        @"COMPRESSION_ENABLED": @(SkVPNAdapterEventCompressionEnabled),
        @"UNSUPPORTED_FEATURE": @(SkVPNAdapterEventUnsupportedFeature)
    };
    
    SkVPNAdapterEvent event = events[eventName] != nil ?
        (SkVPNAdapterEvent)[events[eventName] integerValue] : SkVPNAdapterEventUnknown;
    
    [self.delegate skVPNAdapter:self handleEvent:event message:message];
}

- (void)clientErrorName:(NSString *)errorName fatal:(BOOL)fatal message:(NSString *)message {
    SkVPNAdapterError adapterError = [NSError ovpn_adapterErrorByName:errorName];
    NSString *description = fatal ? @"SkVPN fatal error occured" : @"SkVPN error occured";
    
    NSError *error = [NSError ovpn_errorObjectForAdapterError:adapterError
                                                  description:description
                                                      message:message
                                                        fatal:fatal];

    [self.delegate skVPNAdapter:self handleError:error];
}

- (void)clientLogMessage:(NSString *)logMessage {
    if ([self.delegate respondsToSelector:@selector(skVPNAdapter:handleLogMessage:)]) {
        [self.delegate skVPNAdapter:self handleLogMessage:logMessage];
    }
}

- (void)tick {
    if ([self.delegate respondsToSelector:@selector(skVPNAdapterDidReceiveClockTick:)]) {
        [self.delegate skVPNAdapterDidReceiveClockTick:self];
    }
}

- (void)resetSettings {
    _sessionName = nil;
    _networkSettingsBuilder = nil;
}

- (void)resetTun {
    [_packetFlowBridge invalidateSocketsIfNeeded];
    
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
    
    __block NSError *configurationError;
    void (^completionHandler)(NSError *error) = ^(NSError *error) {
        configurationError = error;
        dispatch_semaphore_signal(semaphore);
    };
    
    [self.delegate skVPNAdapter:self configureTunnelWithNetworkSettings:nil completionHandler:completionHandler];
    
    dispatch_semaphore_wait(semaphore, dispatch_time(DISPATCH_TIME_NOW, TUNNEL_CONFIGURATION_TIMEOUT * NSEC_PER_SEC));
    
    if (configurationError) {
        NSDictionary *userInfo = @{
            NSLocalizedDescriptionKey: @"Failed to reset tunnel. Check underlying error for more details.",
            NSUnderlyingErrorKey: configurationError,
            SkVPNAdapterErrorFatalKey: @(YES)
        };
        
        NSError *error = [NSError errorWithDomain:SkVPNAdapterErrorDomain
                                             code:SkVPNAdapterErrorTUNSetupFailed
                                         userInfo:userInfo];
        
        [self.delegate skVPNAdapter:self handleError:error];
    }
}

#pragma mark -

- (void)dealloc {
    delete _vpnClient;
}

@end
