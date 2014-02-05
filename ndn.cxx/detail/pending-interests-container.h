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
  {
  }

  virtual ~PendingInterestEntry ()
  {
  }

  void
  AddCallbacks (ApiFace::DataCallback onData, ApiFace::TimeoutCallback onTimeout)
  { 
    if (! onData.IsNull ())
      {
         m_dataCallbacks.push_back (onData);
      }
    if (! onTimeout.IsNull ())
      {
         m_timeoutCallbacks.push_back (onTimeout);
      }
  }

  void
  ClearCallbacks ()
  {
    m_dataCallbacks.clear ();
    m_timeoutCallbacks.clear ();
  }

  Ptr<const Interest>
  GetInterest () const
  {
    return m_interest;
  }

  void
  ProcessOnData (Ptr<const Interest> interest, Ptr<const Data> data)
  {
    for (std::list<ApiFace::DataCallback>::iterator i = m_dataCallbacks.begin ();
         i != m_dataCallbacks.end ();
         i++)
      {
        (*i) (interest, data);
      }
  }

  void
  ProcessOnTimeout (Ptr<const Interest> interest)
  {
    for (std::list<ApiFace::TimeoutCallback>::iterator i = m_timeoutCallbacks.begin ();
         i != m_timeoutCallbacks.end ();
         i++)
      {
        (*i) (interest);
      }
  }
  
private:
  std::list<ApiFace::DataCallback> m_dataCallbacks;
  std::list<ApiFace::TimeoutCallback> m_timeoutCallbacks;
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
