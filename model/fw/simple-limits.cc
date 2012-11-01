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

#include "simple-limits.h"

#include "ns3/ndn-l3-protocol.h"
#include "ns3/ndn-interest-header.h"
#include "ns3/ndn-content-object-header.h"

#include "best-route.h"
#include "flooding.h"
#include "smart-flooding.h"

namespace ns3 {
namespace ndn {
namespace fw {

template class SimpleLimits<BestRoute>;
typedef SimpleLimits<BestRoute> SimpleLimitsBestRoute;
NS_OBJECT_ENSURE_REGISTERED (SimpleLimitsBestRoute);

template class SimpleLimits<Flooding>;
typedef SimpleLimits<Flooding> SimpleLimitsFlooding;
NS_OBJECT_ENSURE_REGISTERED (SimpleLimitsFlooding);

template class SimpleLimits<SmartFlooding>;
typedef SimpleLimits<SmartFlooding> SimpleLimitsSmartFlooding;
NS_OBJECT_ENSURE_REGISTERED (SimpleLimitsSmartFlooding);

#ifdef DOXYGEN

class SimpleLimitsBestRoute     : public SimpleLimits<BestRoute> { };
class SimpleLimitsFlooding      : public SimpleLimits<Flooding> { };
class SimpleLimitsSmartFlooding : public SimpleLimits<SmartFlooding> { };

#endif

} // namespace fw
} // namespace ndn
} // namespace ns3
