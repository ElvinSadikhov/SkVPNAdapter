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

// Select appropriate SkVPN protocol data channel implementation

#ifndef SKVPN_CRYPTO_CRYPTODCSEL_H
#define SKVPN_CRYPTO_CRYPTODCSEL_H

#include <skvpn/common/exception.hpp>
#include <skvpn/crypto/cryptodc.hpp>
#include <skvpn/crypto/crypto_chm.hpp>
#include <skvpn/crypto/crypto_aead.hpp>
#include <skvpn/random/randapi.hpp>

namespace skvpn {

  SKVPN_EXCEPTION(crypto_dc_select);

  template <typename CRYPTO_API>
  class CryptoDCSelect : public CryptoDCFactory
  {
  public:
    typedef RCPtr<CryptoDCSelect> Ptr;

    CryptoDCSelect(const Frame::Ptr& frame_arg,
		   const SessionStats::Ptr& stats_arg,
		   const RandomAPI::Ptr& prng_arg)
      : frame(frame_arg),
	stats(stats_arg),
	prng(prng_arg)
    {
    }

    virtual CryptoDCContext::Ptr new_obj(const CryptoAlgs::Type cipher,
					 const CryptoAlgs::Type digest,
					 const CryptoAlgs::KeyDerivation method)
    {
      const CryptoAlgs::Alg& alg = CryptoAlgs::get(cipher);
      if (alg.flags() & CryptoAlgs::CBC_HMAC)
	return new CryptoContextCHM<CRYPTO_API>(cipher, digest, method, frame, stats, prng);
      else if (alg.flags() & CryptoAlgs::AEAD)
	return new AEAD::CryptoContext<CRYPTO_API>(cipher, method, frame, stats);
      else
	SKVPN_THROW(crypto_dc_select, alg.name() << ": only CBC/HMAC and AEAD cipher modes supported");
    }

  private:
    Frame::Ptr frame;
    SessionStats::Ptr stats;
    RandomAPI::Ptr prng;
  };

}

#endif
