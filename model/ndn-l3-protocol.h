/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_L3_PROTOCOL_H
#define NDN_L3_PROTOCOL_H

#include <list>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"

// #include "ndn-content-store.h"
// #include "ndn-pit.h"
// #include "ndn-fib.h"

#include "ndn.h"

namespace ns3 {

class Packet;
class NetDevice;
class Node;
class NdnFace;
class NdnRoute;
class NdnForwardingStrategy;
class Header;
class NdnInterestHeader;
class NdnContentObjectHeader;

    
/**
 * \ingroup ndn
 * \brief Actual implementation of the Ndn network layer
 * 
 * \todo This description is incorrect. Should be changed accordingly
 *
 * This class contains two distinct groups of trace sources.  The
 * trace sources 'Rx' and 'Tx' are called, respectively, immediately
 * after receiving from the NetDevice and immediately before sending
 * to a NetDevice for transmitting a packet.  These are low level
 * trace sources that include the NdnHeader already serialized into
 * the packet.  In contrast, the Drop, SendOutgoing, UnicastForward,
 * and LocalDeliver trace sources are slightly higher-level and pass
 * around the NdnHeader as an explicit parameter and not as part of
 * the packet.
 */
class NdnL3Protocol : public Ndn
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  static const uint16_t ETHERNET_FRAME_TYPE; ///< \brief Ethernet Frame Type of Ndn
  // static const uint16_t IP_PROTOCOL_TYPE;    ///< \brief IP protocol type of Ndn
  // static const uint16_t UDP_PORT;            ///< \brief UDP port of Ndn

  /**
   * \brief Default constructor. Creates an empty stack without forwarding strategy set
   */
  NdnL3Protocol();
  virtual ~NdnL3Protocol ();

  ////////////////////////////////////////////////////////////////////
  // functions defined in base class Ndn

  virtual uint32_t
  AddFace (const Ptr<NdnFace> &face);
  
  virtual uint32_t
  GetNFaces () const;
  
  virtual Ptr<NdnFace>
  GetFace (uint32_t face) const;

  virtual void
  RemoveFace (Ptr<NdnFace> face);

  virtual Ptr<NdnFace>
  GetFaceByNetDevice (Ptr<NetDevice> netDevice) const;
  
  // void ScheduleLeakage();
private:
  void
  Receive (const Ptr<NdnFace> &face, const Ptr<const Packet> &p);

  // /**
  //  * \brief Actual processing of incoming Ndn interests. Note, interests do not have payload
  //  * 
  //  * Processing Interest packets
  //  * @param face    incoming face
  //  * @param header  deserialized Interest header
  //  * @param packet  original packet
  //  */
  // void
  // OnInterest (const Ptr<NdnFace> &face,
  //             Ptr<NdnInterestHeader> &header,
  //             const Ptr<const Packet> &p);

  // /**
  //  * \brief Processing of incoming Ndn NACKs. Note, these packets, like interests, do not have payload
  //  * 
  //  * Processing NACK packets
  //  * @param face    incoming face
  //  * @param header  deserialized Interest header
  //  * @param packet  original packet
  //  */
  // void
  // OnNack (const Ptr<NdnFace> &face,
  //         Ptr<NdnInterestHeader> &header,
  //         const Ptr<const Packet> &p);
  
  // /**
  //  * \brief Actual processing of incoming Ndn content objects
  //  * 
  //  * Processing ContentObject packets
  //  * @param face    incoming face
  //  * @param header  deserialized ContentObject header
  //  * @param payload data packet payload
  //  * @param packet  original packet
  //  */
  // void
  // OnData (const Ptr<NdnFace> &face,
  //         Ptr<NdnContentObjectHeader> &header,
  //         Ptr<Packet> &payload,
  //         const Ptr<const Packet> &packet);

protected:
  virtual void DoDispose (void); ///< @brief Do cleanup

  /**
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();

private:
  NdnL3Protocol(const NdnL3Protocol &); ///< copy constructor is disabled
  NdnL3Protocol &operator = (const NdnL3Protocol &); ///< copy operator is disabled

  // void
  // GiveUpInterest (Ptr<NdnPitEntry> pitEntry,
  //                 Ptr<NdnInterestHeader> header);

  // void
  // OnDataDelayed (Ptr<const NdnContentObjectHeader> header,
  //                Ptr<const Packet> payload,
  //                const Ptr<const Packet> &packet);
  
private:
  uint32_t m_faceCounter; ///< \brief counter of faces. Increased every time a new face is added to the stack
  typedef std::vector<Ptr<NdnFace> > NdnFaceList;
  NdnFaceList m_faces; ///< \brief list of faces that belongs to ndn stack on this node

  // These objects are aggregated, but for optimization, get them here
  Ptr<Node> m_node; ///< \brief node on which ndn stack is installed
  Ptr<NdnForwardingStrategy> m_forwardingStrategy; ///< \brief smart pointer to the selected forwarding strategy

  // bool m_nacksEnabled;
  // bool m_cacheUnsolicitedData;
};
  
} // Namespace ns3

#endif /* NDN_L3_PROTOCOL_H */
