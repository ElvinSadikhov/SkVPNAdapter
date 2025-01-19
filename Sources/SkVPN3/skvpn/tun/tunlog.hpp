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

// Define tun logging macros using SKVPN_DEBUG_TUN as a verbosity level.

#ifndef SKVPN_TUN_TUNLOG_H
#define SKVPN_TUN_TUNLOG_H

#if defined(SKVPN_DEBUG_TUN) && SKVPN_DEBUG_TUN >= 1
#define SKVPN_LOG_TUN_ERROR(x) SKVPN_LOG(x)
#else
#define SKVPN_LOG_TUN_ERROR(x)
#endif

#if defined(SKVPN_DEBUG_TUN) && SKVPN_DEBUG_TUN >= 2
#define SKVPN_LOG_TUN(x) SKVPN_LOG(x)
#else
#define SKVPN_LOG_TUN(x)
#endif

#if defined(SKVPN_DEBUG_TUN) && SKVPN_DEBUG_TUN >= 3
#define SKVPN_LOG_TUN_VERBOSE(x) SKVPN_LOG(x)
#else
#define SKVPN_LOG_TUN_VERBOSE(x)
#endif

#endif // SKVPN_TUN_TUNLOG_H
