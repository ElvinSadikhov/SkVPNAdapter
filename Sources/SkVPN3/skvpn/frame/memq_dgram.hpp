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

// A queue of datagram buffers for handling sequences of UDP packets

#ifndef SKVPN_FRAME_MEMQ_DGRAM_H
#define SKVPN_FRAME_MEMQ_DGRAM_H

#include <skvpn/common/size.hpp>
#include <skvpn/common/exception.hpp>
#include <skvpn/buffer/memq.hpp>
#include <skvpn/frame/frame.hpp>

namespace skvpn {

  class MemQDgram : public MemQBase {
  public:
    SKVPN_SIMPLE_EXCEPTION(frame_uninitialized);

    MemQDgram() {}
    explicit MemQDgram(const Frame::Ptr& frame) : frame_(frame) {}
    void set_frame(const Frame::Ptr& frame) { frame_ = frame; }

    size_t pending() const
    {
      return empty() ? 0 : q.front()->size();
    }

    void write(const unsigned char *data, size_t size)
    {
      if (frame_)
	{
	  const Frame::Context& fc = (*frame_)[Frame::READ_BIO_MEMQ_STREAM];
	  q.push_back(fc.copy(data, size));
	  length += size;
	}
      else
	throw frame_uninitialized();
    }

    size_t read(unsigned char *data, size_t len)
    {
      BufferPtr& b = q.front();
      if (len > b->size())
	len = b->size();
      b->read(data, len);
      if (b->empty())
	q.pop_front();
      length -= len;
      return len;
    }

  private:
    Frame::Ptr frame_;
  };

} // namespace skvpn

#endif // SKVPN_FRAME_MEMQ_DGRAM_H
