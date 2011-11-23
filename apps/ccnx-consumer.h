/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef CCNX_CONSUMER_H
#define CCNX_CONSUMER_H

#include "ccnx-app.h"

namespace ns3 
{

class CcnxConsumer: public CcnxApp
{
public: 
  static TypeId GetTypeId ();
        
  CcnxConsumer ();

  void OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                        const Ptr<const Packet> &payload);

protected:
  // from CcnxApp
  virtual void
  StartApplication ();

  virtual void
  StopApplication ();
  
private:
  //helpers
  void SendPacket ();
     
private:
  UniformVariable m_rand;
  uint32_t        m_seq;
  EventId         m_sendEvent; // Eventid of pending "send packet" event

  Time               m_offTime;
  CcnxNameComponents m_interestName;
  Time               m_interestLifeTime;
  int32_t            m_minSuffixComponents;
  int32_t            m_maxSuffixComponents;
  bool               m_childSelector;
  CcnxNameComponents m_exclude;
};

} // namespace ns3

#endif
