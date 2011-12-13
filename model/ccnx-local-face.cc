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
#include "ns3/assert.h"

#include "ns3/ccnx-header-helper.h"
#include "ns3/ccnx-app.h"

#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("CcnxLocalFace");

namespace ns3 
{

CcnxLocalFace::CcnxLocalFace (Ptr<CcnxApp> app)
  : CcnxFace (app->GetNode ())
  , m_app (app)
{
  NS_LOG_FUNCTION (this << app);
  
  NS_ASSERT (m_app != 0);
}

CcnxLocalFace::~CcnxLocalFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

CcnxLocalFace::CcnxLocalFace ()
  : CcnxFace (0)
{
}

CcnxLocalFace::CcnxLocalFace (const CcnxLocalFace &)
  : CcnxFace (0)
{
}

CcnxLocalFace& CcnxLocalFace::operator= (const CcnxLocalFace &)
{
  return *((CcnxLocalFace*)0);
}


void
CcnxLocalFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION (this);

  CcnxFace::RegisterProtocolHandler (handler);

  m_app->RegisterProtocolHandler (MakeCallback (&CcnxFace::Receive, this));
}
    
void
CcnxLocalFace::SendImpl (Ptr<Packet> p)
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
              m_app->OnNack (header);
            else
              m_app->OnInterest (header);
          
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
    }
  catch (CcnxUnknownHeaderException)
    {
      NS_LOG_ERROR ("Unknown header type");
    }
}

std::ostream& CcnxLocalFace::Print (std::ostream& os) const
{
  os << "dev=local(" << GetId() << ")";
  return os;
}

}; // namespace ns3

