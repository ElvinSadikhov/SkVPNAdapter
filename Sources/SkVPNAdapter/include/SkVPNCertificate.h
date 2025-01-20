//
//  SkVPNCertificate.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 06.09.17.
//
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface SkVPNCertificate : NSObject

+ (nullable SkVPNCertificate *)certificateWithPEM:(NSData *)pemData error:(NSError **)error;
+ (nullable SkVPNCertificate *)certificateWithDER:(NSData *)derData error:(NSError **)error;

@property (readonly, nonatomic) NSInteger version;
@property (readonly, nonatomic) NSData *serial;

@property (readonly, nonatomic) NSData *issuer;
@property (readonly, nonatomic) NSData *subject;

- (instancetype) init NS_UNAVAILABLE;

- (nullable NSData *)pemData:(NSError **)error;
- (nullable NSData *)derData:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
