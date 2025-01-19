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

#ifndef SKVPN_SERVER_PEERADDR_H
#define SKVPN_SERVER_PEERADDR_H

#include <cstdint> // for std::uint32_t, uint64_t, etc.

#include <skvpn/common/rc.hpp>
#include <skvpn/common/to_string.hpp>
#include <skvpn/common/jsonlib.hpp>
#include <skvpn/addr/ip.hpp>

namespace skvpn {
  struct AddrPort
  {
    AddrPort() : port(0) {}

    std::string to_string() const
    {
      return addr.to_string_bracket_ipv6() + ':' + skvpn::to_string(port);
    }

#ifdef HAVE_JSON
    Json::Value to_json() const
    {
      Json::Value jret(Json::objectValue);
      jret["addr"] = Json::Value(addr.to_string());
      jret["port"] = Json::Value(port);
      return jret;
    }
#endif

    IP::Addr addr;
    std::uint16_t port;
  };

  struct PeerAddr : public RCCopyable<thread_unsafe_refcount>
  {
    typedef RCPtr<PeerAddr> Ptr;

    PeerAddr()
      : tcp(false)
    {
    }

    std::string to_string() const
    {
      std::string proto;
      if (tcp)
	proto = "TCP ";
      else
	proto = "UDP ";
      return proto + remote.to_string() + " -> " + local.to_string();
    }

#ifdef HAVE_JSON
    Json::Value to_json() const
    {
      Json::Value jret(Json::objectValue);
      jret["tcp"] = Json::Value(tcp);
      jret["local"] = local.to_json();
      jret["remote"] = remote.to_json();
      return jret;
    }
#endif

    AddrPort remote;
    AddrPort local;
    bool tcp;
  };
}

#endif
