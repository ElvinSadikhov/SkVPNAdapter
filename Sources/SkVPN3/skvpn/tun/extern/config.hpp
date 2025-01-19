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

#ifndef SKVPN_TUN_EXTERN_CONFIG_H
#define SKVPN_TUN_EXTERN_CONFIG_H

// These includes are also intended to resolve forward references in fw.hpp
#include <skvpn/common/options.hpp>
#include <skvpn/tun/client/tunbase.hpp>
#include <skvpn/tun/client/tunprop.hpp>
#include <skvpn/frame/frame.hpp>
#include <skvpn/log/sessionstats.hpp>
#include <skvpn/common/stop.hpp>

namespace skvpn {
  namespace ExternalTun {
    struct Config
    {
      TunProp::Config tun_prop;
      Frame::Ptr frame;
      SessionStats::Ptr stats;
      Stop* stop = nullptr;
      bool tun_persist = false;
    };
  }
}
#endif
