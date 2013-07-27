/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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

#ifndef NDN_CONTENT_STORE_WITH_STATS_H_
#define NDN_CONTENT_STORE_WITH_STATS_H_

#include "content-store-impl.h"

#include "../../utils/trie/multi-policy.h"
#include "custom-policies/lifetime-stats-policy.h"

namespace ns3 {
namespace ndn {
namespace cs {

/**
 * @ingroup ndn-cs
 * @brief Special content store realization that provides ability to track stats of CS operations
 */
template<class Policy>
class ContentStoreWithStats :
    public ContentStoreImpl< ndnSIM::multi_policy_traits< boost::mpl::vector2< Policy, ndnSIM::lifetime_stats_policy_traits > > >
{
public:
  typedef ContentStoreImpl< ndnSIM::multi_policy_traits< boost::mpl::vector2< Policy, ndnSIM::lifetime_stats_policy_traits > > > super;

  typedef typename super::policy_container::template index<1>::type lifetime_stats_container;

  ContentStoreWithStats ()
  {
    // connect traceback to the policy
    super::getPolicy ().template get<1> ().set_traced_callback (&m_willRemoveEntry);
  }

  static TypeId
  GetTypeId ();

  virtual inline void
  Print (std::ostream &os) const;

private:
  static LogComponent g_log; ///< @brief Logging variable

  /// @brief trace of for entry removal: first parameter is pointer to the CS entry, second is how long entry was in the cache
  TracedCallback< Ptr<const Entry>, Time > m_willRemoveEntry;
};

//////////////////////////////////////////
////////// Implementation ////////////////
//////////////////////////////////////////


template<class Policy>
LogComponent
ContentStoreWithStats< Policy >::g_log = LogComponent (("ndn.cs.Stats." + Policy::GetName ()).c_str ());


template<class Policy>
TypeId
ContentStoreWithStats< Policy >::GetTypeId ()
{
  static TypeId tid = TypeId (("ns3::ndn::cs::Stats::"+Policy::GetName ()).c_str ())
    .SetGroupName ("Ndn")
    .SetParent<super> ()
    .template AddConstructor< ContentStoreWithStats< Policy > > ()

    .AddTraceSource ("WillRemoveEntry", "Trace called just before content store entry will be removed",
                     MakeTraceSourceAccessor (&ContentStoreWithStats< Policy >::m_willRemoveEntry))

    // trace stuff here
    ;

  return tid;
}

template<class Policy>
void
ContentStoreWithStats< Policy >::Print (std::ostream &os) const
{
  // const freshness_policy_container &freshness = this->getPolicy ().template get<freshness_policy_container> ();

  for (typename super::policy_container::const_iterator item = this->getPolicy ().begin ();
       item != this->getPolicy ().end ();
       item++)
    {
      Time alive = lifetime_stats_container::policy_base::get_time (&(*item)) - Simulator::Now ();
      os << item->payload ()->GetName () << "(alive: " << alive.ToDouble (Time::S) << "s)" << std::endl;
    }
}



} // namespace cs
} // namespace ndn
} // namespace ns3

#endif // NDN_CONTENT_STORE_IMPL_H_
