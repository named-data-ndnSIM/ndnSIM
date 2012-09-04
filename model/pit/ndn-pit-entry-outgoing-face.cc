/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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

#include "ndn-pit-entry-outgoing-face.h"

#include "ns3/simulator.h"

namespace ns3 {
namespace ndn {
namespace pit {

OutgoingFace::OutgoingFace (Ptr<Face> face)
  : m_face (face)
  , m_sendTime (Simulator::Now ())
  , m_retxCount (0)
  , m_waitingInVain (false)
{
}

OutgoingFace::OutgoingFace ()
  : m_face (0)
  , m_sendTime (0)
  , m_retxCount (0)
  , m_waitingInVain (false)
{
}

OutgoingFace &
OutgoingFace::operator = (const OutgoingFace &other)
{
  m_face = other.m_face;
  m_sendTime = other.m_sendTime;
  m_retxCount = other.m_retxCount;
  m_waitingInVain = other.m_waitingInVain;

  return *this;
}


void
OutgoingFace::UpdateOnRetransmit ()
{
  m_sendTime = Simulator::Now ();
  m_retxCount++;
  m_waitingInVain = false;
}

} // namespace pit
} // namespace ndn
} // namespace ns3
