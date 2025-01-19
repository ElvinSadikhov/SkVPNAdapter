//
//  SkVPNTransportProtocol.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import <Foundation/Foundation.h>

/**
 Transport protocol options
 */
typedef NS_ENUM(NSInteger, SkVPNTransportProtocol) {
    ///
    SkVPNTransportProtocolUDP,
    ///
    SkVPNTransportProtocolTCP,
    ///
    SkVPNTransportProtocolAdaptive,
    /// Use a transport protocol specified in the profile
    SkVPNTransportProtocolDefault
};
