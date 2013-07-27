/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * BSD license, See the doc/LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_COMMON_H
#define NDN_COMMON_H

#include "ns3/nstime.h"
#include "ns3/simulator.h"

#define NDNSIM_MODE 1

#define NDN_NAMESPACE_BEGIN  namespace ns3 { namespace ndn {
#define NDN_NAMESPACE_END    } /*ndn*/ } /*ns3*/ 

/**
 * @brief NS-3 namespace
 */
namespace ns3 {

/**
 * @brief ndnSIM namespace
 */
namespace ndn {
}

}

NDN_NAMESPACE_BEGIN

typedef Time TimeInterval;

namespace time
{

inline Time
NowUnixTimestamp ()
{
  return Simulator::Now ();
}

}

NDN_NAMESPACE_END

#endif // NDN_COMMON_H
