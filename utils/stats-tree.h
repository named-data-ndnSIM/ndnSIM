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

#ifndef STATS_TREE_H
#define STATS_TREE_H

#include "trie.h"
#include "load-stats-node.h"
#include "ns3/ndn-name-components.h"
#include "ns3/ptr.h"

namespace ndnSIM
{

class StatsTree
{
public:
  typedef trie< ns3::NdnNameComponents,
                non_pointer_traits< LoadStatsNode >, void* > tree_type;
  
  StatsTree ();

  void
  Step ();
  
  void
  NewPitEntry (const ns3::NdnNameComponents &key);

  void
  Incoming (const ns3::NdnNameComponents &key, ns3::Ptr<ns3::NdnFace> face);

  void
  Outgoing (const ns3::NdnNameComponents &key, ns3::Ptr<ns3::NdnFace> face);

  void
  Satisfy (const ns3::NdnNameComponents &key);

  void
  Timeout (const ns3::NdnNameComponents &key);

  void
  Rx (const ns3::NdnNameComponents &key, ns3::Ptr<ns3::NdnFace> face, uint32_t amount);

  void
  Tx (const ns3::NdnNameComponents &key, ns3::Ptr<ns3::NdnFace> face, uint32_t amount);

  // const LoadStatsNode &
  // Get (const ns3::NdnNameComponents &key) const;
  const LoadStatsNode &
  operator [] (const ns3::NdnNameComponents &key) const;

  void
  RemoveFace (ns3::Ptr<ns3::NdnFace> face);
  
private:
  const LoadStatsNode &
  WalkLeftRightRoot (tree_type *node);

  
private:
  tree_type m_tree;

  friend std::ostream &
  operator << (std::ostream &os, const StatsTree &tree);
};

std::ostream &
operator << (std::ostream &os, const StatsTree &tree);

} // ndnSIM

#endif // STATS_TREE_H
