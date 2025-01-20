//
//  SkVPNKeyType.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 07.09.17.
//
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, SkVPNKeyType) {
    SkVPNKeyTypeNone = 0,
    SkVPNKeyTypeRSA,
    SkVPNKeyTypeECKEY,
    SkVPNKeyTypeECKEYDH,
    SkVPNKeyTypeECDSA,
    SkVPNKeyTypeRSAALT,
    SkVPNKeyTypeRSASSAPSS,
};
