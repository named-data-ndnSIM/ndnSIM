//
//  ndn_stupidinterestgenerator_helper.h
//  Abstraction
//
//  Created by Ilya Moiseenko on 05.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/node-container.h"
#include "ns3/application-container.h"

namespace ns3 {
    
    /**
     * \brief A helper to make it easier to instantiate an ns3::OnOffApplication 
     * on a set of nodes.
     */
    class StupidInterestGeneratorHelper
    {
    public:
        /**
         * Create an OnOffHelper to make it easier to work with OnOffApplications
         *
         * \param protocol the name of the protocol to use to send traffic
         *        by the applications. This string identifies the socket
         *        factory type used to create sockets for the applications.
         *        A typical value would be ns3::UdpSocketFactory.
         * \param address the address of the remote node to send traffic
         *        to.
         */
        StupidInterestGeneratorHelper (std::string protocol, Address address);
        
        /**
         * Helper function used to set the underlying application attributes.
         *
         * \param name the name of the application attribute to set
         * \param value the value of the application attribute to set
         */
        void SetAttribute (std::string name, const AttributeValue &value);
        
        /**
         * Install an ns3::OnOffApplication on each node of the input container
         * configured with all the attributes set with SetAttribute.
         *
         * \param c NodeContainer of the set of nodes on which an OnOffApplication 
         * will be installed.
         * \returns Container of Ptr to the applications installed.
         */
        ApplicationContainer Install (NodeContainer c) const;
        
        /**
         * Install an ns3::OnOffApplication on the node configured with all the 
         * attributes set with SetAttribute.
         *
         * \param node The node on which an OnOffApplication will be installed.
         * \returns Container of Ptr to the applications installed.
         */
        ApplicationContainer Install (Ptr<Node> node) const;
        
        /**
         * Install an ns3::OnOffApplication on the node configured with all the 
         * attributes set with SetAttribute.
         *
         * \param nodeName The node on which an OnOffApplication will be installed.
         * \returns Container of Ptr to the applications installed.
         */
        ApplicationContainer Install (std::string nodeName) const;
        
    private:
        /**
         * \internal
         * Install an ns3::OnOffApplication on the node configured with all the 
         * attributes set with SetAttribute.
         *
         * \param node The node on which an OnOffApplication will be installed.
         * \returns Ptr to the application installed.
         */
        Ptr<Application> InstallPriv (Ptr<Node> node) const;
        std::string m_protocol;
        Address m_remote;
        ObjectFactory m_factory;
    };
    
} // namespace ns3
