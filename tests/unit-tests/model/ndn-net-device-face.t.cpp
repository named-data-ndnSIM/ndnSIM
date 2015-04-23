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


#include "model/ndn-net-device-face.hpp"
#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-ns3.hpp"
#include "NFD/daemon/face/face-counters.hpp"

#include <ndn-cxx/management/nfd-face-status.hpp>

#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/channel.h"
#include "ndn-face-container.hpp"
#include "ns3/node-container.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(ModelNdnNetDeviceFace, CleanupFixture)

BOOST_AUTO_TEST_CASE(SendInterest)
{
  NodeContainer nodes;
  nodes.Create(2);
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));

  StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);

  Ptr<FaceContainer> node0_faceContainer = ndnHelper.InstallAll();

  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue("100")); // 100 interests a second
  consumerHelper.Install(nodes.Get(0));

  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(1));

  FaceContainer::Iterator node1Face_iterator = node0_faceContainer->Begin() + 1;

  auto node1_netDeviceFace = std::dynamic_pointer_cast<NetDeviceFace>(node0_faceContainer->Get(node1Face_iterator));

  Simulator::Stop(Seconds(20.0));
  Simulator::Run();

  ::ndn::nfd::FaceStatus faceStatus = node1_netDeviceFace->getFaceStatus();
  BOOST_CHECK_EQUAL(faceStatus.getNInInterests(), 2000);
  Simulator::Destroy();
}

BOOST_AUTO_TEST_CASE(SendData)
{

 NodeContainer nodes;
 nodes.Create(2);
 PointToPointHelper p2p;
 p2p.Install(nodes.Get(0), nodes.Get(1));


 StackHelper ndnHelper;
 ndnHelper.SetDefaultRoutes(true);

 Ptr<FaceContainer> node0_faceContainer = ndnHelper.InstallAll();

 ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
 consumerHelper.SetPrefix("/prefix");
 consumerHelper.SetAttribute("Frequency", StringValue("10"));
 consumerHelper.Install(nodes.Get(0));

 ndn::AppHelper producerHelper("ns3::ndn::Producer");
 producerHelper.SetPrefix("/prefix");
 producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
 producerHelper.Install(nodes.Get(1));

 FaceContainer::Iterator node0Face_iterator = node0_faceContainer->Begin();
 FaceContainer::Iterator node1Face_iterator = node0_faceContainer->Begin() + 1;

 auto node0_netDeviceFace = std::dynamic_pointer_cast<NetDeviceFace>(node0_faceContainer->Get(node0Face_iterator));
 auto node1_netDeviceFace = std::dynamic_pointer_cast<NetDeviceFace>(node0_faceContainer->Get(node1Face_iterator));

 auto interest = std::make_shared<ndn::Interest>("/prefix");
 UniformVariable rand(0, std::numeric_limits<uint32_t>::max());
 interest->setNonce(rand.GetValue());
 interest->setInterestLifetime(ndn::time::seconds(1));

 auto data = std::make_shared<ndn::Data>(interest->getName());
 data->setFreshnessPeriod(ndn::time::milliseconds(1000));
 data->setContent(std::make_shared< ::ndn::Buffer>(1024));
 ndn::StackHelper::getKeyChain().sign(*data);

 node1_netDeviceFace->sendData(*data);

 Simulator::Stop(Seconds(20.0));
 Simulator::Run();

 ::ndn::nfd::FaceStatus node0faceStatus = node0_netDeviceFace->getFaceStatus();
 ::ndn::nfd::FaceStatus node1faceStatus = node1_netDeviceFace->getFaceStatus();
 BOOST_CHECK_EQUAL(node1faceStatus.getNOutDatas(), 201);
 Simulator::Destroy();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
