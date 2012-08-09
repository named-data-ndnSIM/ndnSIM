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
 * Authors:     Alexander Afanasyev <alexander.afanasyev@ucla.edu>
                Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef NDN_APP_FACE_H
#define NDN_APP_FACE_H

#include "ndn-face.h"
#include "ns3/traced-callback.h"

namespace ns3 
{

class NdnInterestHeader;
class NdnContentObjectHeader;
class Packet;
class NdnApp;

/**
 * \ingroup ndn-face
 * \brief Implementation of application Ndn face
 *
 * This class defines basic functionality of Ndn face. Face is core
 * component responsible for actual delivery of data packet to and
 * from Ndn stack
 *
 * \see NdnAppFace, NdnNetDeviceFace, NdnIpv4Face, NdnUdpFace
 */
class NdnAppFace  : public NdnFace
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief Default constructor
   */
  NdnAppFace (Ptr<NdnApp> app);
  virtual ~NdnAppFace();
  
  ////////////////////////////////////////////////////////////////////
  // methods overloaded from NdnFace
  virtual void
  RegisterProtocolHandler (ProtocolHandler handler);

protected:
  virtual bool
  SendImpl (Ptr<Packet> p);

public:
  virtual std::ostream&
  Print (std::ostream &os) const;
  ////////////////////////////////////////////////////////////////////
 
private:
  NdnAppFace ();
  NdnAppFace (const NdnAppFace &); ///< \brief Disabled copy constructor
  NdnAppFace& operator= (const NdnAppFace &); ///< \brief Disabled copy operator

private:
  Ptr<NdnApp> m_app;
};

} // namespace ns3

#endif // NDN_APP_FACE_H
