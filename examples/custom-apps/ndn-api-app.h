/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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

#ifndef NDN_EXAMPLES_API_APP_H
#define NDN_EXAMPLES_API_APP_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

#include "ns3/ndnSIM/ndn.cxx/ndn-api-face.h"

namespace ns3 {
namespace ndn {

class ApiApp : public Application
{
public:
  static TypeId
  GetTypeId ();

  ApiApp ();

private:
  void
  RequestData ();

  void
  GotData (Ptr<const Interest> origInterest, Ptr<const Data> data);
  
protected:
  // inherited from Application base class.
  virtual void
  StartApplication ();

  virtual void
  StopApplication ();
  
private:
  Ptr<ApiFace> m_face;

  Name m_name;
  Time m_interestLifetime;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_EXAMPLES_API_APP_H
