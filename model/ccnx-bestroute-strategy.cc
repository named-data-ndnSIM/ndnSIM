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

#include "ccnx-bestroute-strategy.h"
#include "ns3/assert.h"

#include "ccnx-route.h"


NS_LOG_COMPONENT_DEFINE ("CcnxBestRouteStrategy");
namespace __ccnx_private {
    
    struct CcnxFibFaceMetricByFace;
}

namespace ns3 
{
    
using namespace __ccnx_private;
    
NS_OBJECT_ENSURE_REGISTERED (CcnxBestRouteStrategy);
  
TypeId CcnxBestRouteStrategy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxBestRouteStrategy")
  .SetGroupName ("Ccnx")
  .SetParent<Object> ()
  ;
  return tid;
}
    
CcnxBestRouteStrategy::CcnxBestRouteStrategy ()
{
}
    
    
bool
CcnxBestRouteStrategy::PropagateInterest (CcnxPitEntryContainer::type::iterator pitEntry, 
                                          CcnxFibEntryContainer::type::iterator fibEntry,
                                          const Ptr<CcnxFace> &incomingFace,
                                          Ptr<CcnxInterestHeader> &header,
                                          const Ptr<const Packet> &packet,
                                          SendCallback ucb)
{
  NS_LOG_FUNCTION(this);
  NS_LOG_INFO(*fibEntry);
    
  Ptr<CcnxFace> bestFace = fibEntry->FindBestCandidate(0);
   
  if( bestFace == NULL )
    {
      return false;
    }
  else
    {
      bool tryResult = GetPit ()->TryAddOutgoing (pitEntry, bestFace);
      if (tryResult == false)
      {
        return false;
      }
          
      ucb (bestFace, header, packet->Copy());
    }
     
  return true;
}
    
} //namespace ns3
