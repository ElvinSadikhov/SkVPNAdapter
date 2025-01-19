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

// Define simple logging macros that simply output to stdout

#ifndef SKVPN_LOG_LOGSIMPLE_H
#define SKVPN_LOG_LOGSIMPLE_H

#include <iostream>

#ifndef SKVPN_LOG_STREAM
#define SKVPN_LOG_STREAM std::cout
#endif

#define SKVPN_LOG(args) SKVPN_LOG_STREAM << args << std::endl

// like SKVPN_LOG but no trailing newline
#define SKVPN_LOG_NTNL(args) SKVPN_LOG_STREAM << args << std::flush

#define SKVPN_LOG_STRING(str) SKVPN_LOG_STREAM << (str) << std::flush

// no-op constructs normally used with logthread.hpp
namespace skvpn {
  namespace Log {
    struct Context
    {
      struct Wrapper {};
      Context(const Wrapper&) {}
    };
  }
}

#endif // SKVPN_LOG_LOGSIMPLE_H
