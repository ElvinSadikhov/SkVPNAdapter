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

#ifndef SKVPN_COMMON_PLATFORM_NAME_H
#define SKVPN_COMMON_PLATFORM_NAME_H

#include <skvpn/common/size.hpp>
#include <skvpn/common/platform.hpp>

namespace skvpn {

  // return a string that describes our platform
  inline const char *platform_name()
  {
#if defined(SKVPN_PLATFORM_WIN)
#if defined(SKVPN_PLATFORM_UWP)
    return "uwp";
#else
    return "win";
#endif // UWP
#elif defined(SKVPN_PLATFORM_MAC)
    return "mac";
#elif defined(SKVPN_PLATFORM_IPHONE)
    return "ios";
#elif defined(SKVPN_PLATFORM_IPHONE_SIMULATOR)
    return "iosim";
#elif defined(SKVPN_PLATFORM_ANDROID)
    return "android";
#elif defined(SKVPN_PLATFORM_LINUX)
    return "linux";
#else
    return nullptr;
#endif
  }

} // namespace skvpn

#endif // SKVPN_COMMON_PLATFORM_NAME_H
