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

#include "content-store-impl.hpp"

#include "../../utils/trie/random-policy.hpp"
#include "../../utils/trie/lru-policy.hpp"
#include "../../utils/trie/fifo-policy.hpp"
#include "../../utils/trie/lfu-policy.hpp"
#include "../../utils/trie/multi-policy.hpp"
#include "../../utils/trie/aggregate-stats-policy.hpp"

#define NS_OBJECT_ENSURE_REGISTERED_TEMPL(type, templ)                                             \
  static struct X##type##templ##RegistrationClass {                                                \
    X##type##templ##RegistrationClass()                                                            \
    {                                                                                              \
      ns3::TypeId tid = type<templ>::GetTypeId();                                                  \
      tid.GetParent();                                                                             \
    }                                                                                              \
  } x_##type##templ##RegistrationVariable

namespace ns3 {
namespace ndn {

using namespace ndnSIM;

namespace cs {

// explicit instantiation and registering
/**
 * @brief ContentStore with LRU cache replacement policy
 **/
template class ContentStoreImpl<lru_policy_traits>;

/**
 * @brief ContentStore with random cache replacement policy
 **/
template class ContentStoreImpl<random_policy_traits>;

/**
 * @brief ContentStore with FIFO cache replacement policy
 **/
template class ContentStoreImpl<fifo_policy_traits>;

/**
 * @brief ContentStore with Least Frequently Used (LFU) cache replacement policy
 **/
template class ContentStoreImpl<lfu_policy_traits>;

NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, lru_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, random_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, fifo_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, lfu_policy_traits);

typedef multi_policy_traits<boost::mpl::vector2<lru_policy_traits, aggregate_stats_policy_traits>>
  LruWithCountsTraits;
typedef multi_policy_traits<boost::mpl::vector2<random_policy_traits,
                                                aggregate_stats_policy_traits>>
  RandomWithCountsTraits;
typedef multi_policy_traits<boost::mpl::vector2<fifo_policy_traits, aggregate_stats_policy_traits>>
  FifoWithCountsTraits;
typedef multi_policy_traits<boost::mpl::vector2<lfu_policy_traits, aggregate_stats_policy_traits>>
  LfuWithCountsTraits;

template class ContentStoreImpl<LruWithCountsTraits>;
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, LruWithCountsTraits);

template class ContentStoreImpl<RandomWithCountsTraits>;
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, RandomWithCountsTraits);

template class ContentStoreImpl<FifoWithCountsTraits>;
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, FifoWithCountsTraits);

template class ContentStoreImpl<LfuWithCountsTraits>;
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, LfuWithCountsTraits);

#ifdef DOXYGEN
// /**
//  * \brief Content Store implementing LRU cache replacement policy
//  */
class Lru : public ContentStoreImpl<lru_policy_traits> {
};

/**
 * \brief Content Store implementing FIFO cache replacement policy
 */
class Fifo : public ContentStoreImpl<fifo_policy_traits> {
};

/**
 * \brief Content Store implementing Random cache replacement policy
 */
class Random : public ContentStoreImpl<random_policy_traits> {
};

/**
 * \brief Content Store implementing Least Frequently Used cache replacement policy
 */
class Lfu : public ContentStoreImpl<lfu_policy_traits> {
};
#endif

} // namespace cs
} // namespace ndn
} // namespace ns3
