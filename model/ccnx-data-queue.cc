/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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

#include "ccnx-data-queue.h"

#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/ccnx-face.h"

#include <algorithm>

namespace ns3
{

TypeId
CcnxDataQueue::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxDataQueue")
    .SetParent<Ccnx> ()
    .SetGroupName ("Ccnx")
    .AddConstructor<CcnxDataQueue> ()
    
    .AddAttribute ("MaxDelay", "Maximum delay that packets should wait in queue (max for uniform random variable)",
                   TimeValue (Seconds (2.0)),
                   MakeTimeAccessor (&CcnxDataQueue::SetMaxDelay, &CcnxDataQueue::GetMaxDelay),
                   MakeTimeChecker ())
    ;
  return tid;
}

CcnxDataQueue::CcnxDataQueue ()
{
}

void
CcnxDataQueue::SetMaxDelay (const Time &value)
{
  m_maxWaitPeriod = value;
  m_randomPeriod = UniformVariable (0, m_maxWaitPeriod.ToDouble (Time::S));
}

Time
CcnxDataQueue::GetMaxDelay () const
{
  return m_maxWaitPeriod;
}

void
CcnxDataQueue::Enqueue (Ptr<Packet> packet, Ptr<CcnxFace> face)
{
  Time gap = Seconds (m_randomPeriod.GetValue ());
  if (m_totalWaitPeriod < m_maxWaitPeriod)
    {
      gap = std::min (m_maxWaitPeriod - m_totalWaitPeriod, gap);
    }
  else
    gap = Time (0);

  m_queue.push_back (Item (gap, packet, face));
  m_maxWaitPeriod += gap;
  
  if (!m_scheduledSend.IsRunning ())
    m_scheduledSend = Simulator::Schedule (m_queue.front ().gap, &CcnxDataQueue::Send, this);
}

void
CcnxDataQueue::Send ()
{
  NS_ASSERT (m_queue.size () > 0);
  Item &item = m_queue.front ();
  
  //////////////////////////////
  item.face->Send (item.packet);
  //////////////////////////////
  
  m_totalWaitPeriod -= item.gap;
  m_queue.pop_front ();
  
  if (m_queue.size () > 0)
    m_scheduledSend = Simulator::Schedule (m_queue.front ().gap, &CcnxDataQueue::Send, this);
}


} // namespace ns3
