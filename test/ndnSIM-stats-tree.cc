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
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"
#include "../utils/stats-tree.h"
#include "../apps/ndn-producer.h"

#include <boost/lexical_cast.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.StatsTreeTest");

namespace ns3 {

using namespace ndn;
using namespace ndn::ndnSIM;

void
StatsTreeTest::DoRun ()
{
  BasicTests ();
  SimpleScenario ();
}


void
StatsTreeTest::BasicTests ()
{
  StackHelper ndn;

  Ptr<Node> node1   = CreateObject<Node> ();
  Ptr<App> app1 = CreateObject<Producer> ();
  node1->AddApplication (app1);
  ndn.Install (node1);

  Ptr<Face> face1 = CreateObject<AppFace> (app1);
  Ptr<Face> face2 = CreateObject<AppFace> (app1);
  Ptr<Face> face3 = CreateObject<AppFace> (app1);

  node1->GetObject<L3Protocol> ()->AddFace (face1);
  node1->GetObject<L3Protocol> ()->AddFace (face2);
  node1->GetObject<L3Protocol> ()->AddFace (face3);

  // NS_LOG_DEBUG (*face1 << ", " << *face2 << ", " << *face3);
  
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

  // NS_LOG_DEBUG ("count:      " << node.incoming ().find (face1)->second.count ());
  // NS_LOG_DEBUG ("satisfied:  " << node.incoming ().find (face1)->second.satisfied ());
  // NS_LOG_DEBUG ("unsatisfied:" << node.incoming ().find (face1)->second.unsatisfied ());

  node.Step ();
  
  // NS_LOG_DEBUG ("count:      " << node.incoming ().find (face1)->second.count ());
  // NS_LOG_DEBUG ("satisfied:  " << node.incoming ().find (face1)->second.satisfied ());
  // NS_LOG_DEBUG ("unsatisfied:" << node.incoming ().find (face1)->second.unsatisfied ());
  
  LoadStats::stats_tuple tuple = node.incoming ().find (face1)->second.GetSatisfiedRatio ();
  // NS_LOG_DEBUG ("In, face1, satisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.667, 0.01, "Satisfied ratio should be ~ 2/3");
  NS_TEST_ASSERT_MSG_LT     (tuple.get<1> (), 0,           "Satisfied ratio should be less 0 (invalid)");
  NS_TEST_ASSERT_MSG_LT     (tuple.get<2> (), 0,           "Satisfied ratio should be less 0 (invalid)");
  
  tuple = node.incoming ().find (face1)->second.GetUnsatisfiedRatio ();
  // NS_LOG_DEBUG ("In, face1, unsatisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.333, 0.01, "Satisfied ratio should be ~ 1/3");
  NS_TEST_ASSERT_MSG_LT     (tuple.get<1> (), 0,           "Satisfied ratio should be less 0 (invalid)");
  NS_TEST_ASSERT_MSG_LT     (tuple.get<2> (), 0,           "Satisfied ratio should be less 0 (invalid)");

  node.AddIncoming (face1);
  node.Timeout ();
  node.Step ();

  // NS_LOG_DEBUG ("After decaying");
  
  tuple = node.incoming ().find (face1)->second.GetSatisfiedRatio ();
  // NS_LOG_DEBUG ("In, face1, satisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.473776, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<1> (), 0.489, 0.01, "");
  NS_TEST_ASSERT_MSG_LT     (tuple.get<2> (), 0,           "");
  
  tuple = node.incoming ().find (face1)->second.GetUnsatisfiedRatio ();
  // NS_LOG_DEBUG ("In, face1, unsatisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<0> (), 0.526, 0.01, "");
  NS_TEST_ASSERT_MSG_EQ_TOL (tuple.get<1> (), 0.504, 0.01, "");
  NS_TEST_ASSERT_MSG_LT     (tuple.get<2> (), 0,           "");  

  for (uint32_t i = 0; i < 10; i++ )
    node.Step ();

  // NS_LOG_DEBUG ("After more decaying");

  tuple = node.incoming ().find (face1)->second.GetSatisfiedRatio ();
  // NS_LOG_DEBUG ("In, face1, satisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_LT (tuple.get<0> (), 0, "");
  NS_TEST_ASSERT_MSG_LT (tuple.get<1> (), 0, "");
  NS_TEST_ASSERT_MSG_LT (tuple.get<2> (), 0, "");
  
  tuple = node.incoming ().find (face1)->second.GetUnsatisfiedRatio ();
  // NS_LOG_DEBUG ("In, face1, unsatisfied ratio: " << tuple.get<0> () << ", " << tuple.get<1> () << ", " << tuple.get<2> ());

  NS_TEST_ASSERT_MSG_LT (tuple.get<0> (), 0, "");
  NS_TEST_ASSERT_MSG_LT (tuple.get<1> (), 0, "");
  NS_TEST_ASSERT_MSG_LT (tuple.get<2> (), 0, "");

  /////////////////////////////////////////////////////
  //              Actual tree testing                //
  /////////////////////////////////////////////////////

  StatsTree tree;
  tree.NewPitEntry ("/bla/bla/bla");
  tree.NewPitEntry ("/bla/bla/bla");
  tree.NewPitEntry ("/bla/bla/bla");
  tree.NewPitEntry ("/foo/bar");
  tree.NewPitEntry ("/bar/foo");
  tree.NewPitEntry ("/tra/la/la");

  tree.Incoming ("/bla/bla/bla", face1);
  tree.Outgoing ("/foo/bar", face2);
  tree.Satisfy  ("/bar/foo");
  tree.Satisfy  ("/tra/la/la");
  tree.Timeout  ("/tra/la/la");

  tree.Step ();

  NS_TEST_ASSERT_MSG_EQ (boost::lexical_cast<std::string> (tree ["/"]),
                         // "PIT: 0.479734, 0.0991713, 0.0332409/0.159911, 0.0330571, 0.0110803/0.0799556, 0.0165285, 0.00554015",
                         "PIT: ration satisfied: 0.333333 0.333333 -1 / unsatisfied: 0.166667 0.166667 -1 ",
                         "Something wrong with stats tree");

  NS_TEST_ASSERT_MSG_NE (&tree ["/bla/bla/bla"],
                         &tree ["/"],
                         "The stats tree should not be empty");
  for (uint32_t i = 0; i < 9; i++)
    {
      tree.Step ();
    }
  NS_LOG_DEBUG (tree ["/bla/bla/bla"]);
  NS_LOG_DEBUG (tree ["/"]);
  NS_TEST_ASSERT_MSG_EQ (&tree ["/bla/bla/bla"],
                         &tree ["/"],
                         "The stats tree should be empty (only root node)");
}



void
StatsTreeTest::SimpleScenario ()
{
  NodeContainer nodes;
  nodes.Create (2);
  PointToPointHelper p2pHelper;
  p2pHelper.Install (nodes);


  
}

}
