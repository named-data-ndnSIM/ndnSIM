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

#include "model/ndn-ns3.hpp"
#include "helper/ndn-stack-helper.hpp"
#include "model/ndn-header.hpp"
#include "utils/ndn-ns3-packet-tag.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/name.hpp>

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(ModelNdnNs3, CleanupFixture)

BOOST_AUTO_TEST_CASE(ToPacket)
{
  auto interest = make_shared<ndn::Interest>("/prefix");
  Ptr<Packet> interestPacket = Convert::ToPacket(*interest);
  uint32_t type1;
  type1 = Convert::getPacketType(interestPacket);

  BOOST_CHECK_EQUAL(type1, ::ndn::tlv::Interest);

  auto data = std::make_shared<ndn::Data>(interest->getName());
  data->setFreshnessPeriod(ndn::time::milliseconds(1000));
  data->setContent(std::make_shared< ::ndn::Buffer>(1024));
  ndn::StackHelper::getKeyChain().sign(*data);
  Ptr<Packet> DataPacket = Convert::ToPacket(*data);
  uint32_t type2;
  type2 = Convert::getPacketType(DataPacket);

  BOOST_CHECK_EQUAL(type2, ::ndn::tlv::Data);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
