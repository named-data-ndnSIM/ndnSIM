/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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

// dumb-requester.h

#ifndef DUMB_REQUESTER_H_
#define DUMB_REQUESTER_H_

#include "ns3/ndn-app.h"
#include "ns3/ndn-name.h"

namespace ns3 {

/**
 * @brief A dumb requester application
 *
 * This app keeps requesting every second the same content object 
 */
class DumbRequester : public ndn::App
{
public:
  // register NS-3 type "DumbRequester"
  static TypeId
  GetTypeId ();

  DumbRequester ();
  
  // (overridden from ndn::App) Processing upon start of the application
  virtual void
  StartApplication ();

  // (overridden from ndn::App) Processing when application is stopped
  virtual void
  StopApplication ();

  // (overridden from ndn::App) Callback that will be called when Data arrives
  virtual void
  OnData (Ptr<const ndn::Data> contentObject);
  
private:
  void
  SendInterest ();

private:
  bool m_isRunning;
  ndn::Name m_name;
};

} // namespace ns3

#endif // DUMB_REQUESTER_H_
