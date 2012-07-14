/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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
 */

#include "ndnSIM-stats-tree.h"
#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "../utils/stats-tree.h"
#include "../apps/ccnx-producer.h"

#include <boost/lexical_cast.hpp>

NS_LOG_COMPONENT_DEFINE ("CcnxStatsTreeTest");

using namespace ndnSIM;

namespace ns3
{

void
StatsTreeTest::DoRun ()
{
  CcnxStackHelper ccnx;

  Ptr<Node> node1   = CreateObject<Node> ();
  Ptr<CcnxApp> app1 = CreateObject<CcnxProducer> ();
  node1->AddApplication (app1);
  ccnx.Install (node1);

  Ptr<CcnxFace> face1 = CreateObject<CcnxAppFace> (app1);
  Ptr<CcnxFace> face2 = CreateObject<CcnxAppFace> (app1);
  Ptr<CcnxFace> face3 = CreateObject<CcnxAppFace> (app1);

  node1->GetObject<Ccnx> ()->AddFace (face1);
  node1->GetObject<Ccnx> ()->AddFace (face2);
  node1->GetObject<Ccnx> ()->AddFace (face3);

  NS_LOG_DEBUG (*face1 << ", " << *face2 << ", " << *face3);
  
  NS_TEST_ASSERT_MSG_NE (*face1, *face2, "Face1 should not be equal to Face2");
  NS_TEST_ASSERT_MSG_NE (face1, face2, "&Face1 should not be equal to &Face2");
  NS_TEST_ASSERT_MSG_NE (*face2, *face3, "Face2 should not be equal to Face3");
  NS_TEST_ASSERT_MSG_NE (face2, face3, "&Face2 should not be equal to &Face3");

  // hack
  face3->SetId (0);
  NS_TEST_ASSERT_MSG_EQ (*face1, *face3, "Face1 should be now equal to Face3");
  NS_TEST_ASSERT_MSG_NE (face1, face3, "&Face1 should not be equal to &Face3");

  LoadStatsNode::stats_container bla;
  bla[face1].Step (); 
  bla[face2].Step ();

  NS_TEST_ASSERT_MSG_EQ (bla.size (), 2, "Should be two entries in the container");

  bla[face3].Step ();
  NS_TEST_ASSERT_MSG_EQ (bla.size (), 2, "Should be still two entries in the container");

  LoadStatsNode node;
  node.AddIncoming (face1);
  node.AddIncoming (face1);
  node.AddIncoming (face2);
  node.AddIncoming (face3);

  NS_TEST_ASSERT_MSG_EQ (node.incoming ().size (), 2, "Incoming should have two entries again");
  NS_TEST_ASSERT_MSG_EQ (node.outgoing ().size (), 0, "Outgoing should have 0 entries");

  node.Satisfy ();
  node.Satisfy ();
  NS_TEST_ASSERT_MSG_EQ (node.incoming ().size (), 2, "Incoming should have two entries again");
  NS_TEST_ASSERT_MSG_EQ (node.outgoing ().size (), 0, "Outgoing should have 0 entries");

  node.Timeout ();
  NS_TEST_ASSERT_MSG_EQ (node.incoming ().size (), 2, "Incoming should have two entries again");
  NS_TEST_ASSERT_MSG_EQ (node.outgoing ().size (), 0, "Outgoing should have 0 entries");

  NS_LOG_DEBUG ("count:      " << node.incoming ().find (face1)->second.count ());
  NS_LOG_DEBUG ("satisfied:  " << node.incoming ().find (face1)->second.satisfied ());
  NS_LOG_DEBUG ("unsatisfied:" << node.incoming ().find (face1)->second.unsatisfied ());

  node.Step ();
  
  NS_LOG_DEBUG ("count:      " << node.incoming ().find (face1)->second.count ());
  NS_LOG_DEBUG ("satisfied:  " << node.incoming ().find (face1)->second.satisfied ());
  NS_LOG_DEBUG ("unsatisfied:" << node.incoming ().find (face1)->second.unsatisfied ());
  
  LoadStats::stats_tuple tuple = node.incoming ().find (face1)->second.GetSatisfiedRatio ();
  NS_LOG_DEBUG ("In, face1, satisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.667, 0.01, "Satisfied ratio should be ~ 2/3");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<1> (), 0.667, 0.01, "Satisfied ratio should be ~ 2/3");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<2> (), 0.667, 0.01, "Satisfied ratio should be ~ 2/3");
  
  tuple = node.incoming ().find (face1)->second.GetUnsatisfiedRatio ();
  NS_LOG_DEBUG ("In, face1, unsatisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.333, 0.01, "Satisfied ratio should be ~ 1/3");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<1> (), 0.333, 0.01, "Satisfied ratio should be ~ 1/3");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<2> (), 0.333, 0.01, "Satisfied ratio should be ~ 1/3");

  node.AddIncoming (face1);
  node.Timeout ();
  node.Step ();

  NS_LOG_DEBUG ("After decaying");
  
  tuple = node.incoming ().find (face1)->second.GetSatisfiedRatio ();
  NS_LOG_DEBUG ("In, face1, satisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.489, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<1> (), 0.489, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<2> (), 0.489, 0.01, "");
  
  tuple = node.incoming ().find (face1)->second.GetUnsatisfiedRatio ();
  NS_LOG_DEBUG ("In, face1, unsatisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.51, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<1> (), 0.51, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<2> (), 0.51, 0.01, "");  

  for (uint32_t i = 0; i < 50; i++ )
    node.Step ();

  NS_LOG_DEBUG ("After more decaying");

  tuple = node.incoming ().find (face1)->second.GetSatisfiedRatio ();
  NS_LOG_DEBUG ("In, face1, satisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.228, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<1> (), 0.047, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<2> (), 0.015, 0.01, "");
  
  tuple = node.incoming ().find (face1)->second.GetUnsatisfiedRatio ();
  NS_LOG_DEBUG ("In, face1, unsatisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.238, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<1> (), 0.049, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<2> (), 0.016, 0.01, "");

  /////////////////////////////////////////////////////
  //              Actual tree testing                //
  /////////////////////////////////////////////////////

  StatsTree tree;
  tree.NewPitEntry (CcnxNameComponents ("/bla/bla/bla"));
  tree.NewPitEntry (CcnxNameComponents ("/foo/bar"));
  tree.NewPitEntry (CcnxNameComponents ("/bar/foo"));
  tree.NewPitEntry (CcnxNameComponents ("/tra/la/la"));

  tree.Incoming (CcnxNameComponents ("/bla/bla/bla"), face1);
  tree.Outgoing (CcnxNameComponents ("/foo/bar"), face2);
  tree.Satisfy  (CcnxNameComponents ("/bar/foo"));
  tree.Satisfy  (CcnxNameComponents ("/tra/la/la"));

  tree.Step ();
  
  NS_LOG_DEBUG (tree);
}

}
