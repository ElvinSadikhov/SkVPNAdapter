//    SkVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2012-2017 SkVPN Technologies, Inc.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License Version 3
//    as published by the Free Software Foundation.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program in the COPYING file.
//    If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <string>

#include <skvpn/common/to_string.hpp>

namespace skvpn {

  template <class EPRANGE>
  inline std::string asio_resolver_results_to_string(const EPRANGE& endpoint_range)
  {
    std::string ret;
    ret.reserve(64);
    bool first = true;
    for (const auto &i : endpoint_range)
      {
	if (!first)
	  ret += ' ';
	ret += '[';
	ret += skvpn::to_string(i.endpoint().address());
	ret += "]:";
	ret += skvpn::to_string(i.endpoint().port());
	first = false;
      }
    return ret;
  }

}
