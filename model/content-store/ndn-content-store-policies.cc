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

#include "ndn-content-store-policies.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"

NS_LOG_COMPONENT_DEFINE ("NdnContentStorePolicies");

namespace ns3
{

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// LRU policy
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (NdnContentStoreLru);

TypeId 
NdnContentStoreLru::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdnContentStoreLru")
    .SetGroupName ("Ndn")
    .SetParent< NdnContentStore > ()
    .AddConstructor<NdnContentStoreLru> ()
    .AddAttribute ("Size",
                   "Maximum number of packets that content storage can hold",
                   UintegerValue (100),
                   MakeUintegerAccessor (&NdnContentStoreLru::SetMaxSize,
                                         &NdnContentStoreLru::GetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

void
NdnContentStoreLru::SetMaxSize (uint32_t maxSize)
{
  getPolicy ().set_max_size (maxSize);
}

uint32_t
NdnContentStoreLru::GetMaxSize () const
{
  return getPolicy ().get_max_size ();
}

NdnContentStoreLru::NdnContentStoreLru ()
{
}
        
NdnContentStoreLru::~NdnContentStoreLru () 
{
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// RANDOM policy
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (NdnContentStoreRandom);

TypeId 
NdnContentStoreRandom::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdnContentStoreRandom")
    .SetGroupName ("Ndn")
    .SetParent< NdnContentStore > ()
    .AddConstructor<NdnContentStoreRandom> ()
    
    .AddAttribute ("Size",
                   "Maximum number of packets that content storage can hold",
                   UintegerValue (100),
                   MakeUintegerAccessor (&NdnContentStoreRandom::SetMaxSize,
                                         &NdnContentStoreRandom::GetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

void
NdnContentStoreRandom::SetMaxSize (uint32_t maxSize)
{
  getPolicy ().set_max_size (maxSize);
}

uint32_t
NdnContentStoreRandom::GetMaxSize () const
{
  return getPolicy ().get_max_size ();
}

NdnContentStoreRandom::NdnContentStoreRandom ()
{
}
        
NdnContentStoreRandom::~NdnContentStoreRandom () 
{
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// FIFO policy
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (NdnContentStoreFifo);

TypeId 
NdnContentStoreFifo::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdnContentStoreFifo")
    .SetGroupName ("Ndn")
    .SetParent< NdnContentStore > ()
    .AddConstructor<NdnContentStoreFifo> ()
    
    .AddAttribute ("Size",
                   "Maximum number of packets that content storage can hold",
                   UintegerValue (100),
                   MakeUintegerAccessor (&NdnContentStoreFifo::SetMaxSize,
                                         &NdnContentStoreFifo::GetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

void
NdnContentStoreFifo::SetMaxSize (uint32_t maxSize)
{
  getPolicy ().set_max_size (maxSize);
}

uint32_t
NdnContentStoreFifo::GetMaxSize () const
{
  return getPolicy ().get_max_size ();
}

NdnContentStoreFifo::NdnContentStoreFifo ()
{
}
        
NdnContentStoreFifo::~NdnContentStoreFifo () 
{
}


} // namespace ns3

