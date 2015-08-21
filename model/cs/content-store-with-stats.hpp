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

#ifndef NDN_CONTENT_STORE_WITH_STATS_H_
#define NDN_CONTENT_STORE_WITH_STATS_H_

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "content-store-impl.hpp"

#include "../../utils/trie/multi-policy.hpp"
#include "custom-policies/lifetime-stats-policy.hpp"

namespace ns3 {
namespace ndn {
namespace cs {

/**
 * @ingroup ndn-cs
 * @brief Special content store realization that provides ability to track stats of CS operations
 */
template<class Policy>
class ContentStoreWithStats
  : public ContentStoreImpl<ndnSIM::
                              multi_policy_traits<boost::mpl::
                                                    vector2<Policy,
                                                            ndnSIM::
                                                              lifetime_stats_policy_traits>>> {
public:
  typedef ContentStoreImpl<ndnSIM::
                             multi_policy_traits<boost::mpl::
                                                   vector2<Policy,
                                                           ndnSIM::lifetime_stats_policy_traits>>>
    super;

  typedef typename super::policy_container::template index<1>::type lifetime_stats_container;

  ContentStoreWithStats()
  {
    // connect traceback to the policy
    super::getPolicy().template get<1>().set_traced_callback(&m_willRemoveEntry);
  }

  static TypeId
  GetTypeId();

  virtual inline void
  Print(std::ostream& os) const;

public:
  typedef void (*RemoveCsEntryCallback)(Ptr<const Entry>, Time);

private:
  static LogComponent g_log; ///< @brief Logging variable

  /// @brief trace of for entry removal: first parameter is pointer to the CS entry, second is how
  /// long entry was in the cache
  TracedCallback<Ptr<const Entry>, Time> m_willRemoveEntry;
};

//////////////////////////////////////////
////////// Implementation ////////////////
//////////////////////////////////////////

template<class Policy>
LogComponent ContentStoreWithStats<Policy>::g_log = LogComponent(("ndn.cs.Stats."
                                                                  + Policy::GetName()).c_str(), __FILE__);

template<class Policy>
TypeId
ContentStoreWithStats<Policy>::GetTypeId()
{
  static TypeId tid =
    TypeId(("ns3::ndn::cs::Stats::" + Policy::GetName()).c_str())
      .SetGroupName("Ndn")
      .SetParent<super>()
      .template AddConstructor<ContentStoreWithStats<Policy>>()

      .AddTraceSource("WillRemoveEntry",
                      "Trace called just before content store entry will be removed",
                      MakeTraceSourceAccessor(&ContentStoreWithStats<Policy>::m_willRemoveEntry),
                      "ns3::ndn::cs::ContentStoreWithStats::RemoveCsEntryCallback")

    // trace stuff here
    ;

  return tid;
}

template<class Policy>
void
ContentStoreWithStats<Policy>::Print(std::ostream& os) const
{
  // const freshness_policy_container &freshness = this->getPolicy ().template
  // get<freshness_policy_container> ();

  for (typename super::policy_container::const_iterator item = this->getPolicy().begin();
       item != this->getPolicy().end(); item++) {
    Time alive = lifetime_stats_container::policy_base::get_time(&(*item)) - Simulator::Now();
    os << item->payload()->GetName() << "(alive: " << alive.ToDouble(Time::S) << "s)" << std::endl;
  }
}

} // namespace cs
} // namespace ndn
} // namespace ns3

#endif // NDN_CONTENT_STORE_IMPL_H_
