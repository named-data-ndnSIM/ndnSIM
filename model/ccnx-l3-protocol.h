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

#ifndef CCNX_L3_PROTOCOL_H
#define CCNX_L3_PROTOCOL_H

#include <list>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"

#include "ccnx-content-store.h"
#include "ccnx-pit.h"
#include "ccnx-fib.h"

#include "ccnx.h"

namespace ns3 {

class Packet;
class NetDevice;
class Node;
class CcnxFace;
class CcnxRoute;
class CcnxForwardingStrategy;
class Header;
class CcnxInterestHeader;
class CcnxContentObjectHeader;

    
/**
 * \ingroup ccnx
 * \brief Actual implementation of the Ccnx network layer
 * 
 * \todo This description is incorrect. Should be changed accordingly
 *
 * This class contains two distinct groups of trace sources.  The
 * trace sources 'Rx' and 'Tx' are called, respectively, immediately
 * after receiving from the NetDevice and immediately before sending
 * to a NetDevice for transmitting a packet.  These are low level
 * trace sources that include the CcnxHeader already serialized into
 * the packet.  In contrast, the Drop, SendOutgoing, UnicastForward,
 * and LocalDeliver trace sources are slightly higher-level and pass
 * around the CcnxHeader as an explicit parameter and not as part of
 * the packet.
 */
class CcnxL3Protocol : public Ccnx
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  static const uint16_t ETHERNET_FRAME_TYPE; ///< \brief Ethernet Frame Type of CCNx
  // static const uint16_t IP_PROTOCOL_TYPE;    ///< \brief IP protocol type of CCNx
  // static const uint16_t UDP_PORT;            ///< \brief UDP port of CCNx

  /**
   * \brief Default constructor. Creates an empty stack without forwarding strategy set
   */
  CcnxL3Protocol();
  virtual ~CcnxL3Protocol ();

  ////////////////////////////////////////////////////////////////////
  // functions defined in base class Ccnx

  virtual uint32_t
  AddFace (const Ptr<CcnxFace> &face);
  
  virtual uint32_t
  GetNFaces () const;
  
  virtual Ptr<CcnxFace>
  GetFace (uint32_t face) const;

  virtual void
  RemoveFace (Ptr<CcnxFace> face);

  virtual Ptr<CcnxFace>
  GetFaceByNetDevice (Ptr<NetDevice> netDevice) const;
  
  // void ScheduleLeakage();
private:
  void
  Receive (const Ptr<CcnxFace> &face, const Ptr<const Packet> &p);

  /**
   * \brief Actual processing of incoming CCNx interests. Note, interests do not have payload
   * 
   * Processing Interest packets
   * @param face    incoming face
   * @param header  deserialized Interest header
   * @param packet  original packet
   */
  void
  OnInterest (const Ptr<CcnxFace> &face,
              Ptr<CcnxInterestHeader> &header,
              const Ptr<const Packet> &p);

  /**
   * \brief Processing of incoming CCNx NACKs. Note, these packets, like interests, do not have payload
   * 
   * Processing NACK packets
   * @param face    incoming face
   * @param header  deserialized Interest header
   * @param packet  original packet
   */
  void
  OnNack (const Ptr<CcnxFace> &face,
          Ptr<CcnxInterestHeader> &header,
          const Ptr<const Packet> &p);
  
  /**
   * \brief Actual processing of incoming CCNx content objects
   * 
   * Processing ContentObject packets
   * @param face    incoming face
   * @param header  deserialized ContentObject header
   * @param payload data packet payload
   * @param packet  original packet
   */
  void
  OnData (const Ptr<CcnxFace> &face,
          Ptr<CcnxContentObjectHeader> &header,
          Ptr<Packet> &payload,
          const Ptr<const Packet> &packet);

protected:
  virtual void DoDispose (void); ///< @brief Do cleanup

  /**
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();

private:
  CcnxL3Protocol(const CcnxL3Protocol &); ///< copy constructor is disabled
  CcnxL3Protocol &operator = (const CcnxL3Protocol &); ///< copy operator is disabled

  void
  GiveUpInterest (Ptr<CcnxPitEntry> pitEntry,
                  Ptr<CcnxInterestHeader> header);

  void
  OnDataDelayed (Ptr<const CcnxContentObjectHeader> header,
                 Ptr<const Packet> payload,
                 const Ptr<const Packet> &packet);
  
private:
  uint32_t m_faceCounter; ///< \brief counter of faces. Increased every time a new face is added to the stack
  typedef std::vector<Ptr<CcnxFace> > CcnxFaceList;
  CcnxFaceList m_faces; ///< \brief list of faces that belongs to ccnx stack on this node

  // These objects are aggregated, but for optimization, get them here
  Ptr<Node> m_node; ///< \brief node on which ccnx stack is installed
  Ptr<CcnxForwardingStrategy> m_forwardingStrategy; ///< \brief smart pointer to the selected forwarding strategy
  Ptr<CcnxPit> m_pit; ///< \brief PIT (pending interest table)
  Ptr<CcnxFib> m_fib; ///< \brief FIB  
  Ptr<CcnxContentStore> m_contentStore; ///< \brief Content store (for caching purposes only)

  bool m_nacksEnabled;
  bool m_cacheUnsolicitedData;
};
  
} // Namespace ns3

#endif /* CCNX_L3_PROTOCOL_H */
