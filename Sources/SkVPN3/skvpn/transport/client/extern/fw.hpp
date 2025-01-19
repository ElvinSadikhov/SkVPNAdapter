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

#ifndef SKVPN_TRANSPORT_CLIENT_EXTERN_FW_H
#define SKVPN_TRANSPORT_CLIENT_EXTERN_FW_H

#ifdef SKVPN_EXTERNAL_TRANSPORT_FACTORY
#include <skvpn/transport/client/transbase.hpp>
#endif

namespace skvpn {
  namespace ExternalTransport {
#ifdef SKVPN_EXTERNAL_TRANSPORT_FACTORY
    struct Config;
    struct Factory
    {
      virtual TransportClientFactory* new_transport_factory(const Config& conf) = 0;
      virtual ~Factory() {}
    };
#else
    struct Factory {};
#endif
  }
}
#endif
