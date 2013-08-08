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
#include "ndn-data.h"

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
  SetFlags (Face::APPLICATION);
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

AppFace&
AppFace::operator= (const AppFace &)
{
  return *((AppFace*)0);
}

bool
AppFace::SendInterest (Ptr<const Interest> interest)
{
  NS_LOG_FUNCTION (this << interest);

  if (!IsUp ())
    {
      return false;
    }

  if (interest->GetNack () > 0)
    m_app->OnNack (interest);
  else
    m_app->OnInterest (interest);
  
  return true;
}

bool
AppFace::SendData (Ptr<const Data> data)
{
  NS_LOG_FUNCTION (this << data);

  if (!IsUp ())
    {
      return false;
    }

  m_app->OnData (data);
  return true;
}

std::ostream&
AppFace::Print (std::ostream& os) const
{
  os << "dev=local(" << GetId() << ")";
  return os;
}

} // namespace ndn
} // namespace ns3

