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

// Low-level UDP transport object.

#ifndef SKVPN_TRANSPORT_UDPLINK_H
#define SKVPN_TRANSPORT_UDPLINK_H

#include <memory>

#include <skvpn/io/io.hpp>

#include <skvpn/common/size.hpp>
#include <skvpn/common/rc.hpp>
#include <skvpn/frame/frame.hpp>
#include <skvpn/log/sessionstats.hpp>

#ifdef SKVPN_GREMLIN
#include <skvpn/transport/gremlin.hpp>
#endif

#if defined(SKVPN_DEBUG_UDPLINK) && SKVPN_DEBUG_UDPLINK >= 1
#define SKVPN_LOG_UDPLINK_ERROR(x) SKVPN_LOG(x)
#else
#define SKVPN_LOG_UDPLINK_ERROR(x)
#endif

#if defined(SKVPN_DEBUG_UDPLINK) && SKVPN_DEBUG_UDPLINK >= 3
#define SKVPN_LOG_UDPLINK_VERBOSE(x) SKVPN_LOG(x)
#else
#define SKVPN_LOG_UDPLINK_VERBOSE(x)
#endif

namespace skvpn {
  namespace UDPTransport {

    typedef skvpn_io::ip::udp::endpoint AsioEndpoint;

    enum {
      SEND_SOCKET_HALTED=-1,
      SEND_PARTIAL=-2,
    };

    struct PacketFrom
    {
      typedef std::unique_ptr<PacketFrom> SPtr;
      BufferAllocated buf;
      AsioEndpoint sender_endpoint;
    };

    template <typename ReadHandler>
    class Link : public RC<thread_unsafe_refcount>
    {
    public:
      typedef RCPtr<Link> Ptr;

      Link(ReadHandler read_handler_arg,
	   skvpn_io::ip::udp::socket& socket_arg,
	   const Frame::Context& frame_context_arg,
	   const SessionStats::Ptr& stats_arg)
	: socket(socket_arg),
	  halt(false),
	  read_handler(read_handler_arg),
	  frame_context(frame_context_arg),
	  stats(stats_arg)
      {
      }

#ifdef SKVPN_GREMLIN
      void gremlin_config(const Gremlin::Config::Ptr& config)
      {
	if (config)
	  gremlin.reset(new Gremlin::SendRecvQueue(socket.get_executor().context(), config, false));
      }
#endif

      // Returns 0 on success, or a system error code on error.
      // May also return SEND_PARTIAL or SEND_SOCKET_HALTED.
      int send(const Buffer& buf, const AsioEndpoint* endpoint)
      {
#ifdef SKVPN_GREMLIN
	if (gremlin)
	  {
	    gremlin_send(buf, endpoint);
	    return 0;
	  }
	else
#endif
	return do_send(buf, endpoint);
      }

      void start(const int n_parallel)
      {
	if (!halt)
	  {
	    for (int i = 0; i < n_parallel; i++)
	      queue_read(nullptr);
	  }
      }

      void stop()
      {
	halt = true;
#ifdef SKVPN_GREMLIN
	if (gremlin)
	  gremlin->stop();
#endif
      }

      void reset_align_adjust(const size_t align_adjust)
      {
	frame_context.reset_align_adjust(align_adjust);
      }

      ~Link() { stop(); }

    private:
      void queue_read(PacketFrom *udpfrom)
      {
	SKVPN_LOG_UDPLINK_VERBOSE("UDPLink::queue_read");
	if (!udpfrom)
	  udpfrom = new PacketFrom();
	frame_context.prepare(udpfrom->buf);
	socket.async_receive_from(frame_context.mutable_buffer(udpfrom->buf),
				  udpfrom->sender_endpoint,
				  [self=Ptr(this), udpfrom=PacketFrom::SPtr(udpfrom)](const skvpn_io::error_code& error, const size_t bytes_recvd) mutable
                                  {
                                    SKVPN_ASYNC_HANDLER;
                                    self->handle_read(std::move(udpfrom), error, bytes_recvd);
                                  });
      }

      void handle_read(PacketFrom::SPtr pfp, const skvpn_io::error_code& error, const size_t bytes_recvd)
      {
	SKVPN_LOG_UDPLINK_VERBOSE("UDPLink::handle_read: " << error.message());
	if (!halt)
	  {
	    if (bytes_recvd)
	      {
		if (!error)
		  {
		    SKVPN_LOG_UDPLINK_VERBOSE("UDP[" << bytes_recvd << "] from " << pfp->sender_endpoint);
		    pfp->buf.set_size(bytes_recvd);
		    stats->inc_stat(SessionStats::BYTES_IN, bytes_recvd);
		    stats->inc_stat(SessionStats::PACKETS_IN, 1);
#ifdef SKVPN_GREMLIN
		    if (gremlin)
		      gremlin_recv(pfp);
		    else
#endif
		    read_handler->udp_read_handler(pfp);
		  }
		else
		  {
		    SKVPN_LOG_UDPLINK_ERROR("UDP recv error: " << error.message());
		    stats->error(Error::NETWORK_RECV_ERROR);
		  }
	      }
	    if (!halt)
	      queue_read(pfp.release()); // reuse PacketFrom object if still available
	  }
      }

      int do_send(const Buffer& buf, const AsioEndpoint* endpoint)
      {
	if (!halt)
	  {
	    try {
	      const size_t wrote = endpoint
		? socket.send_to(buf.const_buffer(), *endpoint)
		: socket.send(buf.const_buffer());
	      stats->inc_stat(SessionStats::BYTES_OUT, wrote);
	      stats->inc_stat(SessionStats::PACKETS_OUT, 1);
	      if (wrote == buf.size())
		return 0;
	      else
		{
		  SKVPN_LOG_UDPLINK_ERROR("UDP partial send error");
		  stats->error(Error::NETWORK_SEND_ERROR);
		  return SEND_PARTIAL;
		}
	    }
	    catch (skvpn_io::system_error& e)
	      {
		SKVPN_LOG_UDPLINK_ERROR("UDP send exception: " << e.what());
		stats->error(Error::NETWORK_SEND_ERROR);
		return e.code().value();
	      }
	  }
	else
	  return SEND_SOCKET_HALTED;
      }

#ifdef SKVPN_GREMLIN
      void gremlin_send(const Buffer& buf, const AsioEndpoint* endpoint)
      {
	std::unique_ptr<AsioEndpoint> ep;
	if (endpoint)
	  ep.reset(new AsioEndpoint(*endpoint));
	gremlin->send_queue([self=Ptr(this), buf=BufferAllocated(buf, 0), ep=std::move(ep)]() mutable {
	    if (!self->halt)
	      self->do_send(buf, ep.get());
	  });
      }

      void gremlin_recv(PacketFrom::SPtr& pfp)
      {
	gremlin->recv_queue([self=Ptr(this), pfp=std::move(pfp)]() mutable {
	    if (!self->halt)
	      self->read_handler->udp_read_handler(pfp);
	  });
      }
#endif

      skvpn_io::ip::udp::socket& socket;
      bool halt;
      ReadHandler read_handler;
      Frame::Context frame_context;
      SessionStats::Ptr stats;

#ifdef SKVPN_GREMLIN
      std::unique_ptr<Gremlin::SendRecvQueue> gremlin;
#endif
    };
  }
} // namespace skvpn

#endif
