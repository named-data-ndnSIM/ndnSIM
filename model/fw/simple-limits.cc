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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "simple-limits.h"

#include "ns3/ndn-l3-protocol.h"
#include "ns3/ndn-interest-header.h"
#include "ns3/ndn-content-object-header.h"
#include "ns3/ndn-pit.h"
#include "ns3/ndn-pit-entry.h"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/double.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("ndn.fw.SimpleLimits");

namespace ns3 {
namespace ndn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED (SimpleLimits);
  
TypeId
SimpleLimits::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::SimpleLimits")
    .SetGroupName ("Ndn")
    .SetParent <super> ()
    .AddConstructor <SimpleLimits> ()
    ;
  return tid;
}
    
SimpleLimits::SimpleLimits ()
{
}

void
SimpleLimits::DoDispose ()
{
  super::DoDispose ();
}

void
SimpleLimits::NotifyNewAggregate ()
{
  super::NotifyNewAggregate ();
}

bool
SimpleLimits::TrySendOutInterest (Ptr<Face> inFace,
                                  Ptr<Face> outFace,
                                  Ptr<const InterestHeader> header,
                                  Ptr<const Packet> origPacket,
                                  Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  // totally override all (if any) parent processing
  
  pit::Entry::out_iterator outgoing =
    pitEntry->GetOutgoing ().find (outFace);

  if (outgoing != pitEntry->GetOutgoing ().end ())
    {
      // just suppress without any other action
      return false;
    }

  NS_LOG_DEBUG ("Limit: " << outFace->GetLimits ().m_curMaxLimit << ", outstanding: " << outFace->GetLimits ().m_outstanding);
  
  if (outFace->GetLimits ().IsBelowLimit ())
    {
      pitEntry->AddOutgoing (outFace);

      //transmission
      Ptr<Packet> packetToSend = origPacket->Copy ();
      outFace->Send (packetToSend);

      DidSendOutInterest (outFace, header, origPacket, pitEntry);      
      return true;
    }
  else
    {
      NS_LOG_DEBUG ("Face limit for " << header->GetName ());
    }

  return false;
}

void
SimpleLimits::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ().RemoveOutstanding ();
    }
}


void
SimpleLimits::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                          Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  super::WillSatisfyPendingInterest (inFace, pitEntry);

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ().RemoveOutstanding ();
    }
}



} // namespace fw
} // namespace ndn
} // namespace ns3
