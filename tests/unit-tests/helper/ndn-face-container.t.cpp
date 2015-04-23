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

#include "helper/ndn-face-container.hpp"

#include "ns3/node-container.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/node.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(HelperNdnFaceContainer, CleanupFixture)

BOOST_AUTO_TEST_CASE(AddAll)
{
 NodeContainer nodes;
 nodes.Create(4);

 PointToPointHelper p2p;
 p2p.Install(nodes.Get(0), nodes.Get(1));
 p2p.Install(nodes.Get(0), nodes.Get(2));
 p2p.Install(nodes.Get(0), nodes.Get(3));

 StackHelper ndnHelper;
 ndnHelper.SetDefaultRoutes(true);

 Ptr<FaceContainer> a = ndnHelper.InstallAll();
 FaceContainer b;
 b.AddAll(a);

 BOOST_CHECK_EQUAL_COLLECTIONS(a->Begin(), a->Begin() + a->GetN(),
                               b.Begin(), b.End());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
