//
//  SkVPNServerEntry.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import <Foundation/Foundation.h>

@interface SkVPNServerEntry : NSObject

@property (nullable, readonly, nonatomic) NSString *server;
@property (nullable, readonly, nonatomic) NSString *friendlyName;

- (nonnull instancetype) init NS_UNAVAILABLE;

@end
