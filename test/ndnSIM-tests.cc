/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ns3/test.h"

#include "ndnSIM-serialization.h"
#include "ndnSIM-pit.h"
#include "ndnSIM-fib-entry.h"
#include "ndnSIM-api.h"

namespace ns3
{

class NdnSimTestSuite : public TestSuite
{
public:
  NdnSimTestSuite ()
    : TestSuite ("ndnSIM", UNIT)
  {
    SetDataDir (NS_TEST_SOURCEDIR);

    AddTestCase (new InterestSerializationTest (), TestCase::QUICK);
    AddTestCase (new DataSerializationTest (), TestCase::QUICK);
    AddTestCase (new FibEntryTest (), TestCase::QUICK);
    AddTestCase (new PitTest (), TestCase::QUICK);
    AddTestCase (new ApiTest (), TestCase::QUICK);
  }
};

static NdnSimTestSuite suite;

}
