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

#include "per-fib-limits.h"
#include "per-out-face-limits.h"

#include "ns3/ndn-l3-protocol.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"

#include "best-route.h"
#include "flooding.h"
#include "smart-flooding.h"

namespace ns3 {
namespace ndn {
namespace fw {

extern template class PerOutFaceLimits<BestRoute>;
extern template class PerOutFaceLimits<Flooding>;
extern template class PerOutFaceLimits<SmartFlooding>;

template class PerFibLimits< PerOutFaceLimits<BestRoute> >;
typedef PerFibLimits< PerOutFaceLimits<BestRoute> > PerFibLimitsPerOutFaceLimitsBestRoute;
NS_OBJECT_ENSURE_REGISTERED (PerFibLimitsPerOutFaceLimitsBestRoute);

template class PerFibLimits< PerOutFaceLimits<Flooding> >;
typedef PerFibLimits< PerOutFaceLimits<Flooding> > PerFibLimitsPerOutFaceLimitsFlooding;
NS_OBJECT_ENSURE_REGISTERED (PerFibLimitsPerOutFaceLimitsFlooding);

template class PerFibLimits< PerOutFaceLimits<SmartFlooding> >;
typedef PerFibLimits< PerOutFaceLimits<SmartFlooding> > PerFibLimitsPerOutFaceLimitsSmartFlooding;
NS_OBJECT_ENSURE_REGISTERED (PerFibLimitsPerOutFaceLimitsSmartFlooding);

#ifdef DOXYGEN
// /**
//  * \brief Strategy implementing per-fib-per-out-face limits on top of BestRoute strategy
//  */
class BestRoute::PerOutFaceLimits::PerFibLimits : public ::ns3::ndn::fw::PerFibLimits< ::ns3::ndn::fw::PerOutFaceLimits<BestRoute> > { };

/**
 * \brief Strategy implementing per-fib-per-out-face limits on top of Flooding strategy
 */
class Flooding::PerOutFaceLimits::PerFibLimits : public ::ns3::ndn::fw::PerFibLimits< ::ns3::ndn::fw::PerOutFaceLimits<Flooding> > { };

/**
 * \brief Strategy implementing per-fib-per-out-face limits on top of SmartFlooding strategy
 */
class SmartFlooding::PerOutFaceLimits::PerFibLimits : public ::ns3::ndn::fw::PerFibLimits< ::ns3::ndn::fw::PerOutFaceLimits<SmartFlooding> > { };

#endif

} // namespace fw
} // namespace ndn
} // namespace ns3
