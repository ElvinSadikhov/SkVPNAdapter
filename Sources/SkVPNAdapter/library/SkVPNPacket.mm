//
//  SkVPNPacket.m
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 15.01.2018.
//

#import "SkVPNPacket.h"

#include <arpa/inet.h>

@interface SkVPNPacket () {
    NSData *_data;
    NSNumber *_protocolFamily;
}

@end

@implementation SkVPNPacket

- (instancetype)initWithVPNData:(NSData *)data {
    if (self = [super init]) {
        // Get network protocol family from data prefix
        NSUInteger prefix_size = sizeof(uint32_t);
        
        uint32_t protocol = PF_UNSPEC;
        [data getBytes:&protocol length:prefix_size];
        protocol = CFSwapInt32BigToHost(protocol);
        
        NSRange range = NSMakeRange(prefix_size, data.length - prefix_size);
        NSData *packetData = [data subdataWithRange:range];
        
        _data = packetData;
        _protocolFamily = @(protocol);
    }
    return self;
}

- (instancetype)initWithPacketFlowData:(NSData *)data protocolFamily:(NSNumber *)protocolFamily {
    if (self = [super init]) {
        _data = data;
        _protocolFamily = protocolFamily;
    }
    return self;
}

- (NSData *)vpnData {
    // Prepend data with network protocol. It should be done because SkVPN uses uint32_t prefixes containing network
    // protocol.
    uint32_t prefix = CFSwapInt32HostToBig(_protocolFamily.unsignedIntegerValue);
    NSUInteger prefix_size = sizeof(uint32_t);
    
    NSMutableData *data = [NSMutableData dataWithCapacity:prefix_size + _data.length];
    
    [data appendBytes:&prefix length:prefix_size];
    [data appendData:_data];
    
    return data;
}

- (NSData *)packetFlowData {
    return _data;
}

- (NSNumber *)protocolFamily {
    return _protocolFamily;
}

@end
