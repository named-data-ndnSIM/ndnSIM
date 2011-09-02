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
#include "ns3/ccnx-content-object-header.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("ContentObjectTest");

class ContentObjectTest : public TestCase
{
public:
    ContentObjectTest ();
    virtual ~ContentObjectTest ();
    
private:
    virtual void DoRun (void);
};

ContentObjectTest::ContentObjectTest ()
: TestCase ("Content Obejct Serialization Test")
{
}

ContentObjectTest::~ContentObjectTest ()
{
}

void
ContentObjectTest::DoRun(void)
{
    Packet::EnablePrinting ();
	Packet::EnableChecking (); 
    Packet packet (10);
	
    CcnxContentObjectHeader header;
	CcnxContentObjectTail   trailer;
	
    Ptr<CcnxNameComponents> testname = Create<CcnxNameComponents> ();
    (*testname) ("iwant")("icecream");
    header.SetName(testname);
    
	NS_LOG_INFO ("Source: \n" << header << trailer);
    
	packet.AddHeader (header);
	packet.AddTrailer (trailer);
    
	// NS_LOG_INFO ("Deserialized packet: \n" << packet);
    
	NS_LOG_INFO ("Removing and deserializing individual headers");
	
    CcnxContentObjectHeader targetHeader;
	CcnxContentObjectTail   targetTrailer;
    
	packet.RemoveHeader (targetHeader);
	packet.RemoveTrailer (targetTrailer);
    
    
    NS_TEST_ASSERT_MSG_EQ (targetHeader.GetName(), *testname, "Content Object name deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (packet.GetSize(), 10, "Content Object size inequality");
}

class ContentObjectTestSuite : public TestSuite
{
public:
    ContentObjectTestSuite ();
};

ContentObjectTestSuite::ContentObjectTestSuite ()
: TestSuite ("content-object-test-suite", UNIT)
{
    SetDataDir (NS_TEST_SOURCEDIR);
    AddTestCase (new ContentObjectTest);
}

static ContentObjectTestSuite suite;
