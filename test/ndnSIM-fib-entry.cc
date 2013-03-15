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

#include "ndnSIM-fib-entry.h"
#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/node-list.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "ns3/ndn-fib-entry.h"

NS_LOG_COMPONENT_DEFINE ("ndn.FibEntryTest");

namespace ns3
{

class Client : public ndn::App
{
protected:
  void
  StartApplication ()
  {
    ndn::App::StartApplication ();

    // add default route
    Ptr<ndn::fib::Entry> fibEntry = GetNode ()->GetObject<ndn::Fib> ()->Add (ndn::Name ("/"), m_face, 0);
    fibEntry->UpdateStatus (m_face, ndn::fib::FaceMetric::NDN_FIB_GREEN);

    Simulator::Schedule (Seconds (0.5), &Client::SendPacket, this, std::string("/1"), 1);
    Simulator::Schedule (Seconds (4.0), &Client::SendPacket, this, std::string("/2"), 1);
  }

  void
  StopApplication ()
  {
    ndn::App::StopApplication ();
  }

private:
  void
  SendPacket (const std::string &prefix, uint32_t nonce)
  {
    Ptr<Packet> pkt = Create<Packet> (0);
    ndn::Interest i;
    i.SetName (Create<ndn::Name> (prefix));
    i.SetNonce (nonce);
    i.SetInterestLifetime (Seconds (0.5));

    pkt->AddHeader (i);
    m_protocolHandler (pkt);
  }
};

struct StatusRecorder
{
  StatusRecorder (Ptr<Node> node, Ptr<ndn::fib::Entry> fibEntry, Ptr<ndn::Face> face)
    : m_node (node)
    , m_fibEntry (fibEntry)
    , m_face (face)
  {
    count = 0;
  }

  void
  StatusChange (ndn::fib::FaceMetric::Status oldStatus, ndn::fib::FaceMetric::Status newStatus)
  {
    count ++;
    // std::cout << Simulator::Now ().ToDouble (Time::S) << "s\tnode " << m_node->GetId () << " has changed fibEntry " << m_fibEntry->GetPrefix () << " face " << *m_face << " to " << newStatus << " from " << oldStatus << std::endl;
  }

  int count;

private:
  Ptr<Node> m_node;
  Ptr<ndn::fib::Entry> m_fibEntry;
  Ptr<ndn::Face> m_face;
};

void
FibEntryTest::DoRun ()
{
  Ptr<Node> node = CreateObject<Node> ();
  Ptr<Node> nodeSink = CreateObject<Node> ();
  PointToPointHelper p2p;
  p2p.Install (node, nodeSink);

  ndn::StackHelper ndn;
  ndn.SetForwardingStrategy ("ns3::ndn::fw::BestRoute");
  ndn.Install (node);
  ndn.Install (nodeSink);

  ndn::StackHelper::AddRoute (node, "/", 0, 0);

  Ptr<Client> app1 = CreateObject<Client> ();
  node->AddApplication (app1);

  ndn::AppHelper sinkHelper ("ns3::ndn::Producer");
  sinkHelper.SetPrefix ("/");
  sinkHelper.Install (nodeSink)
    .Stop (Seconds (2.0));

  std::list< boost::shared_ptr<StatusRecorder> > recorders;

  for (NodeList::Iterator anode = NodeList::Begin ();
       anode != NodeList::End ();
       anode ++)
    {
      Ptr<ndn::Fib> fib = (*anode)->GetObject<ndn::Fib> ();

      for (Ptr<ndn::fib::Entry> entry = fib->Begin ();
           entry != fib->End ();
           entry = fib->Next (entry))
        {
          BOOST_FOREACH (const ndn::fib::FaceMetric & faceMetric, entry->m_faces)
            {
              boost::shared_ptr<StatusRecorder> recorder = boost::make_shared<StatusRecorder> (*anode, entry, faceMetric.GetFace ());
              recorders.push_back (recorder);

              const_cast<ndn::fib::FaceMetric &> (faceMetric).GetStatusTrace ().ConnectWithoutContext (MakeCallback (&StatusRecorder::StatusChange, recorder.get ()));
            }
        }
    }

  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  NS_TEST_ASSERT_MSG_EQ (recorders.size (), 1, "only one recorder should be: only one real FIB record should have existed");
  NS_TEST_ASSERT_MSG_EQ (recorders.front ()->count, 2, "two events should have been reported");
}

}
