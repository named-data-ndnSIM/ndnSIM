/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-block-header.hpp"

#include <iosfwd>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>

#include <ndn-cxx/encoding/tlv.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/lp/packet.hpp>

namespace io = boost::iostreams;
namespace nfdFace = nfd::face;

namespace ns3 {
namespace ndn {

ns3::TypeId
BlockHeader::GetTypeId()
{
  static ns3::TypeId tid =
    ns3::TypeId("ns3::ndn::Packet")
    .SetGroupName("Ndn")
    .SetParent<Header>()
    .AddConstructor<BlockHeader>()
    ;
  return tid;
}

TypeId
BlockHeader::GetInstanceTypeId(void) const
{
  return GetTypeId();
}

BlockHeader::BlockHeader()
{
}

BlockHeader::BlockHeader(const nfdFace::Transport::Packet& packet)
  : m_block(packet.packet)
{
}

uint32_t
BlockHeader::GetSerializedSize(void) const
{
  return m_block.size();
}

void
BlockHeader::Serialize(ns3::Buffer::Iterator start) const
{
  start.Write(m_block.wire(), m_block.size());
}

class Ns3BufferIteratorSource : public io::source {
public:
  Ns3BufferIteratorSource(ns3::Buffer::Iterator& is)
    : m_is(is)
  {
  }

  std::streamsize
  read(char* buf, std::streamsize nMaxRead)
  {
    std::streamsize i = 0;
    for (; i < nMaxRead && !m_is.IsEnd(); ++i) {
      buf[i] = m_is.ReadU8();
    }
    if (i == 0) {
      return -1;
    }
    else {
      return i;
    }
  }

private:
  ns3::Buffer::Iterator& m_is;
};

uint32_t
BlockHeader::Deserialize(ns3::Buffer::Iterator start)
{
  io::stream<Ns3BufferIteratorSource> is(start);
  m_block = ::ndn::Block::fromStream(is);
  return m_block.size();
}

void
BlockHeader::Print(std::ostream& os) const
{
  namespace tlv = ::ndn::tlv;
  namespace lp = ::ndn::lp;

  std::function<void(const Block& block)> decodeAndPrint = [&os, &decodeAndPrint] (const Block& block) {
    switch (block.type()) {
      case tlv::Interest: {
        Interest i(block);
        os << "Interest: " << i;
        break;
      }
      case tlv::Data: {
        Data d(block);
        os << "Data: " << d.getName();
        break;
      }
      case lp::tlv::LpPacket: {
        os << "NDNLP(";
        lp::Packet p(block);
        if (p.has<lp::FragCountField>() && p.get<lp::FragCountField>() != 1) {
          os << "fragment " << (p.get<lp::FragIndexField>() + 1) << " out of " << p.get<lp::FragCountField>();
        }
        else {
          if (p.has<lp::NackField>()) {
            lp::NackHeader nack = p.get<lp::NackField>();
            os << "NACK(" << nack.getReason() << ") for ";
          }

          ::ndn::Buffer::const_iterator first, last;
          std::tie(first, last) = p.get<lp::FragmentField>(0);
          Block fragmentBlock(&*first, std::distance(first, last));
          decodeAndPrint(fragmentBlock);
        }
        os << ")";
        break;
      }
      default: {
        os << "Unrecognized";
        break;
      }
    }
  };

  decodeAndPrint(m_block);
}

Block&
BlockHeader::getBlock()
{
  return m_block;
}

const Block&
BlockHeader::getBlock() const
{
  return m_block;
}

} // namespace ndn
} // namespace ns3
