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

#ifndef NDN_COMMON_H
#define NDN_COMMON_H

#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"

#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/signature.hpp>
#include <ndn-cxx/signature-info.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/security/key-chain.hpp>

#include <ndn-cxx/util/time.hpp>

namespace nfd {
}

namespace ns3 {
namespace ndn {

using ::ndn::Name;
namespace name = ::ndn::name;

ATTRIBUTE_HELPER_HEADER(Name);

namespace time {
using namespace ::ndn::time;
}

using ::ndn::Exclude;

using std::shared_ptr;
using std::make_shared;

using ::ndn::Interest;
using ::ndn::Data;
using ::ndn::KeyLocator;
using ::ndn::Signature;
using ::ndn::SignatureInfo;
using ::ndn::Block;
using ::ndn::KeyChain;

#ifndef DOXYGEN
// For python bindings
namespace nfd = ::nfd;
#endif // DOXYGEN

} // namespace ndn
} // namespace ns3

#endif // NDN_COMMON_H
