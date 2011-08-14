// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author:
//

#ifndef CCNX_L4_PROTOCOL_H
#define CCNX_L4_PROTOCOL_H

#include "ns3/object.h"
#include "ns3/callback.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

namespace ns3 {

class Packet;
class CcnxInterface;
class CcnxRoute;

/**
 * \brief Actual implementation of CCNx protocol 
 */
class CcnxL4Protocol : public Object
{
public:
  enum RxStatus {
    RX_OK,
    
    // RX_CSUM_FAILED,
    // RX_ENDPOINT_CLOSED,
    // RX_ENDPOINT_UNREACH
  };

  static TypeId GetTypeId (void);

  virtual ~CcnxL4Protocol ();

  /**
   * \param p packet to forward up
   * \param header Ccnx Header information
   * \param incomingInterface the CcnxInterface on which the packet arrived
   * 
   * Called from lower-level layers to send the packet up
   * in the stack. 
   */
  virtual enum RxStatus Receive (Ptr<Packet> p,
                                 CcnxHeader const &header,
                                 Ptr<CcnxInterface> incomingInterface) = 0;

  typedef Callback<void,Ptr<Packet>, Ptr<CcnxRoute> > DownTargetCallback;
  /**
   * This method allows a caller to set the current down target callback
   * set for this L4 protocol
   *
   * \param cb current Callback for the L4 protocol
   */
  virtual void SetDownTarget (DownTargetCallback cb) = 0;
  /**
   * This method allows a caller to get the current down target callback
   * set for this L4 protocol, for
   *
   * \return current Callback for the L4 protocol
   */
  virtual DownTargetCallback GetDownTarget (void) const = 0;
};

} // Namespace ns3

#endif // CCNX_L4_PROTOCOL_H
