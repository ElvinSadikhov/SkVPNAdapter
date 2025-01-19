//
//  SkVPNInterfaceStats.m
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import "SkVPNInterfaceStats.h"
#import "SkVPNInterfaceStats+Internal.h"

@interface SkVPNInterfaceStats ()
@property (readwrite, nonatomic) NSInteger bytesIn;
@property (readwrite, nonatomic) NSInteger bytesOut;
@property (readwrite, nonatomic) NSInteger packetsIn;
@property (readwrite, nonatomic) NSInteger packetsOut;
@property (readwrite, nonatomic) NSInteger errorsIn;
@property (readwrite, nonatomic) NSInteger errorsOut;
@end

@implementation SkVPNInterfaceStats

- (instancetype)initWithInterfaceStats:(ClientAPI::InterfaceStats)stats {
    if (self = [super init]) {
        self.bytesIn = stats.bytesIn;
        self.bytesOut = stats.bytesOut;
        self.packetsIn = stats.packetsIn;
        self.packetsOut = stats.packetsOut;
        self.errorsIn = stats.errorsIn;
        self.errorsOut = stats.errorsOut;
    }
    return self;
}

- (nonnull id)copyWithZone:(nullable NSZone *)zone {
    SkVPNInterfaceStats *stats = [[SkVPNInterfaceStats allocWithZone:zone] init];
    stats.bytesIn = self.bytesIn;
    stats.bytesOut = self.bytesOut;
    stats.packetsIn = self.packetsIn;
    stats.packetsOut = self.packetsOut;
    stats.errorsIn = self.errorsIn;
    stats.errorsOut = self.errorsOut;
    return stats;
}

- (void)encodeWithCoder:(nonnull NSCoder *)aCoder {
    [aCoder encodeInteger:self.bytesIn forKey:NSStringFromSelector(@selector(bytesIn))];
    [aCoder encodeInteger:self.bytesOut forKey:NSStringFromSelector(@selector(bytesOut))];
    [aCoder encodeInteger:self.packetsIn forKey:NSStringFromSelector(@selector(packetsIn))];
    [aCoder encodeInteger:self.packetsOut forKey:NSStringFromSelector(@selector(packetsOut))];
    [aCoder encodeInteger:self.errorsIn forKey:NSStringFromSelector(@selector(errorsIn))];
    [aCoder encodeInteger:self.errorsOut forKey:NSStringFromSelector(@selector(errorsOut))];
}

- (nullable instancetype)initWithCoder:(nonnull NSCoder *)aDecoder {
    if (self = [self init]) {
        self.bytesIn = [aDecoder decodeIntegerForKey:NSStringFromSelector(@selector(bytesIn))];
        self.bytesOut = [aDecoder decodeIntegerForKey:NSStringFromSelector(@selector(bytesOut))];
        self.packetsIn = [aDecoder decodeIntegerForKey:NSStringFromSelector(@selector(packetsIn))];
        self.packetsOut = [aDecoder decodeIntegerForKey:NSStringFromSelector(@selector(packetsOut))];
        self.errorsIn = [aDecoder decodeIntegerForKey:NSStringFromSelector(@selector(errorsIn))];
        self.errorsOut = [aDecoder decodeIntegerForKey:NSStringFromSelector(@selector(errorsOut))];
    }
    return self;
}

+ (BOOL)supportsSecureCoding {
    return YES;
}

@end
