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

#include "ns3/test.h"
#include "ns3/annotated-topology-reader.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/uinteger.h"
#include "ns3/random-variable.h"
#include <limits> 
#include "ns3/ccnx-header-helper.h"
#include "ns3/header.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/nstime.h"
#include "ns3/buffer.h"
#include "ns3/log.h"
#include "ns3/packet.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("InterestHeaderSerializationTest");

class InterestHeaderSerializationTest : public TestCase
{
public:
    InterestHeaderSerializationTest ();
    virtual ~InterestHeaderSerializationTest ();
    
private:
    virtual void DoRun (void);
};

InterestHeaderSerializationTest::InterestHeaderSerializationTest ()
: TestCase ("Interest Header Serialization Test")
{
}

InterestHeaderSerializationTest::~InterestHeaderSerializationTest ()
{
}

void
InterestHeaderSerializationTest::DoRun(void)
{
    Packet packet (0);
    
    uint32_t randomNonce = UniformVariable().GetInteger(1, std::numeric_limits<uint32_t>::max ());
    Ptr<CcnxNameComponents> testname = Create<CcnxNameComponents> ();
    (*testname) ("test") ("test2");
    
    Ptr<CcnxNameComponents> exclude = Create<CcnxNameComponents> ();
    (*exclude) ("exclude") ("exclude2");
    
    Time lifetime = Seconds(4.0);
    bool child = true;
    
    uint32_t maxSuffixComponents = 40;
    uint32_t minSuffixComponents = 20;
    
    CcnxInterestHeader interestHeader;
    interestHeader.SetNonce(randomNonce);
    interestHeader.SetName(testname);
    interestHeader.SetInterestLifetime(lifetime);
    interestHeader.SetChildSelector(child);
    interestHeader.SetExclude(exclude);
    interestHeader.SetMaxSuffixComponents(maxSuffixComponents);
    interestHeader.SetMinSuffixComponents(minSuffixComponents);

    //serialization
    packet.AddHeader (interestHeader);
	
	//deserialization
    CcnxInterestHeader target;
	packet.RemoveHeader (target);

    
    NS_TEST_ASSERT_MSG_EQ (target.GetNonce(), randomNonce, "Interest Header nonce deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.GetName(), *testname, "Interest Header name deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.GetInterestLifetime(), lifetime, "Interest Header lifetime deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.IsEnabledChildSelector(), child, "Interest Header childselector deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.GetExclude(), *exclude, "Interest Header exclude deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.GetMaxSuffixComponents(), (int)maxSuffixComponents, "Interest Header maxSuffixComponents deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.GetMinSuffixComponents(), (int)minSuffixComponents, "Interest Header minSuffixComponents deserialization failed");
}

class InterestHeaderSerializationTestSuite : public TestSuite
{
public:
    InterestHeaderSerializationTestSuite ();
};

InterestHeaderSerializationTestSuite::InterestHeaderSerializationTestSuite ()
: TestSuite ("interest-header-serialization-test-suite", UNIT)
{
    SetDataDir (NS_TEST_SOURCEDIR);
    AddTestCase (new InterestHeaderSerializationTest);
}

static InterestHeaderSerializationTestSuite suite;
