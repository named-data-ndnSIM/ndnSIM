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

namespace ns3 {

class Packet;

namespace ndn {

class Interest;
class Data;

typedef Interest InterestHeader;
typedef Data DataHeader;

class App;

/**
 * \ingroup ndn-face
 * \brief Implementation of application Ndn face
 *
 * This class defines basic functionality of Ndn face. Face is core
 * component responsible for actual delivery of data packet to and
 * from Ndn stack
 *
 * \see AppFace, NdnNetDeviceFace, NdnIpv4Face, NdnUdpFace
 */
class AppFace  : public Face
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief Default constructor
   */
  AppFace (Ptr<App> app);
  virtual ~AppFace();
  
  ////////////////////////////////////////////////////////////////////
  // methods overloaded from Face
  virtual bool
  SendInterest (Ptr<const Interest> interest);

  virtual bool
  SendData (Ptr<const Data> data);

public:
  virtual std::ostream&
  Print (std::ostream &os) const;
  ////////////////////////////////////////////////////////////////////
 
private:
  AppFace ();
  AppFace (const AppFace &); ///< \brief Disabled copy constructor
  AppFace& operator= (const AppFace &); ///< \brief Disabled copy operator

private:
  Ptr<App> m_app;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_APP_FACE_H
