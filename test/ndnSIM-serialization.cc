/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ndnSIM-serialization.h"

#include <boost/lexical_cast.hpp>
#include "ns3/ndnSIM/model/wire/ndnsim.h"

using namespace std;

namespace ns3 {

using namespace ndn;

NS_LOG_COMPONENT_DEFINE ("ndn.Serialization");

void
InterestSerializationTest::DoRun ()
{
  Ptr<Interest> source = Create<Interest> ();
  
  source->SetName (Create<Name> (boost::lexical_cast<Name> ("/test/test2")));
  NS_TEST_ASSERT_MSG_EQ (source->GetName (), boost::lexical_cast<Name> ("/test/test2"), "set/get name failed");
  
  source->SetScope (2);
  NS_TEST_ASSERT_MSG_EQ (source->GetScope (), 2, "set/get scope failed");

  source->SetInterestLifetime (Seconds (100));
  NS_TEST_ASSERT_MSG_EQ (source->GetInterestLifetime (), Seconds (100), "set/get interest lifetime failed");

  source->SetNonce (200);
  NS_TEST_ASSERT_MSG_EQ (source->GetNonce (), 200, "set/get nonce failed");

  source->SetNack (10);
  NS_TEST_ASSERT_MSG_EQ (source->GetNack (), 10, "set/get NACK failed");

  NS_TEST_ASSERT_MSG_EQ (source->GetWire (), 0, "Wire should be empty");
  NS_TEST_ASSERT_MSG_NE (source->GetPayload (), 0, "Payload should not be empty");

  Ptr<Packet> packet = wire::ndnSIM::Interest::ToWire (source);

  NS_TEST_ASSERT_MSG_NE (source->GetWire (), 0, "Wire should not be empty now");

  Ptr<Interest> target = wire::ndnSIM::Interest::FromWire (packet);
  
  NS_TEST_ASSERT_MSG_EQ (source->GetName ()            , target->GetName ()            , "source/target name failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetScope ()           , target->GetScope ()           , "source/target scope failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetInterestLifetime (), target->GetInterestLifetime (), "source/target interest lifetime failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetNonce ()           , target->GetNonce ()           , "source/target nonce failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetNack ()            , target->GetNack ()            , "source/target NACK failed");

  NS_TEST_ASSERT_MSG_EQ (source->GetExclude ()         , 0, "exclude should be empty");
  NS_TEST_ASSERT_MSG_EQ (target->GetExclude ()         , 0, "exclude should be empty");
  
  Ptr<Exclude> exclude = Create<Exclude> ();
  exclude->excludeAfter (name::Component ());
  source->SetExclude (exclude);
  
  NS_TEST_ASSERT_MSG_EQ (boost::lexical_cast<std::string> (*source->GetExclude ()),
                         " ----> ", "exclude should contain only <ANY/>");

  exclude->appendExclude (name::Component ("alex"), false);
  exclude->excludeAfter (name::Component ("zhenkai"));

  source->SetExclude (exclude);  
  NS_TEST_ASSERT_MSG_EQ (boost::lexical_cast<std::string> (*source->GetExclude ()),
                         " ----> alex zhenkai ----> ", "exclude should contain only <ANY/>");

  NS_TEST_ASSERT_MSG_EQ (source->GetWire (), 0, "Wire should be empty");
  NS_TEST_ASSERT_MSG_NE (source->GetPayload (), 0, "Payload should not be empty");

  packet = wire::ndnSIM::Interest::ToWire (source);
  target = wire::ndnSIM::Interest::FromWire (packet);
  NS_TEST_ASSERT_MSG_NE (target->GetExclude (), 0, "exclude should not be empty");

  NS_TEST_ASSERT_MSG_EQ (boost::lexical_cast<std::string> (*target->GetExclude ()),
                         " ----> alex zhenkai ----> ", "exclude should contain only <ANY/>");
}

void
DataSerializationTest::DoRun ()
{
  Ptr<Data> source = Create<Data> (Create<Packet> (1024));
  
  source->SetName (Create<Name> (boost::lexical_cast<Name> ("/test/test2/1")));
  NS_TEST_ASSERT_MSG_EQ (source->GetName (), boost::lexical_cast<Name> ("/test/test2/1"), "set/get name failed");
  
  source->SetFreshness (Seconds (10));
  NS_TEST_ASSERT_MSG_EQ (source->GetFreshness (), Seconds (10), "set/get freshness failed");

  source->SetTimestamp (Seconds (100));
  NS_TEST_ASSERT_MSG_EQ (source->GetTimestamp (), Seconds (100), "set/get timestamp failed");

  NS_TEST_ASSERT_MSG_EQ (source->GetSignature (), 0, "initialization of signature failed");

  Ptr<Packet> packet = wire::ndnSIM::Data::ToWire (source);
  int size = packet->GetSize ();

  source->SetSignature (10);  
  NS_TEST_ASSERT_MSG_EQ (source->GetSignature (), 10, "set/get signature failed");

  packet = wire::ndnSIM::Data::ToWire (source);
  NS_TEST_ASSERT_MSG_EQ (packet->GetSize (), static_cast<unsigned int> (size + 4), "Signature size should have increased by 4");

  Ptr<Data> target = wire::ndnSIM::Data::FromWire (packet);
  
  NS_TEST_ASSERT_MSG_EQ (source->GetName ()     , target->GetName ()     , "source/target name failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetFreshness (), target->GetFreshness (), "source/target freshness failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetTimestamp (), target->GetTimestamp (), "source/target timestamp failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetSignature (), target->GetSignature (), "source/target signature failed");
}

}
