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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ccnx-consumer-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-socket-address.h"
#include "ns3/string.h"
#include "ns3/names.h"

namespace ns3 
{
    
CcnxConsumerHelper::CcnxConsumerHelper (Ptr<CcnxNameComponents> interestName)
{
    m_factory.SetTypeId ("ns3::CcnxConsumer");
    m_factory.Set ("InterestName", PointerValue (interestName));
}
    
void 
CcnxConsumerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
    m_factory.Set (name, value);
}
    
ApplicationContainer
CcnxConsumerHelper::Install (Ptr<Node> node)
{
    return ApplicationContainer (InstallPriv (node));
}
    
ApplicationContainer
CcnxConsumerHelper::Install (std::string nodeName)
{
    Ptr<Node> node = Names::Find<Node> (nodeName);
    return ApplicationContainer (InstallPriv (node));
}
    
ApplicationContainer
CcnxConsumerHelper::Install (NodeContainer c)
{
    ApplicationContainer apps;
    for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
        apps.Add (InstallPriv (*i));
    }
    
    return apps;
}
    
Ptr<Application>
CcnxConsumerHelper::InstallPriv (Ptr<Node> node)
{
    Ptr<CcnxLocalFace> localFace = CreateObject<CcnxLocalFace> ();
    localFace->SetNode(node);
    
    m_factory.Set ("Face", PointerValue (localFace));
    Ptr<CcnxConsumer> app = m_factory.Create<CcnxConsumer> ();
        
    localFace->RegisterProtocolHandler (MakeCallback (&CcnxConsumer::HandlePacket, app));
    localFace->SetUp();
        
    node->AddApplication (app);
        
    return app;
}
}