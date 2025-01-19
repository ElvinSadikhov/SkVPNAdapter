//
//  SkVPNPacketFlowBridge.h
//  SkVPN Adapter
//
//  Created by Jonathan Downing on 12/10/2017.
//  Modified by Sergey Abramchuk on 15/01/2018.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol SkVPNAdapterPacketFlow;

@interface SkVPNPacketFlowBridge: NSObject

@property (nonatomic, weak) id<SkVPNAdapterPacketFlow> packetFlow;

@property (nonatomic, readonly) CFSocketRef skVPNSocket;
@property (nonatomic, readonly) CFSocketRef packetFlowSocket;

- (BOOL)configureSocketsWithError:(NSError **)error;
- (void)invalidateSocketsIfNeeded;

- (void)startReading;

@end

NS_ASSUME_NONNULL_END
