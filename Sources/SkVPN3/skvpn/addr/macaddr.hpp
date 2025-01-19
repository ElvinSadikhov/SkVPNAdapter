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

#ifndef SKVPN_ADDR_MACADDR_H
#define SKVPN_ADDR_MACADDR_H

#include <ostream>
#include <cstring>
#include <string>

#include <skvpn/common/exception.hpp>
#include <skvpn/common/ostream.hpp>
#include <skvpn/common/hexstr.hpp>

namespace skvpn {

  // Fundamental class for representing an ethernet MAC address.

  class MACAddr {
  public:
    MACAddr()
    {
      std::memset(addr_, 0, sizeof(addr_));
    }

    MACAddr(const unsigned char *addr)
    {
      reset(addr);
    }

    void reset(const unsigned char *addr)
    {
      std::memcpy(addr_, addr, sizeof(addr_));
    }

    std::string to_string() const
    {
      return render_hex_sep(addr_, sizeof(addr_), ':');
    }

  private:
    unsigned char addr_[6];
  };

  SKVPN_OSTREAM(MACAddr, to_string)

} // namespace skvpn

#endif // SKVPN_ADDR_MACADDR_H
