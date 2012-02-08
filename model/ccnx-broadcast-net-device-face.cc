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
 *
 */

#include "ccnx-broadcast-net-device-face.h"
#include "ccnx-l3-protocol.h"

#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("CcnxBroadcastNetDeviceFace");

namespace ns3 {

TypeId
CcnxBroadcastNetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxBroadcastNetDeviceFace")
    .SetParent<CcnxNetDeviceFace> ()
    .SetGroupName ("Ccnx")

    .AddAttribute ("MaxDelay", "Maximum delay that packets should wait in queue (max for uniform random variable)",
                   TimeValue (Seconds (0.002)),
                   MakeTimeAccessor (&CcnxBroadcastNetDeviceFace::SetMaxDelay, &CcnxBroadcastNetDeviceFace::GetMaxDelay),
                   MakeTimeChecker ())
    ;
  return tid;
}

/** 
 * By default, Ccnx face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
CcnxBroadcastNetDeviceFace::CcnxBroadcastNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : CcnxNetDeviceFace (node, netDevice)
{
  NS_LOG_FUNCTION (this << node << netDevice);
}

CcnxBroadcastNetDeviceFace::~CcnxBroadcastNetDeviceFace ()
{
  NS_LOG_FUNCTION (this);
}

CcnxBroadcastNetDeviceFace& CcnxBroadcastNetDeviceFace::operator= (const CcnxBroadcastNetDeviceFace &)
{
  return *this;
}

void
CcnxBroadcastNetDeviceFace::SetMaxDelay (const Time &value)
{
  m_maxWaitPeriod = value;
  m_randomPeriod = UniformVariable (0, m_maxWaitPeriod.ToDouble (Time::S));
}

Time
CcnxBroadcastNetDeviceFace::GetMaxDelay () const
{
  return m_maxWaitPeriod;
}

void
CcnxBroadcastNetDeviceFace::SendImpl (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);

  Time gap = Seconds (m_randomPeriod.GetValue ());
  if (m_totalWaitPeriod < m_maxWaitPeriod)
    {
      gap = std::min (m_maxWaitPeriod - m_totalWaitPeriod, gap);
    }
  else
    gap = Time (0);

  m_queue.push_back (Item (gap, packet));
  m_maxWaitPeriod += gap;
  
  if (!m_scheduledSend.IsRunning ())
    m_scheduledSend = Simulator::Schedule (m_queue.front ().gap, &CcnxBroadcastNetDeviceFace::SendFromQueue, this);
}

void
CcnxBroadcastNetDeviceFace::SendFromQueue ()
{
  NS_ASSERT (m_queue.size () > 0);
  Item &item = m_queue.front ();
  
  //////////////////////////////
  CcnxNetDeviceFace::SendImpl (item.packet);
  //////////////////////////////
  
  m_totalWaitPeriod -= item.gap;
  m_queue.pop_front ();
  
  if (m_queue.size () > 0)
    m_scheduledSend = Simulator::Schedule (m_queue.front ().gap, &CcnxBroadcastNetDeviceFace::SendFromQueue, this);
}

// callback
void
CcnxBroadcastNetDeviceFace::ReceiveFromNetDevice (Ptr<NetDevice> device,
                                                  Ptr<const Packet> p,
                                                  uint16_t protocol,
                                                  const Address &from,
                                                  const Address &to,
                                                  NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (device << p << protocol << from << to << packetType);
  Receive (p);
}


std::ostream&
CcnxBroadcastNetDeviceFace::Print (std::ostream& os) const
{
  os << "dev=bc-net(" << GetId () << ")";
  return os;
}

}; // namespace ns3

