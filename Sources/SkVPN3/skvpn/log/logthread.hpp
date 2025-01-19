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

// This is a general-purpose logging framework that allows for SKVPN_LOG and
// SKVPN_LOG_NTNL macros to dispatch logging data to a thread-local handler.

// NOTE: define USE_ASIO_THREADLOCAL if your C++ doesn't support the
// "thread_local" attribute.

#ifndef SKVPN_LOG_LOGTHREAD_H
#define SKVPN_LOG_LOGTHREAD_H

#include <string>
#include <sstream>
#include <thread>

#if defined(USE_ASIO) && defined(USE_ASIO_THREADLOCAL)
#include <asio/detail/tss_ptr.hpp>
#endif

#include <skvpn/common/size.hpp>
#include <skvpn/common/extern.hpp>

// Define these parameters before including this header:

// SKVPN_LOG_CLASS -- client class that exposes a log() method
// SKVPN_LOG_INFO  -- converts a log string to the form that should be passed to log()

#ifndef SKVPN_LOG_CLASS
#error SKVPN_LOG_CLASS must be defined
#endif

#ifndef SKVPN_LOG_INFO
#error SKVPN_LOG_INFO must be defined
#endif

# define SKVPN_LOG(args) \
  do { \
    if (skvpn::Log::Context::defined()) {	\
      std::ostringstream _ovpn_log; \
      _ovpn_log << args << '\n'; \
      (skvpn::Log::Context::obj()->log(SKVPN_LOG_INFO(_ovpn_log.str()))); \
    } \
  } while (0)

// like SKVPN_LOG but no trailing newline
#define SKVPN_LOG_NTNL(args) \
  do { \
    if (skvpn::Log::Context::defined()) {	\
      std::ostringstream _ovpn_log; \
      _ovpn_log << args; \
      (skvpn::Log::Context::obj()->log(SKVPN_LOG_INFO(_ovpn_log.str()))); \
    } \
  } while (0)

# define SKVPN_LOG_STRING(str) \
  do { \
    if (skvpn::Log::Context::defined()) {			  \
      (skvpn::Log::Context::obj()->log(SKVPN_LOG_INFO(str))); \
    } \
  } while (0)

namespace skvpn {
  namespace Log {

#ifdef SKVPN_LOG_GLOBAL
    // SKVPN_LOG uses global object pointer
    SKVPN_EXTERN SKVPN_LOG_CLASS* global_log; // GLOBAL
    struct Context
    {
      struct Wrapper
      {
      };

      Context(const Wrapper& wrap)
      {
      }

      Context(SKVPN_LOG_CLASS *cli)
      {
	global_log = cli;
      }

      ~Context()
      {
	global_log = nullptr;
      }

      static bool defined()
      {
	return global_log != nullptr;
      }

      static SKVPN_LOG_CLASS* obj()
      {
	return global_log;
      }
    };
#else
    // SKVPN_LOG uses thread-local object pointer
#if defined(USE_ASIO) && defined(USE_ASIO_THREADLOCAL)
    SKVPN_EXTERN asio::detail::tss_ptr<SKVPN_LOG_CLASS> global_log; // GLOBAL
#else
    SKVPN_EXTERN thread_local SKVPN_LOG_CLASS* global_log; // GLOBAL
#endif
    struct Context
    {
      // Mechanism for passing thread-local
      // global_log to another thread.
      class Wrapper
      {
      public:
	Wrapper() : log(obj()) {}
      private:
	friend struct Context;
	SKVPN_LOG_CLASS *log;
      };

      // While in scope, turns on global_log
      // for this thread.
      Context(const Wrapper& wrap)
      {
	global_log = wrap.log;
      }

      Context(SKVPN_LOG_CLASS *cli)
      {
	global_log = cli;
      }

      ~Context()
      {
	global_log = nullptr;
      }

      static bool defined()
      {
	return global_log != nullptr;
      }

      static SKVPN_LOG_CLASS* obj()
      {
	return global_log;
      }
    };
#endif
  }
}

#endif
