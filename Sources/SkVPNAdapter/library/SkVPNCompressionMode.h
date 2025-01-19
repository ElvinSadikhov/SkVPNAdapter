//
//  SkVPNCompressionMode.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import <Foundation/Foundation.h>

/**
 Compression mode options
 */
typedef NS_ENUM(NSInteger, SkVPNCompressionMode) {
    /// Allow compression on both uplink and downlink
    SkVPNCompressionModeEnabled,
    /// Support compression stubs only
    SkVPNCompressionModeDisabled,
    /// Allow compression on downlink only (i.e. server -> client)
    SkVPNCompressionModeAsym,
    /// Default behavior (support compression stubs only)
    SkVPNCompressionModeDefault
};
