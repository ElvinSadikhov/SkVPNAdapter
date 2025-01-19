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

#ifndef SKVPN_BUFFER_ASIOBUF_H
#define SKVPN_BUFFER_ASIOBUF_H

#include <skvpn/io/io.hpp>

#include <skvpn/buffer/buffer.hpp>

namespace skvpn {
  class AsioConstBufferSeq2
  {
  public:
    AsioConstBufferSeq2(const Buffer& b1, const Buffer& b2)
      : buf{skvpn_io::const_buffer{b1.c_data(), b1.size()},
	     skvpn_io::const_buffer{b2.c_data(), b2.size()}}
    {
    }

    // Implement the ConstBufferSequence requirements.
    typedef skvpn_io::const_buffer value_type;
    typedef const skvpn_io::const_buffer* const_iterator;
    const skvpn_io::const_buffer* begin() const { return buf; }
    const skvpn_io::const_buffer* end() const { return buf + 2; }

    const size_t size() const
    {
      return skvpn_io::buffer_size(buf[0])
	   + skvpn_io::buffer_size(buf[1]);
    }

  private:
    const skvpn_io::const_buffer buf[2];
  };
}

#endif
