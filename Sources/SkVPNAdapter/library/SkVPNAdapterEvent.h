//
//  SkVPNAdapterEvent.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 05.11.16.
//  Copyright © 2016 ss-abramchuk. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 SkVPN event codes
 */
typedef NS_ENUM(NSInteger, SkVPNAdapterEvent) {
    SkVPNAdapterEventDisconnected,
    SkVPNAdapterEventConnected,
    SkVPNAdapterEventReconnecting,
    SkVPNAdapterEventAuthPending,
    SkVPNAdapterEventResolve,
    SkVPNAdapterEventWait,
    SkVPNAdapterEventWaitProxy,
    SkVPNAdapterEventConnecting,
    SkVPNAdapterEventGetConfig,
    SkVPNAdapterEventAssignIP,
    SkVPNAdapterEventAddRoutes,
    SkVPNAdapterEventEcho,
    SkVPNAdapterEventInfo,
    SkVPNAdapterEventWarn,
    SkVPNAdapterEventPause,
    SkVPNAdapterEventResume,
    SkVPNAdapterEventRelay,
    SkVPNAdapterEventCompressionEnabled,
    SkVPNAdapterEventUnsupportedFeature,
    SkVPNAdapterEventUnknown
};
