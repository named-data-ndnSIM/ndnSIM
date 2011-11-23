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
  /**
   * \brief Default constructor
   */
  CcnxLocalFace (Ptr<CcnxApp> app);
  virtual ~CcnxLocalFace();
  
  ////////////////////////////////////////////////////////////////////
  // methods overloaded from CcnxFace
  virtual void
  RegisterProtocolHandler (ProtocolHandler handler);

protected:
  virtual void
  SendImpl (Ptr<Packet> p);

public:
  virtual std::ostream&
  Print (std::ostream &os) const;
  ////////////////////////////////////////////////////////////////////
 
private:
  CcnxLocalFace (const CcnxLocalFace &); ///< \brief Disabled copy constructor
  CcnxLocalFace& operator= (const CcnxLocalFace &); ///< \brief Disabled copy operator

private:
  Ptr<CcnxApp> m_application;
};

std::ostream& operator<< (std::ostream& os, const CcnxLocalFace &localFace);

} // namespace ns3

#endif
