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

#include "ccnx-content-store-policies.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"

NS_LOG_COMPONENT_DEFINE ("CcnxContentStorePolicies");

namespace ns3
{

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// LRU policy
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (CcnxContentStoreLru);

TypeId 
CcnxContentStoreLru::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxContentStoreLru")
    .SetGroupName ("Ccnx")
    .SetParent< CcnxContentStore > ()
    .AddConstructor<CcnxContentStoreLru> ()
    .AddAttribute ("Size",
                   "Maximum number of packets that content storage can hold",
                   UintegerValue (100),
                   MakeUintegerAccessor (&CcnxContentStoreLru::SetMaxSize,
                                         &CcnxContentStoreLru::GetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

void
CcnxContentStoreLru::SetMaxSize (uint32_t maxSize)
{
  getPolicy ().set_max_size (maxSize);
}

uint32_t
CcnxContentStoreLru::GetMaxSize () const
{
  return getPolicy ().get_max_size ();
}

CcnxContentStoreLru::CcnxContentStoreLru ()
{
}
        
CcnxContentStoreLru::~CcnxContentStoreLru () 
{
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// RANDOM policy
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (CcnxContentStoreRandom);

TypeId 
CcnxContentStoreRandom::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxContentStoreRandom")
    .SetGroupName ("Ccnx")
    .SetParent< CcnxContentStore > ()
    .AddConstructor<CcnxContentStoreRandom> ()
    
    .AddAttribute ("Size",
                   "Maximum number of packets that content storage can hold",
                   UintegerValue (100),
                   MakeUintegerAccessor (&CcnxContentStoreRandom::SetMaxSize,
                                         &CcnxContentStoreRandom::GetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

void
CcnxContentStoreRandom::SetMaxSize (uint32_t maxSize)
{
  getPolicy ().set_max_size (maxSize);
}

uint32_t
CcnxContentStoreRandom::GetMaxSize () const
{
  return getPolicy ().get_max_size ();
}

CcnxContentStoreRandom::CcnxContentStoreRandom ()
{
}
        
CcnxContentStoreRandom::~CcnxContentStoreRandom () 
{
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// FIFO policy
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (CcnxContentStoreFifo);

TypeId 
CcnxContentStoreFifo::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxContentStoreFifo")
    .SetGroupName ("Ccnx")
    .SetParent< CcnxContentStore > ()
    .AddConstructor<CcnxContentStoreFifo> ()
    
    .AddAttribute ("Size",
                   "Maximum number of packets that content storage can hold",
                   UintegerValue (100),
                   MakeUintegerAccessor (&CcnxContentStoreFifo::SetMaxSize,
                                         &CcnxContentStoreFifo::GetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

void
CcnxContentStoreFifo::SetMaxSize (uint32_t maxSize)
{
  getPolicy ().set_max_size (maxSize);
}

uint32_t
CcnxContentStoreFifo::GetMaxSize () const
{
  return getPolicy ().get_max_size ();
}

CcnxContentStoreFifo::CcnxContentStoreFifo ()
{
}
        
CcnxContentStoreFifo::~CcnxContentStoreFifo () 
{
}


} // namespace ns3

