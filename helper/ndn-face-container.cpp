/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-face-container.hpp"

#include <algorithm>

namespace ns3 {
namespace ndn {

FaceContainer::FaceContainer()
{
}

FaceContainer::FaceContainer(const FaceContainer& other)
{
  AddAll(other);
}

FaceContainer&
FaceContainer::operator= (const FaceContainer &other)
{
  m_faces.clear();
  AddAll(other);

  return *this;
}

void
FaceContainer::AddAll(Ptr<FaceContainer> other)
{
  AddAll(*other);
}

void
FaceContainer::AddAll(const FaceContainer& other)
{
  m_faces.insert(m_faces.end(), other.m_faces.begin(), other.m_faces.end());
}

FaceContainer::Iterator
FaceContainer::Begin(void) const
{
  return m_faces.begin();
}

FaceContainer::Iterator
FaceContainer::End(void) const
{
  return m_faces.end();
}

uint32_t
FaceContainer::GetN(void) const
{
  return m_faces.size();
}

// void
// FaceContainer::SetMetricToAll (uint16_t metric)
// {
//   for (FaceContainer::iterator it=m_faces.begin ();
//        it != m_faces.end ();
//        it++)
//     {
//       (*it)->SetMetric (metric);
//     }
// }

void
FaceContainer::Add(const shared_ptr<Face>& face)
{
  m_faces.push_back(face);
}

shared_ptr<Face>
FaceContainer::Get(FaceContainer::Iterator i) const
{
  return *i;
}

} // namespace ndn
} // namespace ns3
