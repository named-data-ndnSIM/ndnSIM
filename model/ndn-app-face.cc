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

#include "ndn-app-face.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/simulator.h"

#include "ns3/ndn-header-helper.h"
#include "ns3/ndn-app.h"

#include "ndn-interest-header.h"
#include "ndn-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("NdnAppFace");

namespace ns3 
{

NS_OBJECT_ENSURE_REGISTERED (NdnAppFace);

TypeId
NdnAppFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::NdnAppFace")
    .SetParent<NdnFace> ()
    .SetGroupName ("Ndn")
    ;
  return tid;
}

NdnAppFace::NdnAppFace (Ptr<NdnApp> app)
  : NdnFace (app->GetNode ())
  , m_app (app)
{
  NS_LOG_FUNCTION (this << app);
  
  NS_ASSERT (m_app != 0);
}

NdnAppFace::~NdnAppFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

NdnAppFace::NdnAppFace ()
  : NdnFace (0)
{
}

NdnAppFace::NdnAppFace (const NdnAppFace &)
  : NdnFace (0)
{
}

NdnAppFace& NdnAppFace::operator= (const NdnAppFace &)
{
  return *((NdnAppFace*)0);
}


void
NdnAppFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION (this);

  NdnFace::RegisterProtocolHandler (handler);

  m_app->RegisterProtocolHandler (MakeCallback (&NdnFace::Receive, this));
}

bool
NdnAppFace::SendImpl (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  try
    {
      NdnHeaderHelper::Type type = NdnHeaderHelper::GetNdnHeaderType (p);
      switch (type)
        {
        case NdnHeaderHelper::INTEREST:
          {
            Ptr<NdnInterestHeader> header = Create<NdnInterestHeader> ();
            p->RemoveHeader (*header);

            if (header->GetNack () > 0)
              m_app->OnNack (header, p);
            else
              m_app->OnInterest (header, p);
          
            break;
          }
        case NdnHeaderHelper::CONTENT_OBJECT:
          {
            static NdnContentObjectTail tail;
            Ptr<NdnContentObjectHeader> header = Create<NdnContentObjectHeader> ();
            p->RemoveHeader (*header);
            p->RemoveTrailer (tail);
            m_app->OnContentObject (header, p/*payload*/);
          
            break;
          }
        }
      
      return true;
    }
  catch (NdnUnknownHeaderException)
    {
      NS_LOG_ERROR ("Unknown header type");
      return false;
    }
}

std::ostream&
NdnAppFace::Print (std::ostream& os) const
{
  os << "dev=local(" << GetId() << ")";
  return os;
}

}; // namespace ns3

