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

// registry utilities for Windows

#ifndef SKVPN_WIN_REG_H
#define SKVPN_WIN_REG_H

#include <windows.h>
#include <skvpn/win/winerr.hpp>
#include <skvpn/common/size.hpp>

namespace skvpn {
  namespace Win {

    template<typename E>
    static void check_reg_error(DWORD status, const std::string& key)
      {
	if (status != ERROR_SUCCESS)
	  {
	    const Win::Error err(status);
	    SKVPN_THROW(E, "registry key " << key << " error: " << err.message());
	  }
      }

    // HKEY wrapper
    class RegKey
    {
      RegKey(const RegKey&) = delete;
      RegKey& operator=(const RegKey&) = delete;

    public:
      RegKey() : key(nullptr) {}
      bool defined() const { return key != nullptr; }
      HKEY* ref() { return &key; }
      HKEY operator()() { return key; }

      ~RegKey()
      {
	if (defined())
	  ::RegCloseKey(key);
      }
    private:
      HKEY key;
    };

    class RegKeyEnumerator : public std::vector<std::string>
    {
    public:
      RegKeyEnumerator(HKEY hkey, const std::string& path)
      {
	RegKey regKey;
	auto status = ::RegOpenKeyExA(hkey,
				      path.c_str(),
				      0,
				      KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS,
				      regKey.ref());
	if (status != ERROR_SUCCESS)
	    return;

	DWORD subkeys_num;
	status = ::RegQueryInfoKeyA(regKey(),
				    nullptr,
				    nullptr,
				    NULL,
				    &subkeys_num,
				    nullptr,
				    nullptr,
				    nullptr,
				    nullptr,
				    nullptr,
				    nullptr,
				    nullptr);

	if (status != ERROR_SUCCESS)
	  return;

	const int MAX_KEY_LENGTH = 255;
	for (DWORD i = 0; i < subkeys_num; ++ i)
	  {
	    DWORD subkey_size = MAX_KEY_LENGTH;
	    char subkey[MAX_KEY_LENGTH];
	    status = ::RegEnumKeyExA(regKey(),
				     i,
				     subkey,
				     &subkey_size,
				     nullptr,
				     nullptr,
				     nullptr,
				     nullptr);
	    if (status == ERROR_SUCCESS)
	      push_back(subkey);
	  }
      }
    };
  }
}

#endif
