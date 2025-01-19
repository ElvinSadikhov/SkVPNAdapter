//
//  NSError+SkVPNError.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 17.01.2018.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString *const SkVPNAdapterErrorDomain;

typedef NS_ERROR_ENUM(SkVPNAdapterErrorDomain, SkVPNAdapterError);

@interface NSError (SkVPNAdapterErrorGeneration)

+ (NSError *)ovpn_errorObjectForAdapterError:(SkVPNAdapterError)adapterError
                                 description:(NSString *)description
                                     message:(nullable NSString *)message
                                       fatal:(BOOL)fatal;

+ (SkVPNAdapterError)ovpn_adapterErrorByName:(NSString *)errorName;

@end

@interface NSError (SkVPNMbedTLSErrorGeneration)

+ (NSError *)ovpn_errorObjectForMbedTLSError:(NSInteger)errorCode description:(NSString *)description;

@end

NS_ASSUME_NONNULL_END
