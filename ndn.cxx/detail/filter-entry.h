/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU v3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_NDNCXX_DETAIL_FILTER_ENTRY_H
#define NDN_NDNCXX_DETAIL_FILTER_ENTRY_H

#include <ns3/ndnSIM/utils/trie/trie-with-policy.h>
#include <ns3/ndnSIM/utils/trie/counting-policy.h>

namespace ns3 {
namespace ndn {

template<class Callback, class Payload>
struct FilterEntry : public ns3::SimpleRefCount< FilterEntry<Callback, Payload> >
{
public:
  FilterEntry (Ptr<const Payload> payload)
    : m_payload (payload)
  { }
  
  void
  AddCallback (Callback callback)
  { 
    m_callback = callback;
  }

  void
  ClearCallback ()
  {
    m_callback = Callback ();
  }

  Ptr<const Payload>
  GetPayload () const
  {
    return m_payload;
  }
  
public:
  Callback m_callback;
  Ptr<const Payload> m_payload;
};


template<class Callback, class Payload>
struct FilterEntryContainer :
    public ns3::ndn::ndnSIM::trie_with_policy<ns3::ndn::Name,
                                              ns3::ndn::ndnSIM::smart_pointer_payload_traits< FilterEntry<Callback, Payload> >,
                                              ns3::ndn::ndnSIM::counting_policy_traits>
{
};

} // ndn
} // ns3

#endif // NDN_NDNCXX_DETAIL_FILTER_ENTRY_H
