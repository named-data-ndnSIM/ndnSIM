/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU v3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_NDNCXX_DETAIL_PENDING_INTEREST_CONTAINER_H
#define NDN_NDNCXX_DETAIL_PENDING_INTEREST_CONTAINER_H

#include <ns3/ndnSIM/utils/trie/trie-with-policy.h>
#include "timeouts-policy.h"

namespace ns3 {
namespace ndn {
namespace detail {

struct PendingInterestEntry : public SimpleRefCount< PendingInterestEntry >
{
public:
  PendingInterestEntry (Ptr<const Interest> interest)
    : m_interest (interest)
  { }
  
  void
  AddCallbacks (ApiFace::DataCallback onData, ApiFace::TimeoutCallback onTimeout)
  { 
    m_dataCallback = onData;
    m_timeoutCallback = onTimeout;
  }

  void
  ClearCallbacks ()
  {
    m_dataCallback = ApiFace::DataCallback ();
    m_timeoutCallback = ApiFace::TimeoutCallback ();
  }

  Ptr<const Interest>
  GetInterest () const
  {
    return m_interest;
  }
  
public:
  ApiFace::DataCallback m_dataCallback;
  ApiFace::TimeoutCallback m_timeoutCallback;
  Ptr<const Interest> m_interest;
};


struct PendingInterestContainer :
    public ndnSIM::trie_with_policy<Name,
                                    ndnSIM::smart_pointer_payload_traits< PendingInterestEntry >,
                                    timeouts_policy_traits>
{
};

} // detail
} // ndn
} // ns3

#endif // NDN_NDNCXX_DETAIL_PENDING_INTEREST_CONTAINER_H
