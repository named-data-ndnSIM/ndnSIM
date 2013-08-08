/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

template class PerOutFaceLimits<BestRoute>;
typedef PerOutFaceLimits<BestRoute> PerOutFaceLimitsBestRoute;
NS_OBJECT_ENSURE_REGISTERED (PerOutFaceLimitsBestRoute);

template class PerOutFaceLimits<Flooding>;
typedef PerOutFaceLimits<Flooding> PerOutFaceLimitsFlooding;
NS_OBJECT_ENSURE_REGISTERED (PerOutFaceLimitsFlooding);

template class PerOutFaceLimits<SmartFlooding>;
typedef PerOutFaceLimits<SmartFlooding> PerOutFaceLimitsSmartFlooding;
NS_OBJECT_ENSURE_REGISTERED (PerOutFaceLimitsSmartFlooding);

#ifdef DOXYGEN
// /**
//  * \brief Strategy implementing per-out-face limits on top of BestRoute strategy
//  */
class BestRoute::PerOutFaceLimits : public ::ns3::ndn::fw::PerOutFaceLimits<BestRoute> { };

/**
 * \brief Strategy implementing per-out-face limits on top of Flooding strategy
 */
class Flooding::PerOutFaceLimits : public ::ns3::ndn::fw::PerOutFaceLimits<Flooding> { };

/**
 * \brief Strategy implementing per-out-face limits on top of SmartFlooding strategy
 */
class SmartFlooding::PerOutFaceLimits : public ::ns3::ndn::fw::PerOutFaceLimits<SmartFlooding> { };
#endif

} // namespace fw
} // namespace ndn
} // namespace ns3

