/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
            Ilya Moiseenko <iliamo@cs.ucla.edu>
 *
 */

#include "ccnx-local-face.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/assert.h" 

NS_LOG_COMPONENT_DEFINE ("CcnxLocalFace");

namespace ns3 
{

NS_OBJECT_ENSURE_REGISTERED (CcnxLocalFace);

TypeId 
CcnxLocalFace::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxLocalFace")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxFace> ()
  ;
  return tid;
}

CcnxLocalFace::CcnxLocalFace () 
{
  NS_LOG_FUNCTION (this);
}

CcnxLocalFace::~CcnxLocalFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CcnxLocalFace::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  CcnxFace::DoDispose ();
}

void
CcnxLocalFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  m_protocolHandler = handler;
}

void
CcnxLocalFace::Send (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (*p);
  if (!IsUp ())
    {
      return;
    }

  
}
    
void
CcnxLocalFace::Receive (Ptr<Packet> p)
{
    //ypedef Callback<void,const Ptr<CcnxFace>&,const Ptr<const Packet>& > ProtocolHandler;
    m_protocolHandler ((const Ptr<CcnxFace>)this,(const Ptr<Packet>)p);
}

std::ostream& operator<< (std::ostream& os, const CcnxLocalFace &localFace)
{
  os << "dev=local";
  return os;
}

}; // namespace ns3

