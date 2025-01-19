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

#ifndef SKVPN_TRANSPORT_SOCKET_PROTECT_H
#define SKVPN_TRANSPORT_SOCKET_PROTECT_H

#include <skvpn/addr/ip.hpp>
#ifdef SKVPN_PLATFORM_UWP
#include <skvpn/transport/uwp_socket_protect.hpp>
#endif

namespace skvpn {
  // Used as an interface in cases where the high-level controlling app
  // needs early access to newly created transport sockets for making
  // property changes.  For example, on Android, we need to "protect"
  // the socket from being routed into the VPN tunnel.
  class BaseSocketProtect {
  public:
    virtual bool socket_protect(int socket, IP::Addr endpoint) = 0;
  };

#ifdef SKVPN_PLATFORM_UWP
  typedef UWPSocketProtect SocketProtect;
#else
  typedef BaseSocketProtect SocketProtect;
#endif
}

#endif
