//
//  SkVPNPrivateKey.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 07.09.17.
//
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, SkVPNKeyType);

NS_ASSUME_NONNULL_BEGIN

@interface SkVPNPrivateKey : NSObject

+ (nullable SkVPNPrivateKey *)keyWithPEM:(NSData *)pemData
                                  password:(nullable NSString *)password
                                     error:(NSError **)error;

+ (nullable SkVPNPrivateKey *)keyWithDER:(NSData *)derData
                                  password:(nullable NSString *)password
                                     error:(NSError **)error;

- (instancetype) init NS_UNAVAILABLE;

@property (nonatomic, readonly) NSInteger size;
@property (nonatomic, readonly) SkVPNKeyType type;

- (nullable NSData *)pemData:(NSError **)error;
- (nullable NSData *)derData:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
