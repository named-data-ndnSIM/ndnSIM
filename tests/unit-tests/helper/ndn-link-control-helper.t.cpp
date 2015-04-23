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

#include "helper/ndn-link-control-helper.hpp"
#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-net-device-face.hpp"
#include "model/ndn-app-face.hpp"
#include "NFD/daemon/fw/forwarder.hpp"

#include <ndn-cxx/management/nfd-face-status.hpp>

#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/channel.h"
#include "ns3/log.h"
#include "ns3/error-model.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/pointer.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(HelperNdnLinkControlHelper, CleanupFixture)

BOOST_AUTO_TEST_CASE(FailLink)
{
  NodeContainer nodes;
  nodes.Create(2);

  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));

  StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);

  Ptr<FaceContainer> node0_faceContainer = ndnHelper.InstallAll();

  AppHelper consumerHelper("ns3::ndn::ConsumerCbr");

  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue("1"));
  ApplicationContainer consumerAppContainer = consumerHelper.Install(nodes.Get(0));

  AppHelper producerHelper("ns3::ndn::Producer");

  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  ApplicationContainer producerAppContainer = producerHelper.Install(nodes.Get(1));

  FaceContainer::Iterator node2Face_it = node0_faceContainer->Begin() + 1;

  auto node1_netDeviceFace = std::dynamic_pointer_cast<NetDeviceFace>(node0_faceContainer->Get(node2Face_it));

  Simulator::Schedule(Seconds(0.0), ndn::LinkControlHelper::FailLink, nodes.Get(0), nodes.Get(1));
  Simulator::Schedule(Seconds(5.0), ndn::LinkControlHelper::UpLink, nodes.Get(0), nodes.Get(1));
  Simulator::Stop(Seconds(10.0));

  Simulator::Run();

  ::ndn::nfd::FaceStatus faceStatus = node1_netDeviceFace->getFaceStatus();
  BOOST_CHECK_EQUAL(faceStatus.getNInInterests(), 5);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
