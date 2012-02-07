/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 *
 */

#include "ccnx-cache-face.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/simulator.h"

#include "ns3/ccnx-header-helper.h"

#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("CcnxLocalFace");

namespace ns3 
{

TypeId
CcnxCacheFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxCacheFace")
    .SetParent<CcnxFace> ()
    .SetGroupName ("Ccnx")
    ;
  return tid;
}

CcnxCacheFace::CcnxCacheFace (Ptr<Node> node)
  : CcnxFace (node)
{
  NS_LOG_FUNCTION (this << node);
}

CcnxCacheFace::~CcnxCacheFace ()
{
  NS_LOG_FUNCTION (this);
}

CcnxCacheFace::CcnxCacheFace ()
  : CcnxFace (0)
{
}

CcnxCacheFace::CcnxCacheFace (const CcnxCacheFace &)
  : CcnxFace (0)
{
}

CcnxCacheFace& CcnxCacheFace::operator= (const CcnxCacheFace &)
{
  return *((CcnxCacheFace*)0);
}


void
CcnxCacheFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION (this);

  CcnxFace::RegisterProtocolHandler (handler);

  // m_app->RegisterProtocolHandler (MakeCallback (&CcnxFace::Receive, this));
}

void
CcnxCacheFace::SendImpl (Ptr<Packet> p)
{
  NS_FATAL_ERROR ("CcnxCacheFace doesn't support Send method (yet?)");
}
// void
// CcnxCacheFace::SendImpl (Ptr<Packet> p)
// {
//   NS_LOG_FUNCTION (this << p);

//   try
//     {
//       CcnxHeaderHelper::Type type = CcnxHeaderHelper::GetCcnxHeaderType (p);
//       switch (type)
//         {
//         case CcnxHeaderHelper::INTEREST:
//           {
//             Ptr<CcnxInterestHeader> header = Create<CcnxInterestHeader> ();
//             p->RemoveHeader (*header);

//             if (header->GetNack () > 0)
//               m_app->OnNack (header, p);
//             else
//               m_app->OnInterest (header, p);
          
//             break;
//           }
//         case CcnxHeaderHelper::CONTENT_OBJECT:
//           {
//             static CcnxContentObjectTail tail;
//             Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader> ();
//             p->RemoveHeader (*header);
//             p->RemoveTrailer (tail);
//             m_app->OnContentObject (header, p/*payload*/);
          
//             break;
//           }
//         }
//     }
//   catch (CcnxUnknownHeaderException)
//     {
//       NS_LOG_ERROR ("Unknown header type");
//     }
// }

std::ostream& CcnxCacheFace::Print (std::ostream& os) const
{
  os << "dev=cache()";
  return os;
}

}; // namespace ns3

