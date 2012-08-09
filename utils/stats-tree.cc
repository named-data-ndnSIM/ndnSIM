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
#include "ns3/ndn-face.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("ndn.StatsTree");

namespace ns3 {
namespace ndn {
namespace ndnSIM {

StatsTree::StatsTree ()
  : m_tree ("")
{
}

void
StatsTree::Step ()
{
  NS_LOG_FUNCTION (this);

  // walking the tree, aggregating and stepping on every node, starting the leaves
  // for (trie_type::

  WalkLeftRightRoot (&m_tree);
  m_tree.payload ().Step ();
  NS_LOG_DEBUG ("[" << m_tree.key () << "] " << m_tree.payload ());  
}

void
StatsTree::NewPitEntry (const NameComponents &key)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());

  item.first->payload ().NewPitEntry ();
}

void
StatsTree::Incoming (const NameComponents &key, Ptr<Face> face)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());

  item.first->payload ().AddIncoming (face);
}

void
StatsTree::Outgoing (const NameComponents &key, Ptr<Face> face)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());

  item.first->payload ().AddOutgoing (face);
}

void
StatsTree::Satisfy (const NameComponents &key)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());

  item.first->payload ().Satisfy ();
}

void
StatsTree::Timeout (const NameComponents &key)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());

  item.first->payload ().Timeout ();
}

void
StatsTree::Rx (const NameComponents &key, Ptr<Face> face, uint32_t amount)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());

  item.first->payload ().Rx (face, amount);
}

void
StatsTree::Tx (const NameComponents &key, Ptr<Face> face, uint32_t amount)
{
  std::pair<tree_type::iterator, bool> item = m_tree.insert (key, LoadStatsNode ());

  item.first->payload ().Tx (face, amount);
}

// const LoadStatsNode &
// StatsTree::Get (const NameComponents &key) const
const LoadStatsNode &
StatsTree::operator [] (const NameComponents &key) const
{
  tree_type::iterator foundItem, lastItem;
  bool reachLast;
  boost::tie (foundItem, reachLast, lastItem) = const_cast<tree_type&> (m_tree).find (key);

  return lastItem->payload ();
}

const LoadStatsNode&
StatsTree::WalkLeftRightRoot (tree_type *node)
{
  tree_type::point_iterator item (*node), end;

  while (item != end)
    {
      node->payload () += WalkLeftRightRoot (&*item);
      item->payload ().Step ();

      NS_LOG_DEBUG ("[" << item->key () << "] " << item->payload ());
      // item->prune (); // will do only if necessary

      tree_type::point_iterator prune_iterator = item;
      item++;

      prune_iterator->prune ();
    }
  
  return node->payload ();
}

void
StatsTree::RemoveFace (Ptr<Face> face)
{
  tree_type::recursive_iterator item (&m_tree), end;
  for (; item != end; item ++)
    {
      item->payload ().RemoveFace (face);
    }
}

std::ostream &
operator << (std::ostream &os, const StatsTree &tree)
{
  // os << "[" << tree.m_tree.key () << "]: " << tree.m_tree.payload ();
  os << tree.m_tree;
  return os;
}


} // namespace ndnSIM
} // namespace ndn
} // namespace ns3

