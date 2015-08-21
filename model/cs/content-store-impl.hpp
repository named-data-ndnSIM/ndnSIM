/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef NDN_CONTENT_STORE_IMPL_H_
#define NDN_CONTENT_STORE_IMPL_H_

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ndn-content-store.hpp"

#include "ns3/packet.h"
#include <boost/foreach.hpp>

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

#include "../../utils/trie/trie-with-policy.hpp"

namespace ns3 {
namespace ndn {
namespace cs {

/**
 * @ingroup ndn-cs
 * @brief Cache entry implementation with additional references to the base container
 */
template<class CS>
class EntryImpl : public Entry {
public:
  typedef Entry base_type;

public:
  EntryImpl(Ptr<ContentStore> cs, shared_ptr<const Data> data)
    : Entry(cs, data)
    , item_(0)
  {
  }

  void
  SetTrie(typename CS::super::iterator item)
  {
    item_ = item;
  }

  typename CS::super::iterator
  to_iterator()
  {
    return item_;
  }
  typename CS::super::const_iterator
  to_iterator() const
  {
    return item_;
  }

private:
  typename CS::super::iterator item_;
};

/**
 * @ingroup ndn-cs
 * @brief Base implementation of NDN content store
 */
template<class Policy>
class ContentStoreImpl
  : public ContentStore,
    protected ndnSIM::
      trie_with_policy<Name,
                       ndnSIM::smart_pointer_payload_traits<EntryImpl<ContentStoreImpl<Policy>>,
                                                            Entry>,
                       Policy> {
public:
  typedef ndnSIM::
    trie_with_policy<Name, ndnSIM::smart_pointer_payload_traits<EntryImpl<ContentStoreImpl<Policy>>,
                                                                Entry>,
                     Policy> super;

  typedef EntryImpl<ContentStoreImpl<Policy>> entry;

  static TypeId
  GetTypeId();

  ContentStoreImpl(){};
  virtual ~ContentStoreImpl(){};

  // from ContentStore

  virtual inline shared_ptr<Data>
  Lookup(shared_ptr<const Interest> interest);

  virtual inline bool
  Add(shared_ptr<const Data> data);

  // virtual bool
  // Remove (shared_ptr<Interest> header);

  virtual inline void
  Print(std::ostream& os) const;

  virtual uint32_t
  GetSize() const;

  virtual Ptr<Entry>
  Begin();

  virtual Ptr<Entry>
  End();

  virtual Ptr<Entry> Next(Ptr<Entry>);

  const typename super::policy_container&
  GetPolicy() const
  {
    return super::getPolicy();
  }

  typename super::policy_container&
  GetPolicy()
  {
    return super::getPolicy();
  }

public:
  typedef void (*CsEntryCallback)(Ptr<const Entry>);

private:
  void
  SetMaxSize(uint32_t maxSize);

  uint32_t
  GetMaxSize() const;

private:
  static LogComponent g_log; ///< @brief Logging variable

  /// @brief trace of for entry additions (fired every time entry is successfully added to the
  /// cache): first parameter is pointer to the CS entry
  TracedCallback<Ptr<const Entry>> m_didAddEntry;
};

//////////////////////////////////////////
////////// Implementation ////////////////
//////////////////////////////////////////

template<class Policy>
LogComponent ContentStoreImpl<Policy>::g_log = LogComponent(("ndn.cs." + Policy::GetName()).c_str(), __FILE__);

template<class Policy>
TypeId
ContentStoreImpl<Policy>::GetTypeId()
{
  static TypeId tid =
    TypeId(("ns3::ndn::cs::" + Policy::GetName()).c_str())
      .SetGroupName("Ndn")
      .SetParent<ContentStore>()
      .AddConstructor<ContentStoreImpl<Policy>>()
      .AddAttribute("MaxSize",
                    "Set maximum number of entries in ContentStore. If 0, limit is not enforced",
                    StringValue("100"), MakeUintegerAccessor(&ContentStoreImpl<Policy>::GetMaxSize,
                                                             &ContentStoreImpl<Policy>::SetMaxSize),
                    MakeUintegerChecker<uint32_t>())

      .AddTraceSource("DidAddEntry",
                      "Trace fired every time entry is successfully added to the cache",
                      MakeTraceSourceAccessor(&ContentStoreImpl<Policy>::m_didAddEntry),
                      "ns3::ndn::cs::ContentStoreImpl::CsEntryCallback");

  return tid;
}

struct isNotExcluded {
  inline isNotExcluded(const Exclude& exclude)
    : m_exclude(exclude)
  {
  }

  bool
  operator()(const name::Component& comp) const
  {
    return !m_exclude.isExcluded(comp);
  }

private:
  const Exclude& m_exclude;
};

template<class Policy>
shared_ptr<Data>
ContentStoreImpl<Policy>::Lookup(shared_ptr<const Interest> interest)
{
  NS_LOG_FUNCTION(this << interest->getName());

  typename super::const_iterator node;
  if (interest->getExclude().empty()) {
    node = this->deepest_prefix_match(interest->getName());
  }
  else {
    node = this->deepest_prefix_match_if_next_level(interest->getName(),
                                                    isNotExcluded(interest->getExclude()));
  }

  if (node != this->end()) {
    this->m_cacheHitsTrace(interest, node->payload()->GetData());

    shared_ptr<Data> copy = make_shared<Data>(*node->payload()->GetData());
    return copy;
  }
  else {
    this->m_cacheMissesTrace(interest);
    return 0;
  }
}

template<class Policy>
bool
ContentStoreImpl<Policy>::Add(shared_ptr<const Data> data)
{
  NS_LOG_FUNCTION(this << data->getName());

  Ptr<entry> newEntry = Create<entry>(this, data);
  std::pair<typename super::iterator, bool> result = super::insert(data->getName(), newEntry);

  if (result.first != super::end()) {
    if (result.second) {
      newEntry->SetTrie(result.first);

      m_didAddEntry(newEntry);
      return true;
    }
    else {
      // should we do anything?
      // update payload? add new payload?
      return false;
    }
  }
  else
    return false; // cannot insert entry
}

template<class Policy>
void
ContentStoreImpl<Policy>::Print(std::ostream& os) const
{
  for (typename super::policy_container::const_iterator item = this->getPolicy().begin();
       item != this->getPolicy().end(); item++) {
    os << item->payload ()->GetName () << std::endl;
  }
}

template<class Policy>
void
ContentStoreImpl<Policy>::SetMaxSize(uint32_t maxSize)
{
  this->getPolicy().set_max_size(maxSize);
}

template<class Policy>
uint32_t
ContentStoreImpl<Policy>::GetMaxSize() const
{
  return this->getPolicy().get_max_size();
}

template<class Policy>
uint32_t
ContentStoreImpl<Policy>::GetSize() const
{
  return this->getPolicy().size();
}

template<class Policy>
Ptr<Entry>
ContentStoreImpl<Policy>::Begin()
{
  typename super::parent_trie::recursive_iterator item(super::getTrie()), end(0);
  for (; item != end; item++) {
    if (item->payload() == 0)
      continue;
    break;
  }

  if (item == end)
    return End();
  else
    return item->payload();
}

template<class Policy>
Ptr<Entry>
ContentStoreImpl<Policy>::End()
{
  return 0;
}

template<class Policy>
Ptr<Entry>
ContentStoreImpl<Policy>::Next(Ptr<Entry> from)
{
  if (from == 0)
    return 0;

  typename super::parent_trie::recursive_iterator item(*StaticCast<entry>(from)->to_iterator()),
    end(0);

  for (item++; item != end; item++) {
    if (item->payload() == 0)
      continue;
    break;
  }

  if (item == end)
    return End();
  else
    return item->payload();
}

} // namespace cs
} // namespace ndn
} // namespace ns3

#endif // NDN_CONTENT_STORE_IMPL_H_
