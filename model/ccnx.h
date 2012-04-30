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

#ifndef _CCNX_H_
#define _CCNX_H_

#include "ns3/object.h"
#include "ns3/callback.h"
#include "ns3/traced-callback.h"

namespace ns3 {

class Node;
class NetDevice;
class Packet;
class CcnxForwardingStrategy;
class CcnxFace;
class CcnxContentObjectHeader;
class CcnxInterestHeader;
class CcnxPit;

/**
 * \internal
 * \brief Private namespace for CCNx content store implementation
 */
namespace __ccnx_private
{
class i_face {};
class i_metric {};
class i_nth {};
class i_prefix {};
class i_ordered {}; ///< tag for Boost.MultiIndex container (ordered by prefix)
class i_mru {};
}

// default data size
// #define NDN_DEFAULT_DATA_SIZE   1024
// #define NDN_INTEREST_RESET_PERIOD	(10*MILLI_SECOND)

/**
 * \defgroup ccnx NDN abstraction
 *
 * This is an abstract implementation of NDN protocol
 */
/**
 * \ingroup ccnx
 * \brief Interface to manage Ccnx stack
 *
 * This class defines the API to manipulate the following aspects of
 * the Ccnx stack implementation:
 * -# register a face (CcnxFace-derived object) for use by the Ccnx
 *    layer
 * -# register forwarding strategy (CcnxForwardingStrategy-derived
 *    object) to use by Ccnx stack
 * -# export Ccnx configuration attributes
 * 
 * Each CcnxFace-derived object has conceptually a single Ccnx
 * interface associated with it.
 *
 * In addition, this class defines CCNx packet coding constants
 *
 * \see CcnxFace, CcnxForwardingStrategy
 */
class Ccnx : public Object
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();
  virtual ~Ccnx ();

  /**
   * \brief Register a new forwarding strategy to be used by this Ccnx
   * stack
   *
   * This call will replace any forwarding strategy that has been
   * previously registered.
   * 
   * \param forwardingStrategy smart pointer to CcnxForwardingStrategy
   * object
   */
  virtual void
  SetForwardingStrategy (Ptr<CcnxForwardingStrategy> forwardingStrategy) = 0;

  /**
   * \brief Get the forwarding strategy being used by this Ccnx stack
   * 
   * \returns smart pointer to CcnxForwardingStrategy object, or null
   * pointer if none
   */
  virtual Ptr<CcnxForwardingStrategy>
  GetForwardingStrategy (void) const = 0;

  /**
   * \brief Add face to CCNx stack
   *
   * \param face smart pointer to CcnxFace-derived object
   * (CcnxLocalFace, CcnxNetDeviceFace, CcnxUdpFace) \returns the
   * index of the Ccnx interface added.
   * 
   * \see CcnxLocalFace, CcnxNetDeviceFace, CcnxUdpFace
   */
  virtual uint32_t
  AddFace (const Ptr<CcnxFace> &face) = 0;

  /**
   * \brief Get current number of faces added to CCNx stack
   *
   * \returns the number of faces
   */
  virtual uint32_t
  GetNFaces (void) const = 0;

  /**
   * \brief Get face by face index
   * \param face The face number of an Ccnx interface.
   * \returns The CcnxFace associated with the Ccnx face number.
   */
  virtual Ptr<CcnxFace>
  GetFace (uint32_t face) const = 0;

  /**
   * \brief Remove face from ccnx stack (remove callbacks)
   */
  virtual void
  RemoveFace (Ptr<CcnxFace> face) = 0;

  /**
   * \brief Get face for NetDevice
   */
  virtual Ptr<CcnxFace>
  GetFaceByNetDevice (Ptr<NetDevice> netDevice) const = 0;

  /**
   * \brief Get PIT associated with Node 
   */
  virtual Ptr<CcnxPit>
  GetPit() const = 0;

  /**
   * \enum DropReason
   * \brief A reason why the packet has been dropped
   */
  enum DropReason
    {
      DUPLICATED, // Interests
      SUPPRESSED, // Interests and Nacks
      NO_FACES,    // Interests
      NON_DUPLICATED, // Nacks
      AFTER_SATISFIED, // Nacks
      UNSOLICITED      // data
    };
  
protected:
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  
  // transmittedInterestTrace is inside ForwardingStrategy
  
  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<const CcnxFace> > m_inInterests;

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 DropReason,
                 Ptr<const CcnxFace> > m_dropInterests;
  
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<const CcnxFace> > m_outNacks;

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<const CcnxFace> > m_inNacks;

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 DropReason,
                 Ptr<const CcnxFace> > m_dropNacks;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  TracedCallback<Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                 bool /*from cache*/,
                 Ptr<const CcnxFace> > m_outData;

  TracedCallback<Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                 Ptr<const CcnxFace> > m_inData;

  TracedCallback<Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                  DropReason,
                  Ptr<const CcnxFace> > m_dropData;  
};

} // namespace ns3 

#endif /* _CCNX_H_ */
