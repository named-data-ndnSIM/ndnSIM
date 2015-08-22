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

#include "ndn-app-helper.hpp"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/names.h"

#include "apps/ndn-app.hpp"

#ifdef NS3_MPI
#include "ns3/mpi-interface.h"
#endif

NS_LOG_COMPONENT_DEFINE("ndn.AppHelper");

namespace ns3 {
namespace ndn {

AppHelper::AppHelper(const std::string& app)
{
  m_factory.SetTypeId(app);
}

void
AppHelper::SetPrefix(const std::string& prefix)
{
  m_factory.Set("Prefix", StringValue(prefix));
}

void
AppHelper::SetAttribute(std::string name, const AttributeValue& value)
{
  m_factory.Set(name, value);
}

ApplicationContainer
AppHelper::Install(Ptr<Node> node)
{
  ApplicationContainer apps;
  Ptr<Application> app = InstallPriv(node);
  if (app != 0)
    apps.Add(app);

  return apps;
}

ApplicationContainer
AppHelper::Install(std::string nodeName)
{
  Ptr<Node> node = Names::Find<Node>(nodeName);
  return Install(node);
}

ApplicationContainer
AppHelper::Install(NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin(); i != c.End(); ++i) {
    Ptr<Application> app = InstallPriv(*i);
    if (app != 0)
      apps.Add(app);
  }

  return apps;
}

Ptr<Application>
AppHelper::InstallPriv(Ptr<Node> node)
{
#ifdef NS3_MPI
  if (MpiInterface::IsEnabled() && node->GetSystemId() != MpiInterface::GetSystemId()) {
    // don't create an app if MPI is enabled and node is not in the correct partition
    return 0;
  }
#endif

  Ptr<Application> app = m_factory.Create<Application>();
  node->AddApplication(app);

  return app;
}

////////////////////////////////////////////////////////////////////////////

FactoryCallbackApp::FactoryCallbackApp(const FactoryCallback& factory)
  : m_factory(factory)
{
}

ApplicationContainer
FactoryCallbackApp::Install(Ptr<Node> node, const FactoryCallback& factory)
{
  ApplicationContainer apps;
  auto app = CreateObject<FactoryCallbackApp>(factory);
  node->AddApplication(app);
  apps.Add(app);
  return apps;
}

void
FactoryCallbackApp::StartApplication()
{
  m_impl = m_factory();
}

void
FactoryCallbackApp::StopApplication()
{
  m_impl.reset();
}


} // namespace ndn
} // namespace ns3
