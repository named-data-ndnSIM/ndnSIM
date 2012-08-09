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

#ifndef NDN_PRODUCER_H
#define NDN_PRODUCER_H

#include "ndn-app.h"

#include "ns3/ptr.h"
#include "ns3/ndn-name-components.h"
#include "ns3/ndn-content-object-header.h"

namespace ns3 
{

/**
 * @brief A simple Interest-sink applia simple Interest-sink application
 *
 * A simple Interest-sink applia simple Interest-sink application,
 * which replying every incoming Interest with Data packet with a specified
 * size and name same as in Interest.cation, which replying every incoming Interest
 * with Data packet with a specified size and name same as in Interest.
 */
class NdnProducer : public NdnApp
{
public: 
  static TypeId
  GetTypeId (void);
        
  NdnProducer ();

  // inherited from NdnApp
  void OnInterest (const Ptr<const NdnInterestHeader> &interest, Ptr<Packet> packet);

protected:
  // inherited from Application base class.
  virtual void
  StartApplication ();    // Called at time specified by Start

  virtual void
  StopApplication ();     // Called at time specified by Stop

private:
  NdnNameComponents m_prefix;
  uint32_t m_virtualPayloadSize;
  
  uint32_t m_signatureBits;
  // NdnContentObjectHeader::SignedInfo m_signedInfo;
};

}

#endif // NDN_PRODUCER_H
