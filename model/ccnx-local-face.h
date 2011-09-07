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
 * Authors:     Alexander Afanasyev <alexander.afanasyev@ucla.edu>
                Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef CCNX_LOCAL_FACE_H
#define CCNX_LOCAL_FACE_H

#include "ccnx-face.h"

namespace ns3 
{

class CcnxInterestHeader;
class CcnxContentObjectHeader;
class Packet;

/**
 * \ingroup ccnx-face
 * \brief Implementation of application CCNx face
 *
 * This class defines basic functionality of CCNx face. Face is core
 * component responsible for actual delivery of data packet to and
 * from CCNx stack
 *
 * \see CcnxLocalFace, CcnxNetDeviceFace, CcnxIpv4Face, CcnxUdpFace
 */
class CcnxLocalFace  : public CcnxFace
{
public:
  typedef Callback<void,const Ptr<const CcnxInterestHeader> &> InterestHandler;
  typedef Callback<void,const Ptr<const CcnxContentObjectHeader> &,
                        const Ptr<const Packet> &> ContentObjectHandler;

  /**
   * \brief Default constructor
   */
  CcnxLocalFace ();
  virtual ~CcnxLocalFace();
  
  ////////////////////////////////////////////////////////////////////
  // methods overloaded from CcnxFace

  /**
   * \brief This method should be called to establish link with lower layers
   */
  virtual void
  RegisterProtocolHandler (ProtocolHandler handler);

  /**
   * \brief This method will be called by lower layers to send data to *application*
   */
  virtual void
  Send (Ptr<Packet> p);

  virtual std::ostream&
  Print (std::ostream &os) const;
  ////////////////////////////////////////////////////////////////////

   /**
   * \brief This method will be called by higher layers to send data to *ccnx stack*
   */
  void ReceiveFromApplication (Ptr<Packet> p);

  /**
   * \brief Set callback to application
   *
   * \param onInterest InterestHandler to be called when interest arrives on the face. Will be disabled if 0
   */
  void SetInterestHandler (InterestHandler onInterest);

    /**
   * \brief Set callback to application
   *
   * \param onContentObject ContentObjectHandler to be called when data arrives. Will be disabled if 0
   */
  void SetContentObjectHandler (ContentObjectHandler onContentObject);
 
private:
  CcnxLocalFace (const CcnxLocalFace &); ///< \brief Disabled copy constructor
  CcnxLocalFace& operator= (const CcnxLocalFace &); ///< \brief Disabled copy operator

private:
  InterestHandler m_onInterest;
  ContentObjectHandler m_onContentObject;
};

std::ostream& operator<< (std::ostream& os, const CcnxLocalFace &localFace);

} // namespace ns3

#endif
