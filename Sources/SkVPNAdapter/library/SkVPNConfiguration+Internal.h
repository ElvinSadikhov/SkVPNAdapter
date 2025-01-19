//
//  SkVPNConfiguration+Internal.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 21.04.17.
//
//

#import "SkVPNConfiguration.h"

#include <ovpnapi.hpp>

using namespace skvpn;

NS_ASSUME_NONNULL_BEGIN

@interface SkVPNConfiguration (Internal)

@property (readonly) ClientAPI::Config config;

+ (SkVPNTransportProtocol)getTransportProtocolFromValue:(nullable NSString *)value;
+ (NSString *)getValueFromTransportProtocol:(SkVPNTransportProtocol)protocol;

+ (SkVPNIPv6Preference)getIPv6PreferenceFromValue:(nullable NSString *)value;
+ (NSString *)getValueFromIPv6Preference:(SkVPNIPv6Preference)preference;

+ (SkVPNCompressionMode)getCompressionModeFromValue:(nullable NSString *)value;
+ (NSString *)getValueFromCompressionMode:(SkVPNCompressionMode)compressionMode;

+ (SkVPNMinTLSVersion)getMinTLSFromValue:(nullable NSString *)value;
+ (NSString *)getValueFromMinTLS:(SkVPNMinTLSVersion)minTLS;

+ (SkVPNTLSCertProfile)getTLSCertProfileFromValue:(nullable NSString *)value;
+ (NSString *)getValueFromTLSCertProfile:(SkVPNTLSCertProfile)tlsCertProfile;

@end

NS_ASSUME_NONNULL_END
