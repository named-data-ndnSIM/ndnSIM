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
#include "ccnx-name-components-tag.h"

#include "ns3/ccnx-header-helper.h"

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
  NS_LOG_FUNCTION (this << value);

  m_maxWaitPeriod = value;
  m_randomPeriod = UniformVariable (0, m_maxWaitPeriod.ToDouble (Time::S));
}

Time
CcnxBroadcastNetDeviceFace::GetMaxDelay () const
{
  return m_maxWaitPeriod;
}


CcnxBroadcastNetDeviceFace::Item::Item (const Time &_gap, const Ptr<Packet> &_packet)
  : gap (_gap), packet (_packet)
{
  NS_LOG_FUNCTION (this << _gap << _packet);
  
  Ptr<const CcnxNameComponentsTag> tag = packet->PeekPacketTag<CcnxNameComponentsTag> ();
  NS_ASSERT_MSG (tag != 0, "CcnxNameComponentsTag should be set somewhere");
  name = tag->GetName ();
  NS_LOG_DEBUG ("Schedule ContentObject with " << *tag->GetName () << " for delayed transmission");
}

void
CcnxBroadcastNetDeviceFace::SendImpl (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);

  CcnxHeaderHelper::Type type = CcnxHeaderHelper::GetCcnxHeaderType (packet);
  if (type == CcnxHeaderHelper::INTEREST)
    {
      // send immediately, don't delay
      
      //////////////////////////////
      CcnxNetDeviceFace::SendImpl (packet);
      //////////////////////////////
    }
  else if (type == CcnxHeaderHelper::CONTENT_OBJECT)
    {
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
  else
    {
      NS_FATAL_ERROR ("Unknown CCNX header type");
    } 
}

void
CcnxBroadcastNetDeviceFace::SendFromQueue ()
{
  NS_LOG_FUNCTION (this);

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

  CcnxHeaderHelper::Type type = CcnxHeaderHelper::GetCcnxHeaderType (p);
  if (type == CcnxHeaderHelper::INTEREST)
    {
      NS_LOG_DEBUG ("Interest");
      Receive (p);
    }
  else if (type == CcnxHeaderHelper::CONTENT_OBJECT)
    {
      NS_LOG_DEBUG ("ContentObject");
      Ptr<const CcnxNameComponentsTag> tag = p->PeekPacketTag<CcnxNameComponentsTag> ();
      NS_ASSERT_MSG (tag != 0, "CcnxNameComponentsTag should be set somewhere");
      Ptr<const CcnxNameComponents> name = tag->GetName ();

      for (ItemQueue::iterator item = m_queue.begin (); item != m_queue.end (); item++)
        {
          if (*item->name == *name)
            {
              // do something
              m_totalWaitPeriod -= item->gap;
              m_queue.erase (item);
              if (m_queue.size () == 0)
                {
                  m_scheduledSend.Cancel ();
                }

              NS_LOG_INFO ("Canceling ContentObject with name " << *name << ", which is scheduled for transmission");
              return;
            }
        }
      Receive (p);
    }
  else
    {
      NS_FATAL_ERROR ("Unknown CCNX header type");
    }
}


std::ostream&
CcnxBroadcastNetDeviceFace::Print (std::ostream& os) const
{
  os << "dev=bc-net(" << GetId () << ")";
  return os;
}

}; // namespace ns3

