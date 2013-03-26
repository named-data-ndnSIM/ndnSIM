/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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

#include "ndn-pit-impl.h"

#include "../../utils/trie/empty-policy.h"
#include "../../utils/trie/persistent-policy.h"
#include "../../utils/trie/random-policy.h"
#include "../../utils/trie/lru-policy.h"
#include "../../utils/trie/multi-policy.h"
#include "../../utils/trie/aggregate-stats-policy.h"

#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("ndn.pit.PitImpl");

#include "custom-policies/serialized-size-policy.h"

#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/simulator.h"

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>


using namespace boost::tuples;
using namespace boost;
namespace ll = boost::lambda;

#define NS_OBJECT_ENSURE_REGISTERED_TEMPL(type, templ)  \
  static struct X ## type ## templ ## RegistrationClass \
  {                                                     \
    X ## type ## templ ## RegistrationClass () {        \
      ns3::TypeId tid = type<templ>::GetTypeId ();      \
      tid.GetParent ();                                 \
    }                                                   \
  } x_ ## type ## templ ## RegistrationVariable

namespace ns3 {
namespace ndn {
namespace pit {

using namespace ndnSIM;

template<>
uint32_t
PitImpl<serialized_size_policy_traits>::GetCurrentSize () const
{
  return super::getPolicy ().get_current_space_used ();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// explicit instantiation and registering
template class PitImpl<persistent_policy_traits>;
template class PitImpl<random_policy_traits>;
template class PitImpl<lru_policy_traits>;
template class PitImpl<serialized_size_policy_traits>;

NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, persistent_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, random_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, lru_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, serialized_size_policy_traits);


typedef multi_policy_traits< boost::mpl::vector2< persistent_policy_traits,
                                                  aggregate_stats_policy_traits > > PersistentWithCountsTraits;
typedef multi_policy_traits< boost::mpl::vector2< random_policy_traits,
                                                  aggregate_stats_policy_traits > > RandomWithCountsTraits;
typedef multi_policy_traits< boost::mpl::vector2< lru_policy_traits,
                                                  aggregate_stats_policy_traits > > LruWithCountsTraits;
typedef multi_policy_traits< boost::mpl::vector2< serialized_size_policy_traits,
                                                  aggregate_stats_policy_traits > > SerializedSizeWithCountsTraits;

template class PitImpl<PersistentWithCountsTraits>;
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, PersistentWithCountsTraits);

template class PitImpl<RandomWithCountsTraits>;
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, RandomWithCountsTraits);

template class PitImpl<LruWithCountsTraits>;
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, LruWithCountsTraits);

template class PitImpl<SerializedSizeWithCountsTraits>;
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, SerializedSizeWithCountsTraits);

#ifdef DOXYGEN
// /**
//  * \brief PIT in which new entries will be rejected if PIT size reached its limit
//  */
class Persistent : public PitImpl<persistent_policy_traits> { };

/**
 * \brief PIT in which PIT reaches its limit, random entry (could be the newly created one) will be removed from PIT
 */
class Random : public PitImpl<random_policy_traits> { };

/**
 * \brief PIT in which  the least recently used entry (the oldest entry with minimum number of incoming faces)
 * will be removed when PIT size reached its limit
 */
class Lru : public PitImpl<lru_policy_traits> { };

/**
 * @brief A variant of persistent PIT implementation where size of PIT is based on size of interests in bytes (MaxSize parameter)
 */
class SerializedSize : public PitImpl<serialized_size_policy_traits> { };

#endif

} // namespace pit
} // namespace ndn
} // namespace ns3
