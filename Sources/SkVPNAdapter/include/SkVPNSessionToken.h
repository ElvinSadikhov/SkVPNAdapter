//
//  SkVPNSessionToken.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 28.04.17.
//
//

#import <Foundation/Foundation.h>

/**
 Class used to get session token from VPN core
 */
@interface SkVPNSessionToken : NSObject <NSCopying, NSSecureCoding>

@property (nullable, readonly, nonatomic) NSString *username;

/**
 An SkVPN Session ID, used as a proxy for password
 */
@property (nullable, readonly, nonatomic) NSString *session;

@end
