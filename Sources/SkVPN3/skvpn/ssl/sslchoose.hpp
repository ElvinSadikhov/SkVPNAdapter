//    SkVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2012-2020 SkVPN Inc.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License Version 3
//    as published by the Free Software Foundation.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program in the COPYING file.
//    If not, see <http://www.gnu.org/licenses/>.

#ifndef SKVPN_SSL_SSLCHOOSE_H
#define SKVPN_SSL_SSLCHOOSE_H

#ifdef USE_OPENSSL
#include <skvpn/openssl/crypto/api.hpp>
#include <skvpn/openssl/ssl/sslctx.hpp>
#include <skvpn/openssl/util/rand.hpp>
#include <skvpn/openssl/util/pem.hpp>
#endif

#ifdef USE_APPLE_SSL
#include <skvpn/applecrypto/crypto/api.hpp>
#include <skvpn/applecrypto/ssl/sslctx.hpp>
#include <skvpn/applecrypto/util/rand.hpp>
#endif

#ifdef USE_MBEDTLS
#include <mbedtls/platform.h>
#include <mbedtls/debug.h>  // for debug_set_threshold
#include <skvpn/mbedtls/crypto/api.hpp>
#include <skvpn/mbedtls/ssl/sslctx.hpp>
#include <skvpn/mbedtls/util/rand.hpp>
#ifdef SKVPN_PLATFORM_UWP
#include <skvpn/mbedtls/util/uwprand.hpp>
#endif
#include <skvpn/mbedtls/util/pem.hpp>
#endif

#ifdef USE_MBEDTLS_APPLE_HYBRID
#include <skvpn/applecrypto/crypto/api.hpp>
#include <skvpn/mbedtls/ssl/sslctx.hpp>
#include <skvpn/mbedtls/util/rand.hpp>
#endif

namespace skvpn {
  namespace SSLLib {
#if defined(USE_MBEDTLS)
#define SSL_LIB_NAME "MbedTLS"
    typedef MbedTLSCryptoAPI CryptoAPI;
    typedef MbedTLSContext SSLAPI;
#if defined SKVPN_PLATFORM_UWP
    typedef MbedTLSRandomWithUWPEntropy RandomAPI;
#else
    typedef MbedTLSRandom RandomAPI;
#endif
    typedef MbedTLSPEM PEMAPI;
#elif defined(USE_MBEDTLS_APPLE_HYBRID)
    // Uses Apple framework for CryptoAPI and MbedTLS for SSLAPI and RandomAPI
#define SSL_LIB_NAME "MbedTLSAppleHybrid"
    typedef AppleCryptoAPI CryptoAPI;
    typedef MbedTLSContext SSLAPI;
    typedef MbedTLSRandom RandomAPI;
#elif defined(USE_APPLE_SSL)
#define SSL_LIB_NAME "AppleSSL"
    typedef AppleCryptoAPI CryptoAPI;
    typedef AppleSSLContext SSLAPI;
    typedef AppleRandom RandomAPI;
#elif defined(USE_OPENSSL)
#define SSL_LIB_NAME "OpenSSL"
    typedef OpenSSLCryptoAPI CryptoAPI;
    typedef OpenSSLContext SSLAPI;
    typedef OpenSSLRandom RandomAPI;
    typedef OpenSSLPEM PEMAPI;
#else
#error no SSL library defined
#endif
  }
}

#endif
