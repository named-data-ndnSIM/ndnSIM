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
#include "geo-tag.h"

#include "ns3/ccnx-header-helper.h"

#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

NS_LOG_COMPONENT_DEFINE ("CcnxBroadcastNetDeviceFace");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CcnxBroadcastNetDeviceFace);

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

    .AddAttribute ("MaxDelayLowPriority", "Maximum delay for low-priority queue ('gradient' pushing queue)",
                   TimeValue (Seconds (0.010)),
                   MakeTimeAccessor (&CcnxBroadcastNetDeviceFace::SetMaxDelayLowPriority, &CcnxBroadcastNetDeviceFace::GetMaxDelayLowPriority),
                   MakeTimeChecker ())
    .AddAttribute ("MaxDistance", "Normalization distance for gradient pushing calculation",
                   DoubleValue (300.0),
                   MakeDoubleAccessor (&CcnxBroadcastNetDeviceFace::m_maxDistance),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("MaxDelayRetransmission", "Maximum delay between successive retransmissions of low-priority pushed packets",
                   TimeValue (Seconds (0.050)),
                   MakeTimeAccessor (&CcnxBroadcastNetDeviceFace::m_maxWaitRetransmission),
                   MakeTimeChecker ())
    .AddAttribute ("MaxRetransmissionAttempts", "Maximum number of retransmission attempts for low-priority pushed packets",
                   UintegerValue (7),
                   MakeUintegerAccessor (&CcnxBroadcastNetDeviceFace::m_maxRetxAttempts),
                   MakeUintegerChecker<uint32_t> ())

    .AddTraceSource ("WaitingTimeVsDistanceTrace", "On every low-priority packet trace the waiting gap and distance",
                     MakeTraceSourceAccessor (&CcnxBroadcastNetDeviceFace::m_waitingTimeVsDistanceTrace))
    .AddTraceSource ("JumpDistance", "Fired just before packet is actually transmitted if GeoTag is present and distance is more than 0",
                     MakeTraceSourceAccessor (&CcnxBroadcastNetDeviceFace::m_jumpDistanceTrace))
    .AddTraceSource ("Tx", "Fired every time packet is send out of face",
                     MakeTraceSourceAccessor (&CcnxBroadcastNetDeviceFace::m_tx))
    ;
  return tid;
}

/** 
 * By default, Ccnx face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
CcnxBroadcastNetDeviceFace::CcnxBroadcastNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : CcnxNetDeviceFace (node, netDevice)
  , m_maxPacketsInQueue (100)
{
  NS_LOG_FUNCTION (this << node << netDevice);

  PointerValue pointer;
  bool ok = netDevice->GetAttributeFailSafe ("Mac", pointer);
  if (!ok)
    {
      NS_LOG_DEBUG ("Cannot get `Mac` thing from NetDevice");
      return;
    }
  pointer.Get<Object> () ->GetAttributeFailSafe ("DcaTxop", pointer);
  if (!ok)
    {
      NS_LOG_DEBUG ("Cannot get `DcaTxop` thing from WifiMac");
      return;
    }
  pointer.Get<Object> () ->GetAttributeFailSafe ("Queue", pointer);
  if (!ok)
    {
      NS_LOG_DEBUG ("Cannot get `Queue` thing from DcaTxop");
      return;
    }
  
  UintegerValue value;
  pointer.Get<Object> () ->GetAttributeFailSafe ("MaxPacketNumber", value);
  if (!ok)
    {
      NS_LOG_DEBUG ("Cannot get `MaxPacketNumber` thing from Queue");
      return;
    }

  NS_LOG_DEBUG ("Number of packets obtained from WifiMac/DcaTxop/Queue: " << value.Get ());
  m_maxPacketsInQueue = value.Get ();  
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

void
CcnxBroadcastNetDeviceFace::SetMaxDelayLowPriority (const Time &value)
{
  m_maxWaitLowPriority = value;
}

Time
CcnxBroadcastNetDeviceFace::GetMaxDelayLowPriority () const
{
  return m_maxWaitLowPriority;
}


CcnxBroadcastNetDeviceFace::Item::Item (const Time &_gap, const Ptr<Packet> &_packet)
  : gap (_gap), packet (_packet), retxCount (0)
{
  NS_LOG_FUNCTION (this << _gap << _packet);

  Ptr<const CcnxNameComponentsTag> tag = packet->PeekPacketTag<CcnxNameComponentsTag> ();
  
  NS_ASSERT_MSG (tag != 0, "CcnxNameComponentsTag should be set somewhere");
  name = tag->GetName ();
  NS_LOG_DEBUG ("Schedule ContentObject with " << *tag->GetName () << " for delayed transmission after " << _gap);
}

CcnxBroadcastNetDeviceFace::Item::Item (const Item &item)
  : gap (item.gap), packet (item.packet), name (item.name), retxCount (item.retxCount)
{
}

CcnxBroadcastNetDeviceFace::Item &
CcnxBroadcastNetDeviceFace::Item::operator ++ ()
{
  // remote GeoTag when packet is scheduled for retransmission
  packet->RemovePacketTag<GeoTag> ();

  retxCount ++;
  return *this;
}

CcnxBroadcastNetDeviceFace::Item &
CcnxBroadcastNetDeviceFace::Item::Gap (const Time &time)
{
  gap = time;
  return *this;
}

Time
CcnxBroadcastNetDeviceFace::GetPriorityQueueGap () const
{
  Time gap = Seconds (m_randomPeriod.GetValue ());
  if (m_totalWaitPeriod < m_maxWaitPeriod)
    {
      gap = std::min (m_maxWaitPeriod - m_totalWaitPeriod, gap);
    }
  else
    gap = Time (0);

  return gap;
}

void
CcnxBroadcastNetDeviceFace::SendLowPriority (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);
  
  if (m_queue.size () >= m_maxPacketsInQueue ||
      m_lowPriorityQueue.size () >= m_maxPacketsInQueue)
    {
      // \todo Maybe add tracing
      NS_LOG_DEBUG ("Too many packets enqueue already. Don't do anything");
      return;
    }
  
  Ptr<const GeoTag> tag = packet->PeekPacketTag<GeoTag> ();

  Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();
  if (mobility == 0)
    {
      NS_FATAL_ERROR ("Mobility model has to be installed on the node");
      return;
    }
  
  double distance = m_maxDistance;
  if (tag != 0) // if tag == 0, it means that packet came from application
    {
      // NS_LOG_DEBUG ("Tag is OK, distance is " << CalculateDistance (tag->GetPosition (), mobility->GetPosition ()));
      distance = CalculateDistance (tag->GetPosition (), mobility->GetPosition ());
      distance = std::min (m_maxDistance, distance);
    }

  // NS_LOG_DEBUG ("Distance: " << distance);
  
  // Mean waiting time.  Reversely proportional to the distance from the original transmitter
  // Closer guys will tend to wait longer than guys far away
  double meanWaiting = m_maxWaitLowPriority.ToDouble (Time::S) * (m_maxDistance - distance) / m_maxDistance;
  // NS_LOG_DEBUG ("Mean waiting: " << meanWaiting);
  
  // TriangularVariable randomLowPriority = TriangularVariable (0, m_maxWaitLowPriority.ToDouble (Time::S), (meanWaiting+m_maxWaitLowPriority.ToDouble (Time::S))/3.0);

  // NormalVariable randomLowPriority = NormalVariable (meanWaiting, /* mean */
  //                                                    m_maxWaitPeriod.ToDouble (Time::S) * m_maxWaitPeriod.ToDouble (Time::S), /*variance*/
  //                                                    meanWaiting + m_maxWaitPeriod.ToDouble (Time::S)/*bound*/);

  UniformVariable randomLowPriority (meanWaiting, meanWaiting + m_maxWaitPeriod.ToDouble (Time::S));
  
  double sample = std::abs (randomLowPriority.GetValue ());
  NS_LOG_DEBUG ("Sample: " << sample);

  m_waitingTimeVsDistanceTrace (distance, sample);
  
  // Actual gap will be defined by Triangular distribution based on Geo metric + Uniform distribution that is aimed to avoid collisions
  m_lowPriorityQueue.push_back (Item (Seconds (sample), packet));

  if (!m_scheduledSend.IsRunning ())
    m_scheduledSend = Simulator::Schedule (m_lowPriorityQueue.front ().gap, &CcnxBroadcastNetDeviceFace::SendFromQueue, this);
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
      if (m_queue.size () >= m_maxPacketsInQueue)
        {
          // \todo Maybe add tracing
          NS_LOG_INFO ("Dropping data packet that exceed queue size");
          return;
        }
      
      Time gap = GetPriorityQueueGap ();
      m_queue.push_back (Item (gap, packet));
      m_totalWaitPeriod += gap;

      if (!m_scheduledSend.IsRunning ())
        m_scheduledSend = Simulator::Schedule (m_queue.front ().gap, &CcnxBroadcastNetDeviceFace::SendFromQueue, this);
    }
  else
    {
      NS_FATAL_ERROR ("Unknown CCNX header type");
    } 
}

void
CcnxBroadcastNetDeviceFace::NotifyJumpDistanceTrace (const Ptr<Packet> packet)
{
  Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();
  if (mobility == 0)
    {
      NS_FATAL_ERROR ("Mobility model has to be installed on the node");
      return;
    }

  m_tx (m_node, packet, mobility->GetPosition ());

  Ptr<const GeoTag> tag = packet->PeekPacketTag<GeoTag> ();
  if (tag == 0) return;
  
  double distance = CalculateDistance (tag->GetPosition (), mobility->GetPosition ());

  m_jumpDistanceTrace (m_node, distance);
}

void
CcnxBroadcastNetDeviceFace::SendFromQueue ()
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT ((m_queue.size () + m_lowPriorityQueue.size ()) > 0);

  // If high-priority queue is not empty, send data from it
  if (m_queue.size () > 0)
    {
      Item &item = m_queue.front ();

      NotifyJumpDistanceTrace (item.packet);
      
      //////////////////////////////
      CcnxNetDeviceFace::SendImpl (item.packet->Copy ());
      //////////////////////////////

      if (item.retxCount < m_maxRetxAttempts)
        m_retxQueue.push_back (++(item.Gap (m_maxWaitRetransmission)));
      
      m_totalWaitPeriod -= item.gap;
      m_queue.pop_front ();
    }
  else if (m_lowPriorityQueue.size () > 0) // no reason for this check, just for readability
    {
      Item &item = m_lowPriorityQueue.front ();
  
      NotifyJumpDistanceTrace (item.packet);

      //////////////////////////////
      CcnxNetDeviceFace::SendImpl (item.packet->Copy ());
      //////////////////////////////

      if (item.retxCount < m_maxRetxAttempts)
        m_retxQueue.push_back (++(item.Gap (m_maxWaitRetransmission)));

      m_lowPriorityQueue.pop_front ();
    }

  if (m_queue.size () > 0)
    m_scheduledSend = Simulator::Schedule (m_queue.front ().gap, &CcnxBroadcastNetDeviceFace::SendFromQueue, this);
  else if (m_lowPriorityQueue.size () > 0)
    m_scheduledSend = Simulator::Schedule (m_lowPriorityQueue.front ().gap, &CcnxBroadcastNetDeviceFace::SendFromQueue, this);

  if (!m_retxEvent.IsRunning () && m_retxQueue.size () > 0)
    {
      m_retxEvent = Simulator::Schedule (m_retxQueue.front ().gap, &CcnxBroadcastNetDeviceFace::ProcessRetx, this);
    }
}

void
CcnxBroadcastNetDeviceFace::ProcessRetx ()
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_retxQueue.size () > 0);

  Time gap = GetPriorityQueueGap ();
  Item item (gap, m_retxQueue.front ().packet);
  item.retxCount = m_retxQueue.front ().retxCount;
  m_lowPriorityQueue.push_back (item);

  m_retxQueue.pop_front ();
  
  if (!m_scheduledSend.IsRunning ())
    m_scheduledSend = Simulator::Schedule (m_lowPriorityQueue.front ().gap, &CcnxBroadcastNetDeviceFace::SendFromQueue, this);
  
  if (m_retxQueue.size () > 0)
    m_retxEvent = Simulator::Schedule (m_retxQueue.front ().gap, &CcnxBroadcastNetDeviceFace::ProcessRetx, this);
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

      bool cancelled = false;
      ItemQueue::iterator item = m_lowPriorityQueue.begin ();
      while (item != m_lowPriorityQueue.end ())
        {
          if (*item->name == *name)
            {
              cancelled = true;
              ItemQueue::iterator tmp = item;
              tmp ++;

              NS_LOG_INFO ("Canceling ContentObject with name " << *name << ", which is scheduled for low-priority transmission");
              m_lowPriorityQueue.erase (item);
              if (m_queue.size () + m_lowPriorityQueue.size () == 0)
                {
                  m_scheduledSend.Cancel ();
                }              

              item = tmp;
            }
          else
            item ++;
        }

      item = m_queue.begin ();
      while (item != m_queue.end ())
        {
          if (*item->name == *name)
            {
              cancelled = true;
              ItemQueue::iterator tmp = item;
              tmp ++;

              NS_LOG_INFO ("Canceling ContentObject with name " << *name << ", which is scheduled for transmission");
              m_totalWaitPeriod -= item->gap;
              m_queue.erase (item);
              if (m_queue.size () == 0)
                {
                  m_scheduledSend.Cancel ();
                }

              item = tmp;
            }
          else
            item ++;
        }

      item = m_retxQueue.begin ();
      while (item != m_retxQueue.end ())
        {
          if (*item->name == *name)
            {
              cancelled = true;
              ItemQueue::iterator tmp = item;
              tmp ++;

              NS_LOG_INFO ("Canceling ContentObject with name " << *name << ", which is planned for retransmission");
              m_retxQueue.erase (item);
              if (m_retxQueue.size () == 0)
                {
                  NS_LOG_INFO ("Canceling the retx processing event");
                  m_retxEvent.Cancel ();
                }

              item = tmp;
            }
          else
            item ++;
        }

      if (cancelled)
        return;
      else
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

