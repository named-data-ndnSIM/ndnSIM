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

#include "ccnx-app-face.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/simulator.h"

#include "ns3/ccnx-header-helper.h"
#include "ns3/ccnx-app.h"

#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("CcnxAppFace");

namespace ns3 
{

NS_OBJECT_ENSURE_REGISTERED (CcnxAppFace);

TypeId
CcnxAppFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxAppFace")
    .SetParent<CcnxFace> ()
    .SetGroupName ("Ccnx")
    ;
  return tid;
}

CcnxAppFace::CcnxAppFace (Ptr<CcnxApp> app)
  : CcnxFace (app->GetNode ())
  , m_app (app)
{
  NS_LOG_FUNCTION (this << app);
  
  NS_ASSERT (m_app != 0);
}

CcnxAppFace::~CcnxAppFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

CcnxAppFace::CcnxAppFace ()
  : CcnxFace (0)
{
}

CcnxAppFace::CcnxAppFace (const CcnxAppFace &)
  : CcnxFace (0)
{
}

CcnxAppFace& CcnxAppFace::operator= (const CcnxAppFace &)
{
  return *((CcnxAppFace*)0);
}


void
CcnxAppFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION (this);

  CcnxFace::RegisterProtocolHandler (handler);

  m_app->RegisterProtocolHandler (MakeCallback (&CcnxFace::Receive, this));
}

bool
CcnxAppFace::SendImpl (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  try
    {
      CcnxHeaderHelper::Type type = CcnxHeaderHelper::GetCcnxHeaderType (p);
      switch (type)
        {
        case CcnxHeaderHelper::INTEREST:
          {
            Ptr<CcnxInterestHeader> header = Create<CcnxInterestHeader> ();
            p->RemoveHeader (*header);

            if (header->GetNack () > 0)
              m_app->OnNack (header, p);
            else
              m_app->OnInterest (header, p);
          
            break;
          }
        case CcnxHeaderHelper::CONTENT_OBJECT:
          {
            static CcnxContentObjectTail tail;
            Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader> ();
            p->RemoveHeader (*header);
            p->RemoveTrailer (tail);
            m_app->OnContentObject (header, p/*payload*/);
          
            break;
          }
        }
      
      return true;
    }
  catch (CcnxUnknownHeaderException)
    {
      NS_LOG_ERROR ("Unknown header type");
      return false;
    }
}

std::ostream& CcnxAppFace::Print (std::ostream& os) const
{
  os << "dev=local(" << GetId() << ")";
  return os;
}

}; // namespace ns3

