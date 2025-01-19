//
//  SkVPNError.h
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 11.02.17.
//
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString *const SkVPNAdapterErrorDomain;
FOUNDATION_EXPORT NSString *const SkVPNIdentityErrorDomain;

FOUNDATION_EXPORT NSString *const SkVPNAdapterErrorFatalKey;
FOUNDATION_EXPORT NSString *const SkVPNAdapterErrorMessageKey;

NS_ASSUME_NONNULL_END

/**
 SkVPN error codes
 */
typedef NS_ERROR_ENUM(SkVPNAdapterErrorDomain, SkVPNAdapterError) {
    SkVPNAdapterErrorConfigurationFailure = 1,
    SkVPNAdapterErrorCredentialsFailure,
    SkVPNAdapterErrorNetworkRecvError,
    SkVPNAdapterErrorNetworkEOFError,
    SkVPNAdapterErrorNetworkSendError,
    SkVPNAdapterErrorNetworkUnavailable,
    SkVPNAdapterErrorDecryptError,
    SkVPNAdapterErrorHMACError,
    SkVPNAdapterErrorReplayError,
    SkVPNAdapterErrorBufferError,
    SkVPNAdapterErrorCCError,
    SkVPNAdapterErrorBadSrcAddr,
    SkVPNAdapterErrorCompressError,
    SkVPNAdapterErrorResolveError,
    SkVPNAdapterErrorSocketSetupFailed,
    SkVPNAdapterErrorSocketProtectError,
    SkVPNAdapterErrorTUNReadError,
    SkVPNAdapterErrorTUNWriteError,
    SkVPNAdapterErrorTUNFramingError,
    SkVPNAdapterErrorTUNSetupFailed,
    SkVPNAdapterErrorTUNIfaceCreate,
    SkVPNAdapterErrorTUNIfaceDisabled,
    SkVPNAdapterErrorTUNError,
    SkVPNAdapterErrorTUNRegisterRingsError,
    SkVPNAdapterErrorTAPNotSupported,
    SkVPNAdapterErrorRerouteGatewayNoDns,
    SkVPNAdapterErrorTransportError,
    SkVPNAdapterErrorTCPOverflow,
    SkVPNAdapterErrorTCPSizeError,
    SkVPNAdapterErrorTCPConnectError,
    SkVPNAdapterErrorUDPConnectError,
    SkVPNAdapterErrorSSLError,
    SkVPNAdapterErrorSSLPartialWrite,
    SkVPNAdapterErrorSSLCaMdTooWeak,
    SkVPNAdapterErrorSSLCaKeyTooSmall,
    SkVPNAdapterErrorSSLDhKeyTooSmall,
    SkVPNAdapterErrorEncapsulationError,
    SkVPNAdapterErrorEPKICertError,
    SkVPNAdapterErrorEPKISignError,
    SkVPNAdapterErrorHandshakeTimeout,
    SkVPNAdapterErrorKeepaliveTimeout,
    SkVPNAdapterErrorInactiveTimeout,
    SkVPNAdapterErrorConnectionTimeout,
    SkVPNAdapterErrorPrimaryExpire,
    SkVPNAdapterErrorTLSVersionMin,
    SkVPNAdapterErrorTLSAuthFail,
    SkVPNAdapterErrorTLSCryptMetaFail,
    SkVPNAdapterErrorCertVerifyFail,
    SkVPNAdapterErrorPEMPasswordFail,
    SkVPNAdapterErrorAuthFailed,
    SkVPNAdapterErrorClientHalt,
    SkVPNAdapterErrorClientRestart,
    SkVPNAdapterErrorTUNHalt,
    SkVPNAdapterErrorRelay,
    SkVPNAdapterErrorRelayError,
    SkVPNAdapterErrorPauseNumber,
    SkVPNAdapterErrorReconnectNumber,
    SkVPNAdapterErrorKeyLimitRenegNumber,
    SkVPNAdapterErrorKeyStateError,
    SkVPNAdapterErrorProxyError,
    SkVPNAdapterErrorProxyNeedCreds,
    SkVPNAdapterErrorKevNegotiateError,
    SkVPNAdapterErrorKevPendingError,
    SkVPNAdapterErrorKevExpireNumber,
    SkVPNAdapterErrorKeyExpansionError,
    SkVPNAdapterErrorPKTIDInvalid,
    SkVPNAdapterErrorPKTIDBacktrack,
    SkVPNAdapterErrorPKTIDExpire,
    SkVPNAdapterErrorPKTIDReplay,
    SkVPNAdapterErrorPKTIDTimeBacktrack,
    SkVPNAdapterErrorDynamicChallenge,
    SkVPNAdapterErrorEPKIError,
    SkVPNAdapterErrorEPKIInvalidAlias,
    SkVPNAdapterErrorUnknown
};
