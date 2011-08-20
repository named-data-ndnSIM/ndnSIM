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
#include <stdint.h>
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/traced-callback.h"

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
class CcnxContentStore;

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
  static const uint16_t IP_PROTOCOL_TYPE;    ///< \brief IP protocol type of CCNx
  static const uint16_t UDP_PORT;            ///< \brief UDP port of CCNx

  /**
   * \brief Default constructor. Creates an empty stack without forwarding strategy set
   */
  CcnxL3Protocol();
  virtual ~CcnxL3Protocol ();

  /**
   * \enum DropReason
   * \brief Reason why a packet has been dropped.
   */
  enum DropReason 
  {
    /** \todo  Fill reasons from QualNet code */
    DROP_DUPLICATE_INTEREST=1,  /**< Duplicate Interest */
    DROP_CONGESTION, /**< Congestion detected */
    DROP_NO_ROUTE,   /**< No route to host */
    DROP_INTERFACE_DOWN,   /**< Interface is down so can not send packet */
  };

  /**
   * \brief Assigns node to the CCNx stack
   *
   * \param node Simulation node
   */
  void SetNode (Ptr<Node> node);

  ////////////////////////////////////////////////////////////////////
  // functions defined in base class Ccnx
  
  void SetForwardingStrategy (Ptr<CcnxForwardingStrategy> forwardingStrategy);
  Ptr<CcnxForwardingStrategy> GetForwardingStrategy () const;

  virtual void Send (const Ptr<CcnxFace> &face, const Ptr<Packet> &packet);
  virtual void Receive (const Ptr<CcnxFace> &face, const Ptr<const Packet> &p);

  virtual uint32_t AddFace (const Ptr<CcnxFace> &face);
  virtual uint32_t GetNFaces () const;
  virtual Ptr<CcnxFace> GetFace (uint32_t face) const;

protected:
  /**
   * \brief Actual processing of incoming CCNx interests
   * 
   * Processing Interest packets
   */
  virtual void
  ReceiveAndProcess (const Ptr<CcnxFace> &face,
                     const Ptr<CcnxInterestHeader> &header,
                     const Ptr<Packet> &p);

  
  /**
   * \brief Actual processing of incoming CCNx content objects
   * 
   * Processing ContentObject packets
   */
  virtual void
  ReceiveAndProcess (const Ptr<CcnxFace> &face,
                     const Ptr<CcnxContentObjectHeader> &header,
                     const Ptr<Packet> &p);

protected:
  virtual void DoDispose (void);

  /**
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();

private:
  CcnxL3Protocol(const CcnxL3Protocol &); ///< copy constructor is disabled
  CcnxL3Protocol &operator = (const CcnxL3Protocol &); ///< copy operator is disabled

  /**
   * \brief Fake function. should never be called. Just to trick C++ to compile
   */
  virtual void
  ReceiveAndProcess (Ptr<CcnxFace> face, Ptr<Header> header, Ptr<Packet> p);

private:
  uint32_t m_faceCounter; ///< \brief counter of faces. Increased every time a new face is added to the stack
  typedef std::vector<Ptr<CcnxFace> > CcnxFaceList;
  CcnxFaceList m_faces; ///< \brief list of faces that belongs to ccnx stack on this node

  Ptr<Node> m_node; ///< \brief node on which ccnx stack is installed
  Ptr<CcnxForwardingStrategy> m_forwardingStrategy; ///< \brief smart pointer to the selected forwarding strategy

  Ptr<CcnxContentStore> m_contentStore;
  
  /**
   * \brief Trace of transmitted packets, including all headers
   * \internal
   */
  TracedCallback<Ptr<const Packet>, Ptr<Ccnx>, Ptr<const CcnxFace> > m_txTrace;

  /**
   * \brief Trace of received packets, including all headers
   * \internal
   */
  TracedCallback<Ptr<const Packet>, Ptr<Ccnx>, Ptr<const CcnxFace> > m_rxTrace;

  /**
   * \brief Trace of dropped packets, including reason and all headers
   * \internal
   */
  TracedCallback<Ptr<const Packet>, DropReason, Ptr<const Ccnx>, Ptr<const CcnxFace> > m_dropTrace;
};
  
} // Namespace ns3

#endif /* CCNX_L3_PROTOCOL_H */
