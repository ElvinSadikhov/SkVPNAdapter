//
//  NSArray+SkVPNAdditions.m
//  SkVPNAdapter
//
//  Created by Sergey Abramchuk on 16/10/2018.
//

#import "NSArray+SkVPNAdditions.h"

@implementation NSArray (SkVPNEmptyArray)

- (BOOL)ovpn_isNotEmpty {
    return (self.count > 0) ? YES : NO;
}

@end
