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

#ifndef SKVPN_ASIO_ASIOCONTEXT_H
#define SKVPN_ASIO_ASIOCONTEXT_H

#include <vector>
#include <memory>
#include <mutex>

#include <skvpn/io/io.hpp>

namespace skvpn {
  class AsioContextStore
  {
  public:
    skvpn_io::io_context& new_context(int concurrency_hint)
    {
      skvpn_io::io_context* ioc = new skvpn_io::io_context(concurrency_hint);
      {
	std::lock_guard<std::mutex> lock(mutex);
	contexts.emplace_back(ioc);
      }
      return *ioc;
    }

  private:
    std::mutex mutex;
    std::vector<std::unique_ptr<skvpn_io::io_context>> contexts;
  };
}

#endif
