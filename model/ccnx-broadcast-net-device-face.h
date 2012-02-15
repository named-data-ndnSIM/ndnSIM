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
 * Authors: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCNX_BROADCAST_NET_DEVICE_FACE_H
#define CCNX_BROADCAST_NET_DEVICE_FACE_H

#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/random-variable.h"
#include "ns3/traced-callback.h"

#include "ns3/ccnx-net-device-face.h"

namespace ns3 {

class CcnxNameComponents;

/**
 * \ingroup ccnx-face
 * \brief Implementation of layer-2 broadcast (Ethernet) CCNx face
 *
 * This class defines basic functionality of CCNx face. Face is core
 * component responsible for actual delivery of data packet to and
 * from CCNx stack
 *
 * CcnxNetDevice face is permanently associated with one NetDevice
 * object and this object cannot be changed for the lifetime of the
 * face
 *
 * The only difference from the base class is that CcnxBroadcastNetDevice
 * makes additional consideration for overheard information.
 *
 * \see CcnxLocalFace, CcnxNetDeviceFace, CcnxIpv4Face, CcnxUdpFace
 */
class CcnxBroadcastNetDeviceFace  : public CcnxNetDeviceFace
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief Constructor
   *
   * \param netDevice a smart pointer to NetDevice object to which
   * this face will be associate
   */
  CcnxBroadcastNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice);
  virtual ~CcnxBroadcastNetDeviceFace();

  // from CcnxFace
  virtual void
  SendLowPriority (Ptr<Packet> p);
  
protected:
  // from CcnxFace
  virtual void
  SendImpl (Ptr<Packet> p);

public:
  virtual std::ostream&
  Print (std::ostream &os) const;

private:
  CcnxBroadcastNetDeviceFace (const CcnxBroadcastNetDeviceFace &); ///< \brief Disabled copy constructor
  CcnxBroadcastNetDeviceFace& operator= (const CcnxBroadcastNetDeviceFace &); ///< \brief Disabled copy operator

  /// \brief callback from lower layers
  virtual void
  ReceiveFromNetDevice (Ptr<NetDevice> device,
                        Ptr<const Packet> p,
                        uint16_t protocol,
                        const Address &from,
                        const Address &to,
                        NetDevice::PacketType packetType);

  void
  SendFromQueue ();

  void
  SetMaxDelay (const Time &value);

  Time
  GetMaxDelay () const;

  void
  SetMaxDelayLowPriority (const Time &value);

  Time
  GetMaxDelayLowPriority () const;

  void
  ProcessRetx ();

  Time
  GetPriorityQueueGap () const;

private:
  struct Item
  {
    Item (const Time &_gap, const Ptr<Packet> &_packet);
    Item (const Item &item);

    Item &
    operator ++ ();

    Item &
    Gap (const Time &time);
    
    Time gap;
    Ptr<Packet> packet;
    Ptr<const CcnxNameComponents> name;
    uint32_t retxCount;
  };
  typedef std::list<Item> ItemQueue;

  EventId m_scheduledSend;

  // Primary queue (for requested ContentObject packets)
  Time m_totalWaitPeriod;
  UniformVariable m_randomPeriod;
  Time m_maxWaitPeriod;
  uint32_t m_maxPacketsInQueue;
  ItemQueue m_queue;

  // Low-priority queue (for pushing Interest and ContentObject packets)
  Time m_maxWaitLowPriority;
  double m_maxDistance;
  ItemQueue m_lowPriorityQueue;

  // Retransmission queue for low-priority pushing
  EventId m_retxEvent;
  Time m_maxWaitRetransmission;
  ItemQueue m_retxQueue;
  uint32_t m_maxRetxAttempts;

  TracedCallback<double, double> m_waitingTimeVsDistanceTrace;
};

} // namespace ns3

#endif //CCNX_BROADCAST_NET_DEVICE_FACE_H
