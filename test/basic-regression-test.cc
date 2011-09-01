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

#include <ctime>
#include <sstream>

#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/application.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/annotated-topology-reader.h"
#include <list>


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("CcnxBasicRegressionTest");

class BasicRegressionTest : public TestCase
{
public:
    
    BasicRegressionTest ();
    virtual ~BasicRegressionTest ();
    
private:
    virtual void DoRun (void);
};

BasicRegressionTest::BasicRegressionTest ()
: TestCase ("Basic regression test")
{
}

BasicRegressionTest::~BasicRegressionTest ()
{
}

void
BasicRegressionTest::DoRun(void)
{
    //string input ("/Users/iliamo/ns3-abstract-ndn/ns-3.11/src/NDNabstraction/examples/simpletopology.txt");
    
    // Set up command line parameters used to control the experiment.
    //CommandLine cmd;
    //cmd.AddValue ("input", "Name of the input file.",
    //              input);
    //cmd.Parse (argc, argv);
    
    
    // ------------------------------------------------------------
    // -- Read topology data.
    // --------------------------------------------
    
    string input = NS_TEST_SOURCEDIR;
    input += "/testtopology.txt";
    
    Ptr<AnnotatedTopologyReader> reader = CreateObject<AnnotatedTopologyReader> ();
    reader->SetFileName (input);
    
    NodeContainer nodes;
    if (reader != 0)
    {
        nodes = reader->Read ();
    }
    else
    {
        NS_TEST_ASSERT_MSG_EQ (true, false, "file not found");
    }
    
    NS_TEST_ASSERT_MSG_EQ (7, reader->LinksSize (), "link count is wrong");
    
    
    // ------------------------------------------------------------
    // -- Create nodes and network stacks
    // --------------------------------------------
    NS_LOG_INFO ("creating internet stack");
    InternetStackHelper stack;
    
    
    //routing
    /*Ipv4StaticRoutingHelper staticRouting;
    Ipv4ListRoutingHelper listRH;
    listRH.Add (staticRouting, 0);
    stack.SetRoutingHelper (listRH);  // has effect on the next Install ()
    stack.Install (nodes);
    
    NS_LOG_INFO ("creating ip4 addresses");
    Ipv4AddressHelper address;
    address.SetBase ("10.0.0.0", "255.255.255.252");*/
    
    int totlinks = reader->LinksSize ();
    
    
    /// applying settings
    NS_LOG_INFO ("creating node containers");
    NodeContainer* nc = new NodeContainer[totlinks];
    TopologyReader::ConstLinksIterator iter;
    int i = 0;
    for ( iter = reader->LinksBegin (); iter != reader->LinksEnd (); iter++, i++ )
    {
        nc[i] = NodeContainer (iter->GetFromNode (), iter->GetToNode ());
    }
    
    NetDeviceContainer* ndc = new NetDeviceContainer[totlinks];
    reader->ApplySettings(ndc,nc);
    /// settings applied
    
    
    
    
    // it creates little subnets, one for each couple of nodes.
    /*NS_LOG_INFO ("creating ipv4 interfaces");
    Ipv4InterfaceContainer* ipic = new Ipv4InterfaceContainer[totlinks];
    for (int i = 0; i < totlinks; i++)
    {
        ipic[i] = address.Assign (ndc[i]);
        address.NewNetwork ();
    }
    
    // ------------------------------------------------------------
    // -- Run the simulation
    // --------------------------------------------
    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop (Seconds (20));
    Simulator::Run ();
    Simulator::Destroy ();
    
    delete[] ipic;
    delete[] ndc;
    delete[] nc;
    
    NS_LOG_INFO ("Done.");
    
    */
}

class BasicRegressionTestSuite : public TestSuite
{
public:
    BasicRegressionTestSuite ();
};

BasicRegressionTestSuite::BasicRegressionTestSuite ()
: TestSuite ("ccnx-basic-regression-test-suite", UNIT)
{
    SetDataDir (NS_TEST_SOURCEDIR);
    AddTestCase (new BasicRegressionTest);
}

static BasicRegressionTestSuite suite;
