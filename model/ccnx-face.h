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
 * Authors: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCNX_FACE_H
#define CCNX_FACE_H

#include <ostream>
#include <algorithm>

#include "ns3/ptr.h"
#include "ns3/ccnx.h"
#include "ns3/nstime.h"
#include "ns3/type-id.h"

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
  static TypeId
  GetTypeId ();
  
  /**
   * \brief Ccnx protocol handler
   *
   * \param face Face from which packet has been received
   * \param packet Original packet
   */
  typedef Callback<void,const Ptr<CcnxFace>&,const Ptr<const Packet>& > ProtocolHandler;

  /**
   * \brief Default constructor
   */
  CcnxFace (Ptr<Node> node);
  virtual ~CcnxFace();

  /**
   * @brief Get node to which this face is associated
   */
  Ptr<Node>
  GetNode () const;

  ////////////////////////////////////////////////////////////////////
  
  /**
   * \brief Register callback to call when new packet arrives on the face
   *
   * This method should call protocol-dependent registration function
   */
  virtual void
  RegisterProtocolHandler (ProtocolHandler handler);

  /**
   * @brief Check if Interest limit is reached
   *
   * Side effect: if limit is not yet reached, the number of outstanding packets will be increased
   *
   * @returns true if Interest limit is not yet reached
   */
  bool
  IsBelowLimit ();
  
  /**
   * \brief Send packet on a face
   *
   * This method will be called by lower layers to send data to device or application
   *
   * \param p smart pointer to a packet to send
   *
   * @return false if either limit is reached
   */ 
  bool
  Send (Ptr<Packet> p);

  /**
   * \brief Receive packet from application or another node and forward it to the CCNx stack
   *
   * \todo The only reason for this call is to handle tracing, if requested
   */
  bool
  Receive (const Ptr<const Packet> &p);
  ////////////////////////////////////////////////////////////////////

  /**
   * \Brief Assign routing/forwarding metric with face
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
   * \brief Enable or disable this face
   */
  virtual void
  SetUp (bool up = true);

  /**
   * \brief Returns true if this face is enabled, false otherwise.
   */
  virtual bool
  IsUp () const;

  /**
   * @brief Print information about the face into the stream
   * @param os stream to write information to
   */
  virtual std::ostream&
  Print (std::ostream &os) const;

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
   * @brief Set maximum value for Interest allowance
   *
   * @param bucket maximum value for Interest allowance. If < 0, then limit will be disabled
   */
  void
  SetBucketMax (double bucket);

  /**
   * @brief Set a normalized value (one second) for Interest allowance bucket leak
   */
   void
  SetBucketLeak (double leak);
  
  /**
   * @brief Leak the Interest allowance bucket by (1/interval) * m_bucketMax amount,
   * where interval is time between two consecutive calls of LeakBucket
   */
  void
  LeakBucket ();

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
  inline bool
  operator!= (const CcnxFace &face) const;
  
  /**
   * \brief Compare two faces. Only two faces on the same node could be compared.
   *
   * Internal index is used for comparison.
   */
  bool
  operator< (const CcnxFace &face) const;

protected:
  /**
   * \brief Send packet on a face (actual implementation)
   *
   * \param p smart pointer to a packet to send
   */
  virtual bool
  SendImpl (Ptr<Packet> p) = 0;  

private:
  CcnxFace (const CcnxFace &); ///< \brief Disabled copy constructor
  CcnxFace& operator= (const CcnxFace &); ///< \brief Disabled copy operator
  
protected:
  // uint16_t m_metric; ///< \brief Routing/forwarding metric
  Ptr<Node> m_node; ///< \brief Smart pointer to Node

  double m_bucket; ///< \brief Value representing current size of the Interest allowance for this face
  double m_bucketMax;  ///< \brief Maximum Interest allowance for this face
  double m_bucketLeak; ///< \brief Normalized amount that should be leaked every second
  
private:
  ProtocolHandler m_protocolHandler; ///< Callback via which packets are getting send to CCNx stack
  bool m_ifup; ///< \brief flag indicating that the interface is UP 
  uint32_t m_id; ///< \brief id of the interface in CCNx stack (per-node uniqueness)
  Time m_lastLeakTime;
  uint32_t m_metric; ///< \brief metric of the face
  bool m_randomizeLimitChecking;

  // bool m_enableMetricTagging;

  TracedCallback<Ptr<const Packet> > m_ccnxTxTrace;
  TracedCallback<Ptr<const Packet> > m_ccnxRxTrace;
  TracedCallback<Ptr<const Packet> > m_ccnxDropTrace;
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

inline bool
CcnxFace::operator!= (const CcnxFace &face) const
{
  return !(*this == face);
}


} // namespace ns3

#endif //CCNX_FACE_H
