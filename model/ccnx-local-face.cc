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
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 *
 */

#include "ccnx-local-face.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/assert.h"

#include "ns3/ccnx-header-helper.h"
#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("CcnxLocalFace");

namespace ns3 
{

// NS_OBJECT_ENSURE_REGISTERED (CcnxLocalFace);

// TypeId 
// CcnxLocalFace::GetTypeId (void)
// {
//   static TypeId tid = TypeId ("ns3::CcnxLocalFace")
//     .SetGroupName ("Ccnx")
//     .SetParent<CcnxFace> ()
//   ;
//   return tid;
// }

CcnxLocalFace::CcnxLocalFace () 
  : m_onInterest (0)
  , m_onContentObject (0)
{
  NS_LOG_FUNCTION (this);
}

CcnxLocalFace::~CcnxLocalFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CcnxLocalFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  m_protocolHandler = handler;
}

void
CcnxLocalFace::SetInterestHandler (InterestHandler onInterest)
{
  m_onInterest = onInterest;
}

void
CcnxLocalFace::SetContentObjectHandler (ContentObjectHandler onContentObject)
{
  m_onContentObject = onContentObject;
}

void
CcnxLocalFace::Send (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (*p);
  if (!IsUp ())
    {
      return;
    }

  try
    {
      CcnxHeaderHelper::Type type = CcnxHeaderHelper::GetCcnxHeaderType (p);
      switch (type)
        {
        case CcnxHeaderHelper::INTEREST:
          if (!m_onInterest.IsNull ())
            {
              Ptr<CcnxInterestHeader> header = Create<CcnxInterestHeader> ();
              p->RemoveHeader (*header);
              m_onInterest (header);
            }
          break;
        case CcnxHeaderHelper::CONTENT_OBJECT:
          if (!m_onContentObject.IsNull ())
            {
              static CcnxContentObjectTail tail;
              Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader> ();
              p->RemoveHeader (*header);
              p->RemoveTrailer (tail);
              m_onContentObject (header, p/*payload*/);
            }
          break;
        }
    }
  catch (CcnxUnknownHeaderException)
    {
      NS_LOG_ERROR ("Unknown header type");
    }
}

// propagate interest down to ccnx stack
void
CcnxLocalFace::ReceiveFromApplication (Ptr<Packet> p)
{
  m_protocolHandler (Ptr<CcnxFace>(this), p);
}

std::ostream& CcnxLocalFace::Print (std::ostream& os) const
{
  os << "dev=local(" << GetId() << ")";
  return os;
}

}; // namespace ns3

