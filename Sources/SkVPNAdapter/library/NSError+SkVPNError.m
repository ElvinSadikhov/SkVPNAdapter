//
//  NSError+SkVPNError.m
//  SkVPN Adapter
//
//  Created by Sergey Abramchuk on 17.01.2018.
//

#import "NSError+SkVPNError.h"

#import <mbedtls/error.h>

#import "SkVPNError.h"

@implementation NSError (SkVPNAdapterErrorGeneration)

+ (NSError *)ovpn_errorObjectForAdapterError:(SkVPNAdapterError)adapterError
                                 description:(NSString *)description
                                     message:(NSString *)message
                                       fatal:(BOOL)fatal
{
    NSMutableDictionary *userInfo = [[NSMutableDictionary alloc] initWithDictionary:@{
        NSLocalizedDescriptionKey: description,
        SkVPNAdapterErrorFatalKey: @(fatal)
    }];
    
    NSString *errorReason = [NSError ovpn_reasonForAdapterError:adapterError];
    if (errorReason) {
        userInfo[NSLocalizedFailureReasonErrorKey] = errorReason;
    }
    
    if (message.length) {
        userInfo[SkVPNAdapterErrorMessageKey] = message;
    }
    
    return [NSError errorWithDomain:SkVPNAdapterErrorDomain code:adapterError userInfo:userInfo];
}

+ (SkVPNAdapterError)ovpn_adapterErrorByName:(NSString *)errorName {
    NSDictionary *errors = @{
        @"NETWORK_RECV_ERROR": @(SkVPNAdapterErrorNetworkRecvError),
        @"NETWORK_EOF_ERROR": @(SkVPNAdapterErrorNetworkEOFError),
        @"NETWORK_SEND_ERROR": @(SkVPNAdapterErrorNetworkSendError),
        @"NETWORK_UNAVAILABLE": @(SkVPNAdapterErrorNetworkUnavailable),
        @"DECRYPT_ERROR": @(SkVPNAdapterErrorDecryptError),
        @"HMAC_ERROR": @(SkVPNAdapterErrorDecryptError),
        @"REPLAY_ERROR": @(SkVPNAdapterErrorReplayError),
        @"BUFFER_ERROR": @(SkVPNAdapterErrorBufferError),
        @"CC_ERROR": @(SkVPNAdapterErrorCCError),
        @"BAD_SRC_ADDR": @(SkVPNAdapterErrorBadSrcAddr),
        @"COMPRESS_ERROR": @(SkVPNAdapterErrorCompressError),
        @"RESOLVE_ERROR": @(SkVPNAdapterErrorResolveError),
        @"SOCKET_PROTECT_ERROR": @(SkVPNAdapterErrorSocketProtectError),
        @"TUN_READ_ERROR": @(SkVPNAdapterErrorTUNReadError),
        @"TUN_WRITE_ERROR": @(SkVPNAdapterErrorTUNWriteError),
        @"TUN_FRAMING_ERROR": @(SkVPNAdapterErrorTUNFramingError),
        @"TUN_SETUP_FAILED": @(SkVPNAdapterErrorTUNSetupFailed),
        @"TUN_IFACE_CREATE": @(SkVPNAdapterErrorTUNIfaceCreate),
        @"TUN_IFACE_DISABLED": @(SkVPNAdapterErrorTUNIfaceDisabled),
        @"TUN_ERROR": @(SkVPNAdapterErrorTUNError),
        @"TUN_REGISTER_RINGS_ERROR": @(SkVPNAdapterErrorTUNRegisterRingsError),
        @"TAP_NOT_SUPPORTED": @(SkVPNAdapterErrorTAPNotSupported),
        @"REROUTE_GW_NO_DNS": @(SkVPNAdapterErrorRerouteGatewayNoDns),
        @"TRANSPORT_ERROR": @(SkVPNAdapterErrorTransportError),
        @"TCP_OVERFLOW": @(SkVPNAdapterErrorTCPOverflow),
        @"TCP_SIZE_ERROR": @(SkVPNAdapterErrorTCPSizeError),
        @"TCP_CONNECT_ERROR": @(SkVPNAdapterErrorTCPConnectError),
        @"UDP_CONNECT_ERROR": @(SkVPNAdapterErrorUDPConnectError),
        @"SSL_ERROR": @(SkVPNAdapterErrorSSLError),
        @"SSL_PARTIAL_WRITE": @(SkVPNAdapterErrorSSLPartialWrite),
        @"SSL_CA_MD_TOO_WEAK": @(SkVPNAdapterErrorSSLCaMdTooWeak),
        @"SSL_CA_KEY_TOO_SMALL": @(SkVPNAdapterErrorSSLCaKeyTooSmall),
        @"SSL_DH_KEY_TOO_SMALL": @(SkVPNAdapterErrorSSLDhKeyTooSmall),
        @"ENCAPSULATION_ERROR": @(SkVPNAdapterErrorEncapsulationError),
        @"EPKI_CERT_ERROR": @(SkVPNAdapterErrorEPKICertError),
        @"EPKI_SIGN_ERROR": @(SkVPNAdapterErrorEPKISignError),
        @"HANDSHAKE_TIMEOUT": @(SkVPNAdapterErrorHandshakeTimeout),
        @"KEEPALIVE_TIMEOUT": @(SkVPNAdapterErrorKeepaliveTimeout),
        @"INACTIVE_TIMEOUT": @(SkVPNAdapterErrorInactiveTimeout),
        @"CONNECTION_TIMEOUT": @(SkVPNAdapterErrorConnectionTimeout),
        @"PRIMARY_EXPIRE": @(SkVPNAdapterErrorPrimaryExpire),
        @"TLS_VERSION_MIN": @(SkVPNAdapterErrorTLSVersionMin),
        @"TLS_AUTH_FAIL": @(SkVPNAdapterErrorTLSAuthFail),
        @"TLS_CRYPT_META_FAIL": @(SkVPNAdapterErrorTLSCryptMetaFail),
        @"CERT_VERIFY_FAIL": @(SkVPNAdapterErrorCertVerifyFail),
        @"PEM_PASSWORD_FAIL": @(SkVPNAdapterErrorPEMPasswordFail),
        @"AUTH_FAILED": @(SkVPNAdapterErrorAuthFailed),
        @"CLIENT_HALT": @(SkVPNAdapterErrorClientHalt),
        @"CLIENT_RESTART": @(SkVPNAdapterErrorClientRestart),
        @"TUN_HALT": @(SkVPNAdapterErrorTUNHalt),
        @"RELAY": @(SkVPNAdapterErrorRelay),
        @"RELAY_ERROR": @(SkVPNAdapterErrorRelayError),
        @"N_PAUSE": @(SkVPNAdapterErrorPauseNumber),
        @"N_RECONNECT": @(SkVPNAdapterErrorReconnectNumber),
        @"N_KEY_LIMIT_RENEG": @(SkVPNAdapterErrorKeyLimitRenegNumber),
        @"KEY_STATE_ERROR": @(SkVPNAdapterErrorKeyStateError),
        @"PROXY_ERROR": @(SkVPNAdapterErrorProxyError),
        @"PROXY_NEED_CREDS": @(SkVPNAdapterErrorProxyNeedCreds),
        @"KEV_NEGOTIATE_ERROR": @(SkVPNAdapterErrorKevNegotiateError),
        @"KEV_PENDING_ERROR": @(SkVPNAdapterErrorKevPendingError),
        @"N_KEV_EXPIRE": @(SkVPNAdapterErrorKevExpireNumber),
        @"KEY_EXPANSION_ERROR": @(SkVPNAdapterErrorKeyExpansionError),
        @"PKTID_INVALID": @(SkVPNAdapterErrorPKTIDInvalid),
        @"PKTID_BACKTRACK": @(SkVPNAdapterErrorPKTIDBacktrack),
        @"PKTID_EXPIRE": @(SkVPNAdapterErrorPKTIDExpire),
        @"PKTID_REPLAY": @(SkVPNAdapterErrorPKTIDReplay),
        @"PKTID_TIME_BACKTRACK": @(SkVPNAdapterErrorPKTIDTimeBacktrack),
        @"DYNAMIC_CHALLENGE": @(SkVPNAdapterErrorDynamicChallenge),
        @"EPKI_ERROR": @(SkVPNAdapterErrorEPKIError),
        @"EPKI_INVALID_ALIAS": @(SkVPNAdapterErrorEPKIInvalidAlias)
    };
    
    SkVPNAdapterError error = errors[errorName] != nil ?
        (SkVPNAdapterError)[errors[errorName] integerValue] : SkVPNAdapterErrorUnknown;
    
    return error;
}

+ (NSString *)ovpn_reasonForAdapterError:(SkVPNAdapterError)error {
    switch (error) {
        case SkVPNAdapterErrorConfigurationFailure: return @"See SkVPN error message for more details.";
        case SkVPNAdapterErrorCredentialsFailure: return @"See SkVPN error message for more details.";
        case SkVPNAdapterErrorNetworkRecvError: return @"Errors receiving on network socket.";
        case SkVPNAdapterErrorNetworkEOFError: return @"EOF received on TCP network socket.";
        case SkVPNAdapterErrorNetworkSendError: return @"Errors sending on network socket";
        case SkVPNAdapterErrorNetworkUnavailable: return @"Network unavailable.";
        case SkVPNAdapterErrorDecryptError: return @"Data channel encrypt/decrypt error.";
        case SkVPNAdapterErrorHMACError: return @"HMAC verification failure.";
        case SkVPNAdapterErrorReplayError: return @"Error from PacketIDReceive.";
        case SkVPNAdapterErrorBufferError: return @"Exception thrown in Buffer methods.";
        case SkVPNAdapterErrorCCError: return @"General control channel errors.";
        case SkVPNAdapterErrorBadSrcAddr: return @"Packet from unknown source address.";
        case SkVPNAdapterErrorCompressError: return @"Compress/Decompress errors on data channel.";
        case SkVPNAdapterErrorResolveError: return @"DNS resolution error.";
        case SkVPNAdapterErrorSocketSetupFailed: return nil;
        case SkVPNAdapterErrorSocketProtectError: return @"Error calling protect() method on socket.";
        case SkVPNAdapterErrorTUNReadError: return @"Read errors on TUN/TAP interface.";
        case SkVPNAdapterErrorTUNWriteError: return @"Write errors on TUN/TAP interface.";
        case SkVPNAdapterErrorTUNFramingError: return @"Error with tun PF_INET/PF_INET6 prefix.";
        case SkVPNAdapterErrorTUNSetupFailed: return @"Error setting up TUN/TAP interface.";
        case SkVPNAdapterErrorTUNIfaceCreate: return @"Error creating TUN/TAP interface.";
        case SkVPNAdapterErrorTUNIfaceDisabled: return @"TUN/TAP interface is disabled.";
        case SkVPNAdapterErrorTUNError: return @"General tun error.";
        case SkVPNAdapterErrorTUNRegisterRingsError: return @"Error registering ring buffers with wintun.";
        case SkVPNAdapterErrorTAPNotSupported: return @"Dev TAP is present in profile but not supported.";
        case SkVPNAdapterErrorRerouteGatewayNoDns: return @"redirect-gateway specified without alt DNS servers.";
        case SkVPNAdapterErrorTransportError: return @"General transport error";
        case SkVPNAdapterErrorTCPOverflow: return @"TCP output queue overflow.";
        case SkVPNAdapterErrorTCPSizeError: return @"Bad embedded uint16_t TCP packet size.";
        case SkVPNAdapterErrorTCPConnectError: return @"Client error on TCP connect.";
        case SkVPNAdapterErrorUDPConnectError: return @"Client error on UDP connect.";
        case SkVPNAdapterErrorSSLError: return @"Errors resulting from read/write on SSL object.";
        case SkVPNAdapterErrorSSLPartialWrite: return @"SSL object did not process all written cleartext.";
        case SkVPNAdapterErrorSSLCaMdTooWeak: return @"CA message digest is too weak";
        case SkVPNAdapterErrorSSLCaKeyTooSmall: return @"CA key is too small";
        case SkVPNAdapterErrorSSLDhKeyTooSmall: return @"DH key is too small";
        case SkVPNAdapterErrorEncapsulationError: return @"Exceptions thrown during packet encapsulation.";
        case SkVPNAdapterErrorEPKICertError: return @"Error obtaining certificate from External PKI provider.";
        case SkVPNAdapterErrorEPKISignError: return @"Error obtaining RSA signature from External PKI provider.";
        case SkVPNAdapterErrorHandshakeTimeout: return @"Handshake failed to complete within given time frame.";
        case SkVPNAdapterErrorKeepaliveTimeout: return @"Lost contact with peer.";
        case SkVPNAdapterErrorInactiveTimeout: return @"Disconnected due to inactive timer.";
        case SkVPNAdapterErrorConnectionTimeout: return @"Connection failed to establish within given time.";
        case SkVPNAdapterErrorPrimaryExpire: return @"Primary key context expired.";
        case SkVPNAdapterErrorTLSVersionMin: return @"Peer cannot handshake at our minimum required TLS version.";
        case SkVPNAdapterErrorTLSAuthFail: return @"tls-auth HMAC verification failed.";
        case SkVPNAdapterErrorTLSCryptMetaFail: return @"tls-crypt-v2 metadata verification failed.";
        case SkVPNAdapterErrorCertVerifyFail: return @"Peer certificate verification failure.";
        case SkVPNAdapterErrorPEMPasswordFail: return @"Incorrect or missing PEM private key decryption password.";
        case SkVPNAdapterErrorAuthFailed: return @"General authentication failure";
        case SkVPNAdapterErrorClientHalt: return @"HALT message from server received.";
        case SkVPNAdapterErrorClientRestart: return @"RESTART message from server received.";
        case SkVPNAdapterErrorTUNHalt: return @"Halt command from tun interface";
        case SkVPNAdapterErrorRelay: return @"RELAY message from server received.";
        case SkVPNAdapterErrorRelayError: return @"RELAY error.";
        case SkVPNAdapterErrorPauseNumber: return nil;
        case SkVPNAdapterErrorReconnectNumber: return nil;
        case SkVPNAdapterErrorKeyLimitRenegNumber: return nil;
        case SkVPNAdapterErrorKeyStateError: return @"Received packet didn't match expected key state.";
        case SkVPNAdapterErrorProxyError: return @"HTTP proxy error.";
        case SkVPNAdapterErrorProxyNeedCreds: return @"HTTP proxy needs credentials.";
        case SkVPNAdapterErrorKevNegotiateError: return nil;
        case SkVPNAdapterErrorKevPendingError: return nil;
        case SkVPNAdapterErrorKevExpireNumber: return nil;
        case SkVPNAdapterErrorKeyExpansionError: return nil;
        case SkVPNAdapterErrorPKTIDInvalid: return nil;
        case SkVPNAdapterErrorPKTIDBacktrack: return nil;
        case SkVPNAdapterErrorPKTIDExpire: return nil;
        case SkVPNAdapterErrorPKTIDReplay: return nil;
        case SkVPNAdapterErrorPKTIDTimeBacktrack: return nil;
        case SkVPNAdapterErrorDynamicChallenge: return nil;
        case SkVPNAdapterErrorEPKIError: return nil;
        case SkVPNAdapterErrorEPKIInvalidAlias: return nil;
        case SkVPNAdapterErrorUnknown: return @"Unknown error.";
    }
}

@end

@implementation NSError (SkVPNMbedTLSErrorGeneration)

+ (NSError *)ovpn_errorObjectForMbedTLSError:(NSInteger)errorCode description:(NSString *)description {
    size_t length = 1024;
    char *buffer = malloc(length);
    
    mbedtls_strerror(errorCode, buffer, length);
    
    NSString *reason = [NSString stringWithUTF8String:buffer];
    
    free(buffer);
    
    return [NSError errorWithDomain:SkVPNIdentityErrorDomain code:errorCode userInfo:@{
        NSLocalizedDescriptionKey: description,
        NSLocalizedFailureReasonErrorKey: reason
    }];
}

@end
