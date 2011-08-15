/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include "ccnx-route.h"

#include "ns3/ccnx-face.h"
#include "ns3/assert.h"

namespace ns3 {

CcnxRoute::CcnxRoute ()
{
}

void
CcnxRoute::SetPrefix (const Ptr<Name::Components> &prefix)
{
  m_prefix = prefix;
}

const Name::Components&
CcnxRoute::GetPrefix (void) const
{
  return *m_prefix;
}

void
CcnxRoute::SetOutputFace (Ptr<CcnxFace> outputFace)
{
  m_outputFace = outputFace;
}

Ptr<CcnxFace>
CcnxRoute::GetOutputFace (void) const
{
  return m_outputFace;
}

std::ostream& operator<< (std::ostream& os, CcnxRoute const& route)
{
  os << "prefix=" << route.GetPrefix () << ", " << route.GetOutputFace ();
  return os;
}

} //namespace ns3
