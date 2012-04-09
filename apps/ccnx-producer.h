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
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCNX_PRODUCER_H
#define CCNX_PRODUCER_H

#include "ccnx-app.h"

#include "ns3/ptr.h"
#include "ns3/ccnx-name-components.h"

namespace ns3 
{

class CcnxProducer : public CcnxApp
{
public: 
  static TypeId
  GetTypeId (void);
        
  CcnxProducer ();

  // inherited from CcnxApp
  void OnInterest (const Ptr<const CcnxInterestHeader> &interest, Ptr<Packet> packet);

protected:
  // inherited from Application base class.
  virtual void
  StartApplication ();    // Called at time specified by Start

  virtual void
  StopApplication ();     // Called at time specified by Stop

private:
  CcnxNameComponents m_prefix;
  uint32_t m_virtualPayloadSize;
};

}

#endif // CCNX_PRODUCER_H
