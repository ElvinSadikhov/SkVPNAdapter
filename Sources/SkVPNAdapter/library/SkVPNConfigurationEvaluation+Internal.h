//
//  SkVPNConfigurationEvaluation+Internal.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 26.04.17.
//
//

#import "SkVPNConfigurationEvaluation.h"

#include <ovpnapi.hpp>

using namespace skvpn;

@interface SkVPNConfigurationEvaluation (Internal)

- (instancetype)initWithEvalConfig:(ClientAPI::EvalConfig)eval;

@end
