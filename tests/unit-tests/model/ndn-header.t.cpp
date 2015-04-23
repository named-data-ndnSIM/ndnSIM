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

#include "model/ndn-header.hpp"
#include "model/ndn-ns3.hpp"
#include "helper/ndn-stack-helper.hpp"

#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(ModelNdnHeader, CleanupFixture)

BOOST_AUTO_TEST_CASE(TypeId)
{
 auto interest = make_shared<ndn::Interest>("/prefix");
 PacketHeader<Interest> interestPktHeader(*interest);
 BOOST_CHECK_EQUAL(interestPktHeader.GetTypeId().GetName().c_str(), "ns3::ndn::Interest");

 auto data = make_shared<ndn::Data>();
 data->setFreshnessPeriod(ndn::time::milliseconds(1000));
 data->setContent(std::make_shared< ::ndn::Buffer>(1024));
 ndn::StackHelper::getKeyChain().sign(*data);
 PacketHeader<Data> dataPktHeader(*data);

 BOOST_CHECK_EQUAL(dataPktHeader.GetTypeId().GetName().c_str(), "ns3::ndn::Data");
 BOOST_CHECK_EQUAL(dataPktHeader.GetSerializedSize(), 1354); // 328 + 1024
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
