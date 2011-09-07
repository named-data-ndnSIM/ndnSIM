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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnx-face-container.h"

#include <algorithm>

#include "ns3/ccnx-face.h"

namespace ns3 {

CcnxFaceContainer::CcnxFaceContainer ()
{
}

CcnxFaceContainer::CcnxFaceContainer (const CcnxFaceContainer &other)
{
  AddAll (other);
}

CcnxFaceContainer&
CcnxFaceContainer::operator= (const CcnxFaceContainer &other)
{
  m_faces.clear ();
  AddAll (other);

  return *this;
}

  
void
CcnxFaceContainer::AddAll (Ptr<CcnxFaceContainer> other)
{
  AddAll (*other);
}

void
CcnxFaceContainer::AddAll (const CcnxFaceContainer &other)
{
  m_faces.insert (m_faces.end (),
                  other.m_faces.begin (), other.m_faces.end ());
}

CcnxFaceContainer::Iterator
CcnxFaceContainer::Begin (void) const
{
  return m_faces.begin ();
}

CcnxFaceContainer::Iterator
CcnxFaceContainer::End (void) const
{
  return m_faces.end ();
}

uint32_t
CcnxFaceContainer::GetN (void) const
{
  return m_faces.size ();
}

// void 
// CcnxFaceContainer::SetMetricToAll (uint16_t metric)
// {
//   for (FaceContainer::iterator it=m_faces.begin ();
//        it != m_faces.end ();
//        it++)
//     {
//       (*it)->SetMetric (metric);
//     }
// }

void 
CcnxFaceContainer::Add (const Ptr<CcnxFace> &face)
{
  m_faces.push_back (face);
}

Ptr<CcnxFace>
CcnxFaceContainer::Get (CcnxFaceContainer::Iterator i) const
{
  return *i;
}


} // namespace ns3
