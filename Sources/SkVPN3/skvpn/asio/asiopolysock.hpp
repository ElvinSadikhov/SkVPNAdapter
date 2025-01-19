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

// Asio polymorphic socket for handling TCP
// and unix domain sockets.

#ifndef SKVPN_ASIO_ASIOPOLYSOCK_H
#define SKVPN_ASIO_ASIOPOLYSOCK_H

#include <skvpn/io/io.hpp>

#include <skvpn/common/platform.hpp>
#include <skvpn/common/exception.hpp>
#include <skvpn/common/size.hpp>
#include <skvpn/common/rc.hpp>
#include <skvpn/common/function.hpp>
#include <skvpn/common/to_string.hpp>
#include <skvpn/common/sockopt.hpp>
#include <skvpn/addr/ip.hpp>

#if defined(SKVPN_POLYSOCK_SUPPORTS_ALT_ROUTING)
#include <skvpn/asio/alt_routing.hpp>
#elif defined(SKVPN_POLYSOCK_SUPPORTS_BIND)
#include <skvpn/asio/asioboundsock.hpp>
#endif

#ifdef ASIO_HAS_LOCAL_SOCKETS
#include <skvpn/common/peercred.hpp>
#endif

namespace skvpn {
  namespace AsioPolySock {
    // for shutdown()
    enum ShutdownFlags {
      SHUTDOWN_SEND = (1<<0),
      SHUTDOWN_RECV = (1<<1),
    };

    class Base : public RC<thread_unsafe_refcount>
    {
    public:
      typedef RCPtr<Base> Ptr;

      virtual void async_send(const skvpn_io::const_buffer& buf,
			      Function<void(const skvpn_io::error_code&, const size_t)>&& callback) = 0;

      virtual void async_receive(const skvpn_io::mutable_buffer& buf,
				 Function<void(const skvpn_io::error_code&, const size_t)>&& callback) = 0;

      virtual std::string remote_endpoint_str() const = 0;
      virtual bool remote_ip_port(IP::Addr& addr, unsigned int& port) const = 0;

      virtual void non_blocking(const bool state) = 0;

      virtual void close() = 0;

      virtual void shutdown(const unsigned int flags) {}

      virtual void tcp_nodelay() {}
      virtual void set_cloexec() {}

      virtual int native_handle()
      {
	return -1;
      }

#ifdef ASIO_HAS_LOCAL_SOCKETS
      virtual bool peercreds(SockOpt::Creds& cr)
      {
	return false;
      }
#endif

#if defined(SKVPN_POLYSOCK_SUPPORTS_ALT_ROUTING)
      virtual bool alt_routing_enabled()
      {
	return false;
      }
#endif

      virtual bool is_open() const = 0;
      virtual bool is_local() const = 0;

      size_t index() const { return index_; }

    protected:
      Base(const size_t index)
	: index_(index)
      {
      }

    private:
      size_t index_;
    };

    struct TCP : public Base
    {
      typedef RCPtr<TCP> Ptr;

      TCP(skvpn_io::io_context& io_context,
	  const size_t index)
	:  Base(index),
	   socket(io_context)
      {
      }

      virtual void async_send(const skvpn_io::const_buffer& buf,
			      Function<void(const skvpn_io::error_code&, const size_t)>&& callback) override
      {
	socket.async_send(buf, std::move(callback));
      }

      virtual void async_receive(const skvpn_io::mutable_buffer& buf,
				 Function<void(const skvpn_io::error_code&, const size_t)>&& callback) override
      {
	socket.async_receive(buf, std::move(callback));
      }

#if !defined(SKVPN_POLYSOCK_SUPPORTS_ALT_ROUTING)
      virtual std::string remote_endpoint_str() const override
      {
	try {
	  return "TCP " + skvpn::to_string(socket.remote_endpoint());
	}
	catch (const std::exception&)
	  {
	    return "TCP";
	  }
      }
#endif

      virtual bool remote_ip_port(IP::Addr& addr, unsigned int& port) const override
      {
	try {
	  addr = IP::Addr::from_asio(socket.remote_endpoint().address());
	  port = socket.remote_endpoint().port();
	  return true;
	}
	catch (const std::exception&)
	  {
	    return false;
	  }
      }

      virtual void non_blocking(const bool state) override
      {
	socket.non_blocking(state);
      }

      virtual void tcp_nodelay() override
      {
	socket.set_option(skvpn_io::ip::tcp::no_delay(true));
      }

#if !defined(SKVPN_PLATFORM_WIN)
      virtual void set_cloexec() override
      {
	const int fd = socket.native_handle();
	if (fd >= 0)
	  SockOpt::set_cloexec(fd);
      }
#endif

      virtual void shutdown(const unsigned int flags) override
      {
	if (flags & SHUTDOWN_SEND)
	  socket.shutdown(skvpn_io::ip::tcp::socket::shutdown_send);
	else if (flags & SHUTDOWN_RECV)
	  socket.shutdown(skvpn_io::ip::tcp::socket::shutdown_receive);
      }

      virtual void close() override
      {
	socket.close();
      }

      virtual bool is_open() const override
      {
	return socket.is_open();
      }

      virtual bool is_local() const override
      {
	return false;
      }

      virtual int native_handle() override
      {
	return socket.native_handle();
      }

#if defined(SKVPN_POLYSOCK_SUPPORTS_ALT_ROUTING)
      virtual std::string remote_endpoint_str() const override
      {
	const char *proto = (socket.alt_routing_enabled() ? "TCP ALT " : "TCP ");
	return proto + socket.to_string();
      }

      virtual bool alt_routing_enabled() override
      {
	return socket.alt_routing_enabled();
      }

      AltRouting::Socket socket;
#elif defined(SKVPN_POLYSOCK_SUPPORTS_BIND)
      AsioBoundSocket::Socket socket;
#else
      skvpn_io::ip::tcp::socket socket;
#endif
    };

#ifdef ASIO_HAS_LOCAL_SOCKETS
    struct Unix : public Base
    {
      typedef RCPtr<Unix> Ptr;

      Unix(skvpn_io::io_context& io_context,
	   const size_t index)
	:  Base(index),
	   socket(io_context)
      {
      }

      virtual void async_send(const skvpn_io::const_buffer& buf,
			      Function<void(const skvpn_io::error_code&, const size_t)>&& callback) override
      {
	socket.async_send(buf, std::move(callback));
      }

      virtual void async_receive(const skvpn_io::mutable_buffer& buf,
				 Function<void(const skvpn_io::error_code&, const size_t)>&& callback) override
      {
	socket.async_receive(buf, std::move(callback));
      }

      virtual std::string remote_endpoint_str() const override
      {
	return "LOCAL";
      }

      virtual bool remote_ip_port(IP::Addr&, unsigned int&) const override
      {
	return false;
      }

      virtual void non_blocking(const bool state) override
      {
	socket.non_blocking(state);
      }

      virtual bool peercreds(SockOpt::Creds& cr) override
      {
	return SockOpt::peercreds(socket.native_handle(), cr);
      }

      virtual void set_cloexec() override
      {
	const int fd = socket.native_handle();
	if (fd >= 0)
	  SockOpt::set_cloexec(fd);
      }

#if !defined(SKVPN_PLATFORM_MAC)
      // shutdown() throws "socket is not connected" exception
      // on macos if another side has called close() - this behavior
      // breaks communication with agent, and hence disabled
      virtual void shutdown(const unsigned int flags) override
      {
	if (flags & SHUTDOWN_SEND)
	  socket.shutdown(skvpn_io::ip::tcp::socket::shutdown_send);
	else if (flags & SHUTDOWN_RECV)
	  socket.shutdown(skvpn_io::ip::tcp::socket::shutdown_receive);
      }
#endif

      virtual void close() override
      {
	socket.close();
      }

      virtual bool is_open() const override
      {
	return socket.is_open();
      }

      virtual bool is_local() const override
      {
	return true;
      }

      virtual int native_handle() override
      {
	return socket.native_handle();
      }

      skvpn_io::local::stream_protocol::socket socket;
    };
#endif

#if defined(SKVPN_PLATFORM_WIN)
    struct NamedPipe : public Base
    {
      typedef RCPtr<NamedPipe> Ptr;

      NamedPipe(skvpn_io::windows::stream_handle&& handle_arg,
		const size_t index)
	:  Base(index),
	   handle(std::move(handle_arg))
      {
      }

      virtual void async_send(const skvpn_io::const_buffer& buf,
			      Function<void(const skvpn_io::error_code&, const size_t)>&& callback) override
      {
	handle.async_write_some(buf, std::move(callback));
      }

      virtual void async_receive(const skvpn_io::mutable_buffer& buf,
				 Function<void(const skvpn_io::error_code&, const size_t)>&& callback) override
      {
	handle.async_read_some(buf, std::move(callback));
      }

      virtual std::string remote_endpoint_str() const override
      {
	return "NAMED_PIPE";
      }

      virtual bool remote_ip_port(IP::Addr&, unsigned int&) const override
      {
	return false;
      }

      virtual void non_blocking(const bool state) override
      {
      }

      virtual void close() override
      {
	handle.close();
      }

      virtual bool is_open() const override
      {
	return handle.is_open();
      }

      virtual bool is_local() const override
      {
	return true;
      }

      skvpn_io::windows::stream_handle handle;
    };
#endif
  }
}

#endif
