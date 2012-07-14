/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "load-stats-node.h"
#include "ns3/ccnx-face.h"

using namespace ns3;

namespace ndnSIM
{

void
LoadStatsNode::Step ()
{
  m_pit.Step ();
  
  for (stats_container::iterator item = m_incoming.begin ();
       item != m_incoming.end ();
       item ++)
    {
      item->second.Step ();
    }
  
  for (stats_container::iterator item = m_outgoing.begin ();
       item != m_outgoing.end ();
       item ++)
    {
      item->second.Step ();
    }
}

void
LoadStatsNode::NewPitEntry ()
{
  m_pit.count ()++;
}

void
LoadStatsNode::AddIncoming (ns3::Ptr<ns3::CcnxFace> face)
{
  m_incoming [face].count ()++;
}

void
LoadStatsNode::AddOutgoing (ns3::Ptr<ns3::CcnxFace> face)
{
  m_outgoing [face].count ()++;
}

void
LoadStatsNode::Satisfy ()
{
  m_pit.satisfied ()++;
  
  for (stats_container::iterator item = m_incoming.begin ();
       item != m_incoming.end ();
       item ++)
    {
      item->second.satisfied ()++;
    }

  for (stats_container::iterator item = m_outgoing.begin ();
       item != m_outgoing.end ();
       item ++)
    {
      item->second.satisfied ()++;
    }
}

void
LoadStatsNode::Timeout ()
{
  m_pit.unsatisfied ()++;
  
  for (stats_container::iterator item = m_incoming.begin ();
       item != m_incoming.end ();
       item ++)
    {
      item->second.unsatisfied ()++;
    }

  for (stats_container::iterator item = m_outgoing.begin ();
       item != m_outgoing.end ();
       item ++)
    {
      item->second.unsatisfied ()++;
    }
}

LoadStatsNode &
LoadStatsNode::operator += (const LoadStatsNode &stats)
{
  m_pit += stats.m_pit;
  
  // aggregate incoming
  for (stats_container::const_iterator item = stats.m_incoming.begin ();
       item != stats.m_incoming.end ();
       item ++)
    {
      m_incoming [item->first] += item->second;
    }
  
  // aggregate outgoing
  for (stats_container::const_iterator item = stats.m_outgoing.begin ();
       item != stats.m_outgoing.end ();
       item ++)
    {
      m_outgoing [item->first] += item->second;
    }

  return *this;
}

std::ostream&
operator << (std::ostream &os, const LoadStatsNode &node)
{
  os << "PIT: " << node.m_pit << std::endl;
  return os;
}


}
