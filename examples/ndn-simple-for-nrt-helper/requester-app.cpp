/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2016  Regents of the University of California.
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

#include "ns3/core-module.h"
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/ndn-cxx/link.hpp"
#include "ns3/ndnSIM/ndn-cxx/interest.hpp"

NS_LOG_COMPONENT_DEFINE("RequesterApp");

namespace ns3 {
namespace ndn {

class RequesterApp : public App
{
public:
  static TypeId
  GetTypeId()
  {
    static TypeId tid = TypeId("RequesterApp")
      .SetParent<App>()
      .AddConstructor<RequesterApp>()
      .AddAttribute("Name", "Name of data to request",
                    StringValue("/data/name"),
                    MakeNameAccessor(&RequesterApp::m_name), MakeNameChecker())
      .AddAttribute("Delegation", "Delegation name to attach to Interest",
                    StringValue("/"),
                    MakeNameAccessor(&RequesterApp::getDelegation, &RequesterApp::setDelegation), MakeNameChecker())
      ;

    return tid;
  }

protected:
  virtual void
  StartApplication() override
  {
    App::StartApplication();

    Simulator::Schedule(Seconds(1.0), &RequesterApp::sendInterest, this);
  }

  virtual void
  StopApplication() override
  {
    // do cleanup
    App::StopApplication();
    m_face->close();
  }

private:
  void
  setDelegation(const Name& delegation)
  {
    m_delegation = delegation;

    m_link = ::ndn::Link(Name(m_name).append("/LINK"));
    m_link.addDelegation(1, m_delegation);
    ndn::StackHelper::getKeyChain().sign(m_link, ::ndn::security::SigningInfo(::ndn::security::SigningInfo::SIGNER_TYPE_SHA256));

    NS_LOG_DEBUG("Created Link Object "<< m_link);
  }

  Name
  getDelegation() const
  {
    return m_delegation;
  }

  void
  sendInterest()
  {
    auto interest = make_shared<Interest>(m_name);
    interest->setInterestLifetime(time::seconds(1));
    if (m_delegation.size() > 0) {
      interest->setForwardingHint(m_link.getDelegationList());
    }

    NS_LOG_DEBUG("Sending an Interest for "<< *interest);

    m_transmittedInterests(interest, this, m_face);
    m_appLink->onReceiveInterest(*interest);
  }

private:
  Name m_name;
  Name m_delegation;
  ::ndn::Link m_link;
};

NS_OBJECT_ENSURE_REGISTERED(RequesterApp);

} // namespace ndn
} // namespace ns3
