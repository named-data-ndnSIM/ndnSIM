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

#ifndef CCNX_FORWARDING_HELPER_H
#define CCNX_FORWARDING_HELPER_H

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/output-stream-wrapper.h"
#include "ns3/ccnx.h"
#include "ns3/ccnx-flooding-strategy.h"

namespace ns3 {

class Node;

/**
 * \ingroup ccnx-helpers
 *
 * \brief a factory to create ns3::CcnxForwardingStrategy objects
 *
 * \todo Document this class
 */
class CcnxForwardingHelper
{
public:
    
  /*
   * \brief Default constructor, which sets NDN_FLOODING forwarding strategy
   */
  CcnxForwardingHelper();
  
  /*
   * \brief This constructor sets a specified forwarding strategy 
   */
  CcnxForwardingHelper(Ccnx::ForwardingStrategy strategy);
  
  /*
   * Destroy an instance of an CcnxForwardingHelper
   */
  ~CcnxForwardingHelper ();

  /*
   * \brief creates a specified ForwardingStrategy object and binds it to Ccnx
   */
  void SetForwarding(Ptr<Ccnx> ccnx) const;
    
  /**
   * \brief prints the forwarding tables of all nodes at a particular time.
   * \param printTime the time at which the forwarding table is supposed to be printed.
   * \param stream The output stream object to use 
   *
   * This method calls the PrintForwardingTable() method of the 
   * CcnxForwardingStrategy stored in the Ccnx object, for all nodes at the
   * specified time; the output format is forwarding protocol-specific.
   */
  void PrintForwardingTableAllAt (Time printTime, Ptr<OutputStreamWrapper> stream) const;

  /**
   * \brief prints the forwarding tables of all nodes at regular intervals specified by user.
   * \param printInterval the time interval for which the forwarding table is supposed to be printed.
   * \param stream The output stream object to use
   *
   * This method calls the PrintForwardingTable() method of the 
   * CcnxForwardingStrategy stored in the Ccnx object, for all nodes at the
   * specified time interval; the output format is forwarding protocol-specific.
   */
  void PrintForwardingTableAllEvery (Time printInterval, Ptr<OutputStreamWrapper> stream) const;

  /**
   * \brief prints the forwarding tables of a node at a particular time.
   * \param printTime the time at which the forwarding table is supposed to be printed.
   * \param node The node ptr for which we need the forwarding table to be printed
   * \param stream The output stream object to use
   *
   * This method calls the PrintForwardingTable() method of the 
   * CcnxForwardingStrategy stored in the Ccnx object, for the selected node 
   * at the specified time; the output format is forwarding protocol-specific.
   */
  void PrintForwardingTableAt (Time printTime, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const;

  /**
   * \brief prints the forwarding tables of a node at regular intervals specified by user.
   * \param printInterval the time interval for which the forwarding table is supposed to be printed.
   * \param node The node ptr for which we need the forwarding table to be printed
   * \param stream The output stream object to use
   *
   * This method calls the PrintForwardingTable() method of the 
   * CcnxForwardingStrategy stored in the Ccnx object, for the selected node 
   * at the specified interval; the output format is forwarding protocol-specific.
   */
  void PrintForwardingTableEvery (Time printInterval, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const;

private:
  Ccnx::ForwardingStrategy m_strategy;
  void Print (Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const;
  void PrintEvery (Time printInterval, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const;
};

} // namespace ns3


#endif /* CCNX_FORWARDING_HELPER_H */
