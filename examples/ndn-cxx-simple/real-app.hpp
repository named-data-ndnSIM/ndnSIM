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

#ifndef NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_REAL_APP_HPP
#define NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_REAL_APP_HPP

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/util/scheduler.hpp>

#include <iostream>

namespace app {

class RealApp
{
public:
  RealApp(ndn::KeyChain& keyChain)
    : m_keyChain(keyChain)
    , m_faceProducer(m_faceConsumer.getIoService())
    , m_scheduler(m_faceConsumer.getIoService())
  {
    // register prefix and set interest filter on producer face
    m_faceProducer.setInterestFilter("/hello", std::bind(&RealApp::respondToAnyInterest, this, _2),
                                     std::bind([]{}), std::bind([]{}));

    // use scheduler to send interest later on consumer face
    m_scheduler.scheduleEvent(ndn::time::seconds(2), [this] {
        m_faceConsumer.expressInterest(ndn::Interest("/hello/world"),
                                       std::bind([] { std::cout << "Hello!" << std::endl; }),
                                       std::bind([] { std::cout << "NACK!" << std::endl; }),
                                       std::bind([] { std::cout << "Bye!.." << std::endl; }));
      });
  }

  void
  run()
  {
    m_faceConsumer.processEvents(); // ok (will not block and do nothing)
    // m_faceConsumer.getIoService().run(); // will crash
  }

private:
  void
  respondToAnyInterest(const ndn::Interest& interest)
  {
    auto data = std::make_shared<ndn::Data>(interest.getName());
    m_keyChain.sign(*data);
    m_faceProducer.put(*data);
  }

private:
  ndn::KeyChain& m_keyChain;
  ndn::Face m_faceConsumer;
  ndn::Face m_faceProducer;
  ndn::Scheduler m_scheduler;
};

} // namespace app

#endif // NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_REAL_APP_HPP
