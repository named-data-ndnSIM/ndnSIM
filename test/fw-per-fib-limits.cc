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

#include "fw-per-fib-limits.h"
#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

#include <boost/lexical_cast.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.test.fw.PerFibLimits");

namespace ns3 {
namespace ndn {

void Decay (Ptr<fib::Entry> entry)
{
  entry->GetLimits ().DecayCurrentLimit ();
}


template<class T>
void
PrintTracedValue (std::string context, T oldValue, T newValue)
{
  NS_LOG_DEBUG (context << ": " <<
                oldValue << " => " << newValue);
}

void
Test1 (Ptr<fib::Entry> entry)
{
  entry->GetLimits ().IsBelowLimit ();
  entry->GetLimits ().DecreaseLimit ();
}

void
Test2 (Ptr<fib::Entry> entry)
{
  entry->GetLimits ().RemoveOutstanding ();
  for (uint32_t i = 0; i < 40; i++)
    entry->GetLimits ().IncreaseLimit ();
}

void
FwPerFibLimits::CheckCurMaxLimit (Ptr<fib::Entry> entry, double amount)
{
  NS_TEST_ASSERT_MSG_EQ_TOL ((double)entry->GetLimits ().m_curMaxLimit, amount, 0.1, "");
}

void
FwPerFibLimits::CheckOutstanding (Ptr<fib::Entry> entry, uint32_t amount)
{
  NS_TEST_ASSERT_MSG_EQ ((double)entry->GetLimits ().m_outstanding, amount, "");
}

// void 
// FwPerFibLimits::Check2 (Ptr<fib::Entry> entry)
// {
//   NS_TEST_ASSERT_MSG_EQ ((double)entry->GetLimits ().m_outstanding, 0, "");
// }

void
FwPerFibLimits::DoRun ()
{
  Simulator::Destroy ();

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper p2pHelper;
  p2pHelper.Install (nodes);
  
  StackHelper ndn;
  ndn.SetForwardingStrategy ("ns3::ndn::fw::PerFibLimits");
  ndn.Install (nodes);

  Ptr<Fib> fib = nodes.Get (0)->GetObject<Fib> ();
  ndn.AddRoute (nodes.Get (0), "/bla", 0, 10);

  Ptr<fib::Entry> entry = fib->Begin ();
  
  bool ok = entry->GetLimits ().TraceConnect ("CurMaxLimit", "fibEntry.curMax", MakeCallback (PrintTracedValue<double>));
  NS_TEST_ASSERT_MSG_EQ (ok, true, "");

  ok = entry->GetLimits ().TraceConnect ("Outstanding", "fibEntry.out", MakeCallback (PrintTracedValue<uint32_t>));
  NS_TEST_ASSERT_MSG_EQ (ok, true, "");

  ok = nodes.Get (0)->GetObject<L3Protocol> ()->GetFace (0)->GetLimits ()->TraceConnect ("CurMaxLimit", "face.curMax", MakeCallback (PrintTracedValue<double>));
  NS_TEST_ASSERT_MSG_EQ (ok, true, "");
  nodes.Get (0)->GetObject<L3Protocol> ()->GetFace (0)->GetLimits ()->TraceDisconnect ("CurMaxLimit", "face.curMax", MakeCallback (PrintTracedValue<double>));

  ok = nodes.Get (0)->GetObject<L3Protocol> ()->GetFace (0)->GetLimits ()->TraceConnect ("Outstanding", "face.out",    MakeCallback (PrintTracedValue<uint32_t>));
  NS_TEST_ASSERT_MSG_EQ (ok, true, "");
  nodes.Get (0)->GetObject<L3Protocol> ()->GetFace (0)->GetLimits ()->TraceDisconnect ("Outstanding", "face.out",    MakeCallback (PrintTracedValue<uint32_t>));

  Config::Connect ("/NodeList/0/$ns3::ndn::L3Protocol/FaceList/*/Limits/CurMaxLimit", MakeCallback (PrintTracedValue<double>));
  Config::Connect ("/NodeList/0/$ns3::ndn::L3Protocol/FaceList/*/Limits/Outstanding", MakeCallback (PrintTracedValue<uint32_t>));

  nodes.Get (0)->GetObject<L3Protocol> ()->GetFace (0)->GetLimits ()->SetMaxLimit (100);
  
  entry->GetLimits ().SetMaxLimit (100);
  NS_TEST_ASSERT_MSG_EQ_TOL ((double)entry->GetLimits ().m_curMaxLimit, 100, 0.1, "");
  
  entry->GetLimits ().DecreaseLimit ();
  NS_TEST_ASSERT_MSG_EQ_TOL ((double)entry->GetLimits ().m_curMaxLimit, 50, 0.1, "");

  entry = fib->Begin ();

  NS_LOG_DEBUG (entry);
  Simulator::Schedule (Seconds (0.1), Decay, entry);
  Simulator::Schedule (Seconds (25.0), Decay, entry);
  Simulator::Schedule (Seconds (28.0), Decay, entry);
  Simulator::Schedule (Seconds (40.0), Decay, entry);
  Simulator::Schedule (Seconds (60.0), Decay, entry);
  Simulator::Schedule (Seconds (100.0), Decay, entry);

  Simulator::Schedule (Seconds (100.1), &FwPerFibLimits::CheckCurMaxLimit, this, entry, 81.5);

  Simulator::Schedule (Seconds (100.5), &FwPerFibLimits::CheckOutstanding, this, entry, 0);
  Simulator::Schedule (Seconds (101.0), Test1, entry);
  Simulator::Schedule (Seconds (101.5), &FwPerFibLimits::CheckOutstanding, this, entry, 1);
  Simulator::Schedule (Seconds (101.5), &FwPerFibLimits::CheckCurMaxLimit, this, entry, 40.75);

  Simulator::Schedule (Seconds (102.0), Test2, entry);
  Simulator::Schedule (Seconds (102.5), &FwPerFibLimits::CheckOutstanding, this, entry, 0);
  Simulator::Schedule (Seconds (102.5), &FwPerFibLimits::CheckCurMaxLimit, this, entry, 41.75);

  AppHelper consumer ("ns3::ndn::ConsumerBatches");
  consumer.SetPrefix ("/bla");
  consumer.SetAttribute ("Batches", StringValue ("105 1"));
  consumer.SetAttribute ("LifeTime", StringValue ("1s"));
  consumer.Install (nodes.Get (0))
    .Stop (Seconds (110.0));

  Simulator::Schedule (Seconds (105.1), &FwPerFibLimits::CheckOutstanding, this, entry, 1);
  Simulator::Schedule (Seconds (106.7), &FwPerFibLimits::CheckOutstanding, this, entry, 0);

  Simulator::Schedule (Seconds (109.0), &FwPerFibLimits::CheckCurMaxLimit, this, entry, 14.27);
  Simulator::Schedule (Seconds (119.9), &FwPerFibLimits::CheckCurMaxLimit, this, entry, 16.32);
  
  Simulator::Stop (Seconds (120.0));
  Simulator::Run ();
 
  Simulator::Destroy ();
}

} // namespace ndn
} // namespace ns3
