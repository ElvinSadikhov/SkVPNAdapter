//
//  SkVPNMinTLSVersion.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import <Foundation/Foundation.h>

/**
 Minimum TLS version options
 */
typedef NS_ENUM(NSInteger, SkVPNMinTLSVersion) {
    /// Don't specify a minimum, and disable any minimum specified in profile
    SkVPNMinTLSVersionDisabled,
    /// Use TLS 1.0 minimum (overrides profile)
    SkVPNMinTLSVersion10,
    /// Use TLS 1.1 minimum (overrides profile)
    SkVPNMinTLSVersion11,
    /// Use TLS 1.2 minimum (overrides profile)
    SkVPNMinTLSVersion12,
    /// Use profile minimum
    SkVPNMinTLSVersionDefault
};
