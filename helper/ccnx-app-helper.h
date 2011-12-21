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

#ifndef CCNX_APP_HELPER_H
#define CCNX_APP_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/attribute.h"
#include "ns3/node-container.h"
#include "ns3/application-container.h"
#include "ns3/ptr.h"

namespace ns3 
{

/**
 * \brief A helper to make it easier to instantiate an ns3::CcnxConsumer Application
 * on a set of nodes.
 */
class CcnxAppHelper
{        
public:

  /**
   * \brief Create an CcnxAppHelper to make it easier to work with CCNx apps
   *
   * \param app Class of the application
   */
  CcnxAppHelper (const std::string &prefix);

  /**
   * @brief Set the prefix consumer will be requesting
   */
  void
  SetPrefix (const std::string &prefix);
  
  /**
   * \brief Helper function used to set the underlying application attributes.
   *
   * \param name the name of the application attribute to set
   * \param value the value of the application attribute to set
   */
  void SetAttribute (std::string name, const AttributeValue &value);
        
  /**
   * Install an ns3::CcnxConsumer on each node of the input container
   * configured with all the attributes set with SetAttribute.
   *
   * \param c NodeContainer of the set of nodes on which an CcnxConsumer 
   * will be installed.
   * \returns Container of Ptr to the applications installed.
   */
  ApplicationContainer Install (NodeContainer c);
        
  /**
   * Install an ns3::CcnxConsumer on the node configured with all the 
   * attributes set with SetAttribute.
   *
   * \param node The node on which an CcnxConsumer will be installed.
   * \returns Container of Ptr to the applications installed.
   */
  ApplicationContainer Install (Ptr<Node> node);
        
  /**
   * Install an ns3::CcnxConsumer on the node configured with all the 
   * attributes set with SetAttribute.
   *
   * \param nodeName The node on which an CcnxConsumer will be installed.
   * \returns Container of Ptr to the applications installed.
   */
  ApplicationContainer Install (std::string nodeName);
        
private:
  /**
   * \internal
   * Install an ns3::CcnxConsumer on the node configured with all the 
   * attributes set with SetAttribute.
   *
   * \param node The node on which an CcnxConsumer will be installed.
   * \returns Ptr to the application installed.
   */
  Ptr<Application> InstallPriv (Ptr<Node> node);
  ObjectFactory m_factory;
};

} // namespace ns3

#endif // CCNX_APP_HELPER_H

