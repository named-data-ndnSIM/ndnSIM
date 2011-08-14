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


#include "ccnx-l4-protocol.h"

#include "ns3/uinteger.h"

NS_LOG_COMPONENT_DEFINE ("CcnxL4Protocol");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CcnxL4Protocol);

TypeId 
CcnxL4Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxL4Protocol")
    .SetParent<Object> ()
  ;
  return tid;
}

CcnxL4Protocol::CcnxL4Protocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

CcnxL4Protocol::~CcnxL4Protocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
CcnxL4Protocol::SetNode (Ptr<Node> node)
{
  m_node = node;
}

/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the udp stack and link it to the ipv4 object
 * present in the node along with the socket factory
 */
void
CcnxL4Protocol::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node> node = this->GetObject<Node> ();
      if (node != 0)
        {
          Ptr<Ccnx> ccnx = this->GetObject<Ccnx> ();
          if (ccnx != 0)
            {
              this->SetNode (node);
              ccnx->Insert (this);
              // Ptr<UdpSocketFactoryImpl> udpFactory = CreateObject<UdpSocketFactoryImpl> ();
              // udpFactory->SetUdp (this);
              // node->AggregateObject (udpFactory);
              this->SetDownTarget (MakeCallback (&Ccnx::Send, ccnx));
            }
        }
    }
  Object::NotifyNewAggregate ();
}

void
CcnxL4Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_node = 0;
  m_downTarget.Nullify ();
  CcnxL4Protocol::DoDispose ();
}

void
SetDownTarget (DownTargetCallback cb)
{
  m_downTarget = cb;
}

DownTargetCallback
GetDownTarget (void) const
{
  retur m_downTarget;
}

} //namespace ns3
