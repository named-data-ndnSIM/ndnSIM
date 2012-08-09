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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ndnSIM-serialization.h"

#include <boost/lexical_cast.hpp>

using namespace std;

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("NdnSimSerialization");

void
InterestSerializationTest::DoRun ()
{
  NdnInterestHeader source;
  source.SetName                (Create<NdnNameComponents> (boost::lexical_cast<NdnNameComponents> ("/test/test2")));
  NS_TEST_ASSERT_MSG_EQ (source.GetName (), boost::lexical_cast<NdnNameComponents> ("/test/test2"), "set/get name failed");
  
  source.SetMinSuffixComponents (20);
  NS_TEST_ASSERT_MSG_EQ (source.GetMinSuffixComponents (), 20, "set/get minSuffixComponents failed");
  
  source.SetMaxSuffixComponents (40);
  NS_TEST_ASSERT_MSG_EQ (source.GetMaxSuffixComponents (), 40, "set/get maxSuffixComponents failed");

  source.SetExclude (Create<NdnNameComponents> (boost::lexical_cast<NdnNameComponents> ("/exclude/exclude2")));
  NS_TEST_ASSERT_MSG_EQ (source.GetExclude (), boost::lexical_cast<NdnNameComponents> ("/exclude/exclude2"), "set/get exclude failed");

  source.SetChildSelector       (false);
  NS_TEST_ASSERT_MSG_EQ (source.IsEnabledChildSelector (), false, "set/get child selector failed");
  source.SetChildSelector       (true);
  NS_TEST_ASSERT_MSG_EQ (source.IsEnabledChildSelector (), true, "set/get child selector failed");

  source.SetAnswerOriginKind    (false);
  NS_TEST_ASSERT_MSG_EQ (source.IsEnabledAnswerOriginKind (), false, "set/get answer origin kind failed");
  source.SetAnswerOriginKind    (true);
  NS_TEST_ASSERT_MSG_EQ (source.IsEnabledAnswerOriginKind (), true, "set/get answer origin kind failed");

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
  NdnInterestHeader target;
  packet.RemoveHeader (target);
  
  NS_TEST_ASSERT_MSG_EQ (source.GetName ()                  , target.GetName ()                 , "source/target name failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetMinSuffixComponents ()   , target.GetMinSuffixComponents ()   , "source/target minSuffixComponents failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetMaxSuffixComponents ()   , target.GetMaxSuffixComponents ()   , "source/target maxSuffixComponents failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetExclude ()               , target.GetExclude ()               , "source/target exclude failed");
  NS_TEST_ASSERT_MSG_EQ (source.IsEnabledChildSelector ()   , target.IsEnabledChildSelector ()   , "source/target child selector failed");
  NS_TEST_ASSERT_MSG_EQ (source.IsEnabledAnswerOriginKind (), target.IsEnabledAnswerOriginKind (), "source/target answer origin kind failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetScope ()                 , target.GetScope ()                 , "source/target scope failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetInterestLifetime ()      , target.GetInterestLifetime ()      , "source/target interest lifetime failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetNonce ()                 , target.GetNonce ()                 , "source/target nonce failed");
  NS_TEST_ASSERT_MSG_EQ (source.GetNack ()                  , target.GetNack ()                  , "source/target NACK failed");
}

void
ContentObjectSerializationTest::DoRun ()
{
  // NS_TEST_ASSERT_MSG_EQ (true, false, "test not implemented yet");
}

}

