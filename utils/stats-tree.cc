/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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

#include "stats-tree.h"
#include "ns3/ccnx-face.h"
#include "ns3/log.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("StatsTree");

namespace ndnSIM
{

StatsTree::StatsTree ()
  : m_tree ("")
{
}

void
StatsTree::Step ()
{
  // walking the tree, aggregating and stepping on every node, starting the leaves
  // for (trie_type::

  WalkLeftRightRoot (&m_tree);
}

void
StatsTree::NewPitEntry (const ns3::CcnxNameComponents &key)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());
  NS_ASSERT (item.second == false); // should always return false

  item.first->payload ().NewPitEntry ();
  NS_LOG_DEBUG ("NewPitEntry: " << item.first->payload ());
}

void
StatsTree::Incoming (const CcnxNameComponents &key, Ptr<CcnxFace> face)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());
  NS_ASSERT (item.second == false); // should always return false

  item.first->payload ().AddIncoming (face);
}

void
StatsTree::Outgoing (const CcnxNameComponents &key, Ptr<CcnxFace> face)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());
  NS_ASSERT (item.second == false); // should always return false

  item.first->payload ().AddOutgoing (face);
}

void
StatsTree::Satisfy (const CcnxNameComponents &key)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());
  NS_ASSERT (item.second == false); // should always return false

  item.first->payload ().Satisfy ();
}

void
StatsTree::Timeout (const CcnxNameComponents &key)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());
  NS_ASSERT (item.second == false); // should always return false

  item.first->payload ().Timeout ();
}

const LoadStatsNode &
StatsTree::Get (const ns3::CcnxNameComponents &key) const
{
  tree_type::iterator foundItem, lastItem;
  bool reachLast;
  boost::tie (foundItem, reachLast, lastItem) = const_cast<tree_type&> (m_tree).find (key);

  NS_ASSERT_MSG (foundItem == lastItem, "Found item should always be the same as last item (same address)");

  return foundItem->payload ();
}

const LoadStatsNode&
StatsTree::WalkLeftRightRoot (tree_type *node)
{
  tree_type::point_iterator item (*node), end;
  
  for (; item != end; item++)
    {
      node->payload () += WalkLeftRightRoot (&*item);
      NS_LOG_DEBUG (node << ", " << node->payload ());
    }

  node->payload ().Step ();
  return node->payload ();
}

std::ostream &
operator << (std::ostream &os, const StatsTree &tree)
{
  os << tree.m_tree.payload ();
  return os;
}


}
