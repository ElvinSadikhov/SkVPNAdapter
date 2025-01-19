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

#ifndef SKVPN_AUTH_AUTHCREDS
#define SKVPN_AUTH_AUTHCREDS

#include <utility> // for std::move
#include <string>

#include <skvpn/common/size.hpp>
#include <skvpn/common/exception.hpp>
#include <skvpn/common/rc.hpp>
#include <skvpn/common/options.hpp>
#include <skvpn/common/unicode.hpp>
#include <skvpn/buffer/safestr.hpp>
#include <skvpn/auth/validatecreds.hpp>

namespace skvpn {

    class AuthCreds : public RC<thread_unsafe_refcount>
    {
    public:
      typedef RCPtr<AuthCreds> Ptr;

      AuthCreds(std::string&& username_arg,
		SafeString&& password_arg,
		const std::string& peer_info_str)
	: username(std::move(username_arg)),
	  password(std::move(password_arg))
      {
	peer_info.parse_from_peer_info(peer_info_str, nullptr);
	peer_info.update_map();
      }

      // for unit test
      AuthCreds(std::string username_arg,
		SafeString password_arg,
		OptionList peer_info_arg)
	: username(std::move(username_arg)),
	  password(std::move(password_arg)),
	  peer_info(std::move(peer_info_arg))
      {
      }

      bool defined() const
      {
	return !username.empty();
      }

      bool is_valid_user_pass(const bool strict) const
      {
	return ValidateCreds::is_valid(ValidateCreds::USERNAME, username, strict)
	    && ValidateCreds::is_valid(ValidateCreds::PASSWORD, password, strict);
      }

      bool is_valid(const bool strict) const
      {
	return defined() && is_valid_user_pass(strict);
      }

      void wipe_password()
      {
	password.wipe();
      }

      std::string to_string() const
      {
	std::ostringstream os;
	os << "*** AuthCreds ***" << std::endl;
	os << "user: '" << username << "'" << std::endl;
	os << "pass: (" << password.length() << " chars)" << std::endl;
	os << "peer info:" << std::endl;
	os << peer_info.render(Option::RENDER_BRACKET|Option::RENDER_NUMBER);
	return os.str();
      }

      std::string username;
      SafeString password;
      OptionList peer_info;
    };

}

#endif
