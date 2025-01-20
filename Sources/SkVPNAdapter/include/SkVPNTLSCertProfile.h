//
//  SkVPNTLSCertProfile.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import <Foundation/Foundation.h>

/**
 Options of the tls-cert-profile setting
 */
typedef NS_ENUM(NSInteger, SkVPNTLSCertProfile) {
    /// Allow 1024-bit RSA certs signed with SHA1
    SkVPNTLSCertProfileLegacy,
    /// Require at least 2048-bit RSA certs signed with SHA256 or higher
    SkVPNTLSCertProfilePreferred,
    /// Require NSA Suite-B
    SkVPNTLSCertProfileSuiteB,
    /// Use legacy as the default if profile doesn't specify tls-cert-profile
    SkVPNTLSCertProfileLegacyDefault,
    /// Use preferred as the default if profile doesn't specify tls-cert-profile
    SkVPNTLSCertProfilePreferredDefault,
    /// Use profile default
    SkVPNTLSCertProfileDefault
};
