/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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

#ifndef CCNX_FACE_H
#define CCNX_FACE_H

#include <ostream>

#include "ns3/ptr.h"
#include "ns3/object.h"

namespace ns3 {

class Packet;
class Node;

  
/**
 * \ingroup ccnx
 * \defgroup ccnx-face Faces
 */
/**
 * \ingroup ccnx-face
 * \brief Virtual class defining CCNx face
 *
 * This class defines basic functionality of CCNx face. Face is core
 * component responsible for actual delivery of data packet to and
 * from CCNx stack
 *
 * \see CcnxLocalFace, CcnxNetDeviceFace, CcnxIpv4Face, CcnxUdpFace
 */
class CcnxFace  : public Object
{
public:
  /**
   * \brief Ccnx protocol hanler
   *
   * \param face Face from which packet has been received
   * \param packet Received packet
   */
  typedef Callback<void,const Ptr<CcnxFace>&,const Ptr<const Packet>& > ProtocolHandler;
  
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Default constructor
   */
  CcnxFace ();
  virtual ~CcnxFace();

  ////////////////////////////////////////////////////////////////////
  
  /**
   * \brief Register callback to call when new packet arrives on the face
   *
   * This method should call protocol-dependent registration function
   */
  virtual void RegisterProtocolHandler (ProtocolHandler handler) = 0;
  
  /**
   * \brief Send packet on a face
   *
   * \param p smart pointer to a packet to send
   */ 
  virtual void Send (Ptr<Packet> p) = 0;

  ////////////////////////////////////////////////////////////////////

  /**
   * \brief Associate Node object with face
   *
   * \param node smart pointer to a Node object
   */
  virtual void SetNode (Ptr<Node> node);

  /**
   * \brief Assign routing/forwarding metric with face
   *
   * \param metric configured routing metric (cost) of this face
   */
  virtual void SetMetric (uint16_t metric);

  /**
   * \brief Get routing/forwarding metric assigned to the face
   *
   * \returns configured routing/forwarding metric (cost) of this face
   */
  virtual uint16_t GetMetric (void) const;

  /**
   * These are face states and may be distinct from actual lower-layer
   * device states, such as found in real implementations (where the
   * device may be down but ccnx face state is still up).
   */
  
  /**
   * \brief Enable this face
   */
  virtual void SetUp ();

  /**
   * \brief Disable this face
   */
  virtual void SetDown (void);

  /**
   * \brief Returns true if this face is enabled, false otherwise.
   */
  virtual bool IsUp () const;

  /**
   * \brief Returns true if this face is disabled, false otherwise.
   */
  virtual bool IsDown () const;

  /**
   * \brief Set node Id
   *
   * Id is purely informative and should not be used for any other purpose
   *
   * \param id id to set
   */
  inline void
  SetId (uint32_t id);

  /**
   * \brief Get node Id
   *
   * Id is purely informative and should not be used for any other purpose
   *
   * \returns id id to set
   */
  inline uint32_t
  GetId () const;

  /**
   * \brief Compare two faces. Only two faces on the same node could be compared.
   *
   * Internal index is used for comparison.
   */
  bool
  operator== (const CcnxFace &face) const;

  /**
   * \brief Compare two faces. Only two faces on the same node could be compared.
   *
   * Internal index is used for comparison.
   */
  bool
  operator< (const CcnxFace &face) const;
  
protected:
  virtual void DoDispose (void);

private:
  CcnxFace (const CcnxFace &); ///< \brief Disabled copy constructor
  CcnxFace& operator= (const CcnxFace &); ///< \brief Disabled copy operator
  
protected:
  uint16_t m_metric; ///< \brief Routing/forwarding metric
  Ptr<Node> m_node; ///< \brief Smart pointer to Node
  ProtocolHandler m_protocolHandler; ///< Callback via which packets are getting send to CCNx stack

private:
  bool m_ifup; ///< \brief flag indicating that the interface is UP 
  uint32_t m_id; ///< \brief id of the interface in CCNx stack (per-node uniqueness)
};

std::ostream& operator<< (std::ostream& os, const CcnxFace &face);

inline bool
operator < (const Ptr<CcnxFace> &lhs, const Ptr<CcnxFace> &rhs)
{
  return *lhs < *rhs;
}

void
CcnxFace::SetId (uint32_t id)
{
  m_id = id;
}

uint32_t
CcnxFace::GetId () const
{
  return m_id;
}

} // namespace ns3

#endif //CCNX_FACE_H
