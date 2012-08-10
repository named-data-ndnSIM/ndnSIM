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

#ifndef NDNSIM_TEST_PIT_H
#define NDNSIM_TEST_PIT_H

#include "ns3/test.h"
#include "ns3/ptr.h"

namespace ns3 {

namespace ndn {
class Fib;
class Pit;
}
  
class PitTest : public TestCase
{
public:
  PitTest ()
    : TestCase ("PIT test")
  {
  }
    
private:
  virtual void DoRun ();

  void Test (Ptr<ndn::Fib> fib);
  void Check0 (Ptr<ndn::Pit> pit);
  void Check1 (Ptr<ndn::Pit> pit);
  void Check2 (Ptr<ndn::Pit> pit);
  void Check3 (Ptr<ndn::Pit> pit);
};
  
}

#endif // NDNSIM_TEST_PIT_H
