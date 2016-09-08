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

#include "NFD/daemon/face/null-face.hpp"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(HelperNdnFaceContainer, CleanupFixture)

BOOST_AUTO_TEST_CASE(Basic)
{
  FaceContainer c1;
  BOOST_CHECK_EQUAL(c1.GetN(), 0);

  c1.Add(nfd::face::makeNullFace(FaceUri("null://1")));
  BOOST_CHECK_EQUAL(c1.GetN(), 1);

  c1.Add(nfd::face::makeNullFace(FaceUri("null://2")));
  BOOST_CHECK_EQUAL(c1.GetN(), 2);

  FaceContainer c2(c1);
  BOOST_CHECK_EQUAL(c2.GetN(), c1.GetN());

  FaceContainer c3;
  BOOST_CHECK_EQUAL(c3.GetN(), 0);

  c3 = c1;
  BOOST_CHECK_EQUAL(c3.GetN(), c1.GetN());

  for (size_t i = 0; i < c1.GetN(); ++i) {
    BOOST_CHECK_EQUAL(c1.Get(i)->getLocalUri(), c2.Get(i)->getLocalUri());
    BOOST_CHECK_EQUAL(c1.Get(i)->getLocalUri(), c3.Get(i)->getLocalUri());
  }

  size_t pos = 0;
  for (FaceContainer::Iterator i = c1.Begin(); i != c1.End(); ++i, ++pos) {
    BOOST_CHECK_EQUAL((*i)->getLocalUri(), c2.Get(pos)->getLocalUri());
    BOOST_CHECK_EQUAL((*i)->getLocalUri(), c3.Get(pos)->getLocalUri());
  }
}

BOOST_AUTO_TEST_CASE(AddAll)
{
  FaceContainer c1;
  c1.Add(nfd::face::makeNullFace(FaceUri("null://1")));
  c1.Add(nfd::face::makeNullFace(FaceUri("null://2")));

  FaceContainer c2(c1);
  c2.AddAll(c1);
  BOOST_CHECK_EQUAL(c2.GetN(), 4);

  FaceContainer c3(c1);
  c3.AddAll(c3);
  BOOST_CHECK_EQUAL(c3.GetN(), 4);

  Ptr<FaceContainer> c4 = Create<FaceContainer>(c1);
  c4->AddAll(c4);

  BOOST_CHECK_EQUAL_COLLECTIONS(c2.Begin(), c2.Begin() + c1.GetN(),
                                c1.Begin(), c1.End());

  BOOST_CHECK_EQUAL_COLLECTIONS(c2.Begin() + c1.GetN(), c2.End(),
                                c1.Begin(), c1.End());

  BOOST_CHECK_EQUAL_COLLECTIONS(c2.Begin(), c2.End(), c3.Begin(), c3.End());
  BOOST_CHECK_EQUAL_COLLECTIONS(c2.Begin(), c2.End(), c4->Begin(), c4->End());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
