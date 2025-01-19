//
//  SkVPNIPv6Preference.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import <Foundation/Foundation.h>

/**
 IPv6 preference options
 */
typedef NS_ENUM(NSInteger, SkVPNIPv6Preference) {
    /// Request combined IPv4/IPv6 tunnel
    SkVPNIPv6PreferenceEnabled,
    /// Disable IPv6, so tunnel will be IPv4-only
    SkVPNIPv6PreferenceDisabled,
    /// Leave decision to server
    SkVPNIPv6PreferenceDefault
};
