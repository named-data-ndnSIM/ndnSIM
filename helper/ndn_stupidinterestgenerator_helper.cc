//
//  ndn_stupidinterestgenerator_helper.cpp
//  Abstraction
//
//  Created by Ilya Moiseenko on 05.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ndn_stupidinterestgenerator_helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-socket-address.h"
#include "ns3/string.h"
#include "ns3/names.h"


namespace ns3 {
    
    StupidInterestGeneratorHelper::StupidInterestGeneratorHelper (std::string protocol, Address address)
    {
        m_factory.SetTypeId ("ns3::StupidInterestGenerator");
        m_factory.Set ("Protocol", StringValue (protocol));
        m_factory.Set ("Remote", AddressValue (address));
    }
    
    void 
    StupidInterestGeneratorHelper::SetAttribute (std::string name, const AttributeValue &value)
    {
        m_factory.Set (name, value);
    }
    
    ApplicationContainer
    StupidInterestGeneratorHelper::Install (Ptr<Node> node) const
    {
        return ApplicationContainer (InstallPriv (node));
    }
    
    ApplicationContainer
    StupidInterestGeneratorHelper::Install (std::string nodeName) const
    {
        Ptr<Node> node = Names::Find<Node> (nodeName);
        return ApplicationContainer (InstallPriv (node));
    }
    
    ApplicationContainer
    StupidInterestGeneratorHelper::Install (NodeContainer c) const
    {
        ApplicationContainer apps;
        for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
        {
            apps.Add (InstallPriv (*i));
        }
        
        return apps;
    }
    
    Ptr<Application>
    StupidInterestGeneratorHelper::InstallPriv (Ptr<Node> node) const
    {
        Ptr<Application> app = m_factory.Create<Application> ();
        node->AddApplication (app);
        
        return app;
    }
    
} // namespace ns3
