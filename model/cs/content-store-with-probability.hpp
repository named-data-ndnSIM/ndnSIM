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

#ifndef NDN_CONTENT_STORE_WITH_PROBABILITY_H_
#define NDN_CONTENT_STORE_WITH_PROBABILITY_H_

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "content-store-impl.hpp"

#include "../../utils/trie/multi-policy.hpp"
#include "custom-policies/probability-policy.hpp"
#include "ns3/double.h"
#include "ns3/type-id.h"

namespace ns3 {
namespace ndn {
namespace cs {

/**
 * @ingroup ndn-cs
 * @brief Special content store realization that probabilistically accepts data packet
 *        into CS (placement policy)
 */
template<class Policy>
class ContentStoreWithProbability
  : public ContentStoreImpl<ndnSIM::multi_policy_traits<boost::mpl::
                                                          vector2<ndnSIM::probability_policy_traits,
                                                                  Policy>>> {
public:
  typedef ContentStoreImpl<ndnSIM::multi_policy_traits<boost::mpl::
                                                         vector2<ndnSIM::probability_policy_traits,
                                                                 Policy>>> super;

  typedef typename super::policy_container::template index<0>::type probability_policy_container;

  ContentStoreWithProbability(){};

  static TypeId
  GetTypeId();

private:
  void
  SetCacheProbability(double probability)
  {
    this->getPolicy().template get<probability_policy_container>().set_probability(probability);
  }

  double
  GetCacheProbability() const
  {
    return this->getPolicy().template get<probability_policy_container>().get_probability();
  }
};

//////////////////////////////////////////
////////// Implementation ////////////////
//////////////////////////////////////////

template<class Policy>
TypeId
ContentStoreWithProbability<Policy>::GetTypeId()
{
  static TypeId tid =
    TypeId(("ns3::ndn::cs::Probability::" + Policy::GetName()).c_str())
      .SetGroupName("Ndn")
      .SetParent<super>()
      .template AddConstructor<ContentStoreWithProbability<Policy>>()

      .AddAttribute("CacheProbability",
                    "Set probability of caching in ContentStore. "
                    "If 1, every content is cached. If 0, no content is cached.",
                    DoubleValue(1.0), //(+)
                    MakeDoubleAccessor(&ContentStoreWithProbability<Policy>::GetCacheProbability,
                                       &ContentStoreWithProbability<Policy>::SetCacheProbability),
                    MakeDoubleChecker<double>());

  return tid;
}

} // namespace cs
} // namespace ndn
} // namespace ns3

#endif // NDN_CONTENT_STORE_WITH_PROBABILITY_H_
