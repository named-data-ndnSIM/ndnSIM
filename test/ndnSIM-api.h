/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU v3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDNSIM_TEST_API_H
#define NDNSIM_TEST_API_H

#include "ns3/test.h"
#include "ns3/ptr.h"

namespace ns3 {

class Application;

namespace ndn {
}
  
class ApiTest : public TestCase
{
public:
  ApiTest ()
    : TestCase ("API test")
  {
  }
    
private:
  virtual void DoRun ();

  void Check0 (Ptr<Application> app);
  void Check1 (Ptr<Application> app);
  void Check2 (Ptr<Application> app);
};
  
}

#endif // NDNSIM_TEST_API_H
