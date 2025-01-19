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

#ifndef SKVPN_WIN_SLEEP_H
#define SKVPN_WIN_SLEEP_H

#include <windows.h>

#include <string>

#include <skvpn/common/action.hpp>
#include <skvpn/common/to_string.hpp>

namespace skvpn {

  class WinSleep : public Action
  {
  public:
    typedef RCPtr<WinSleep> Ptr;

    WinSleep(DWORD dwMilliseconds_arg)
      : dwMilliseconds(dwMilliseconds_arg)
    {
    }

    virtual void execute(std::ostream& os) override
    {
      os << to_string() << std::endl;
      ::Sleep(dwMilliseconds);
    }

    virtual std::string to_string() const override
    {
      return "Sleeping for " + skvpn::to_string(dwMilliseconds) + " milliseconds...";
    }

  private:
    DWORD dwMilliseconds;
  };

}
#endif
