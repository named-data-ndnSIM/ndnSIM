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

#include "ndn-interest.h"
#include "ndn-content-object.h"

NS_LOG_COMPONENT_DEFINE ("ndn.AppFace");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (AppFace);

TypeId
AppFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::AppFace")
    .SetParent<Face> ()
    .SetGroupName ("Ndn")
    ;
  return tid;
}

AppFace::AppFace (Ptr<App> app)
  : Face (app->GetNode ())
  , m_app (app)
{
  NS_LOG_FUNCTION (this << app);
  
  NS_ASSERT (m_app != 0);
}

AppFace::~AppFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

AppFace::AppFace ()
  : Face (0)
{
}

AppFace::AppFace (const AppFace &)
  : Face (0)
{
}

AppFace& AppFace::operator= (const AppFace &)
{
  return *((AppFace*)0);
}


void
AppFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION (this);

  Face::RegisterProtocolHandler (handler);

  m_app->RegisterProtocolHandler (MakeCallback (&Face::Receive, this));
}

bool
AppFace::SendImpl (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  try
    {
      HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (p);
      switch (type)
        {
        case HeaderHelper::INTEREST_NDNSIM:
          {
            Ptr<InterestHeader> header = Create<InterestHeader> ();
            p->RemoveHeader (*header);

            if (header->GetNack () > 0)
              m_app->OnNack (header, p);
            else
              m_app->OnInterest (header, p);
          
            break;
          }
        case HeaderHelper::CONTENT_OBJECT_NDNSIM:
          {
            static ContentObjectTail tail;
            Ptr<ContentObjectHeader> header = Create<ContentObjectHeader> ();
            p->RemoveHeader (*header);
            p->RemoveTrailer (tail);
            m_app->OnContentObject (header, p/*payload*/);
          
            break;
          }
        default:
          NS_FATAL_ERROR ("ccnb support is currently broken");
          break;
        }
      
      return true;
    }
  catch (UnknownHeaderException)
    {
      NS_LOG_ERROR ("Unknown header type");
      return false;
    }
}

std::ostream&
AppFace::Print (std::ostream& os) const
{
  os << "dev=local(" << GetId() << ")";
  return os;
}

} // namespace ndn
} // namespace ns3

