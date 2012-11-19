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

using namespace std;

namespace ns3 {

using namespace ndn;

NS_LOG_COMPONENT_DEFINE ("ndn.Serialization");

void
InterestSerializationTest::DoRun ()
{
  InterestHeader source;
  
  source.SetName (Create<NameComponents> (boost::lexical_cast<NameComponents> ("/test/test2")));
  NS_TEST_ASSERT_MSG_EQ (source.GetName (), boost::lexical_cast<NameComponents> ("/test/test2"), "set/get name failed");
  
  source.SetScope (2);
  NS_TEST_ASSERT_MSG_EQ (source.GetScope (), 2, "set/get scope failed");

  source.SetInterestLifetime (Seconds (100));
  NS_TEST_ASSERT_MSG_EQ (source.GetInterestLifetime (), Seconds (100), "set/get interest lifetime failed");

  source.SetNonce (200);
  NS_TEST_ASSERT_MSG_EQ (source.GetNonce (), 200, "set/get nonce failed");

  source.SetNack (10);
  NS_TEST_ASSERT_MSG_EQ (source.GetNack (), 10, "set/get NACK failed");

  Packet packet (0);
  //serialization
  packet.AddHeader (source);
	
  //deserialization
  InterestHeader target;
  packet.RemoveHeader (target);
  
  NS_TEST_ASSERT_MSG_EQ (source.GetName ()            , target.GetName ()            , "source/target name failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetScope ()           , target.GetScope ()           , "source/target scope failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetInterestLifetime (), target.GetInterestLifetime (), "source/target interest lifetime failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetNonce ()           , target.GetNonce ()           , "source/target nonce failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetNack ()            , target.GetNack ()            , "source/target NACK failed");
}

void
ContentObjectSerializationTest::DoRun ()
{
  ContentObjectHeader source;
  
  source.SetName (Create<NameComponents> (boost::lexical_cast<NameComponents> ("/test/test2/1")));
  NS_TEST_ASSERT_MSG_EQ (source.GetName (), boost::lexical_cast<NameComponents> ("/test/test2/1"), "set/get name failed");
  
  source.SetFreshness (Seconds (10));
  NS_TEST_ASSERT_MSG_EQ (source.GetFreshness (), Seconds (10), "set/get freshness failed");

  source.SetTimestamp (Seconds (100));
  NS_TEST_ASSERT_MSG_EQ (source.GetTimestamp (), Seconds (100), "set/get timestamp failed");

  Packet packet (0);
  //serialization
  packet.AddHeader (source);
	
  //deserialization
  ContentObjectHeader target;
  packet.RemoveHeader (target);
  
  NS_TEST_ASSERT_MSG_EQ (source.GetName ()     , target.GetName ()     , "source/target name failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetFreshness (), target.GetFreshness (), "source/target freshness failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetTimestamp (), target.GetTimestamp (), "source/target timestamp failed");
}

}
