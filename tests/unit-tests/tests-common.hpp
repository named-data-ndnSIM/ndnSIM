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

#ifndef NDNSIM_TESTS_UNIT_TESTS_TESTS_COMMON_HPP
#define NDNSIM_TESTS_UNIT_TESTS_TESTS_COMMON_HPP

#include "ns3/core-module.h"
#include "model/ndn-global-router.hpp"
#include "helper/ndn-scenario-helper.hpp"

#include "boost-test.hpp"

namespace ns3 {
namespace ndn {

class CleanupFixture
{
public:
  ~CleanupFixture()
  {
    Simulator::Destroy();
    Names::Clear();
    GlobalRouter::clear();
  }
};

class ScenarioHelperWithCleanupFixture : public ScenarioHelper, public CleanupFixture
{
public:
};

} // namespace ndn
} // namespace ns3

#endif // NDNSIM_TESTS_UNIT_TESTS_TESTS_COMMON_HPP
