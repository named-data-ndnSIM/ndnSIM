/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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

#include "ndn-pit-queue.h"

#include "ns3/ndn-face.h"
#include "ns3/ndn-pit-entry.h"

#include "ns3/assert.h"

using namespace std;

namespace ns3 {
namespace ndn {

PitQueue::PitQueue ()
  : m_maxQueueSize (10)
  , m_lastQueue (m_queues.begin ())
{
}
  
void
PitQueue::SetMaxQueueSize (uint32_t size)
{
  m_maxQueueSize = size;
}

uint32_t
PitQueue::GetMaxQueueSize () const
{
  return m_maxQueueSize;
}


bool
PitQueue::Enqueue (Ptr<Face> inFace,
		   Ptr<pit::Entry> pitEntry)
{
  PerInFaceQueue::iterator queue = m_queues.find (inFace);
  if (queue == m_queues.end ())
    {
      pair<PerInFaceQueue::iterator, bool> itemPair = m_queues.insert (make_pair (inFace, boost::make_shared<Queue> ()));
      NS_ASSERT (itemPair.second == true);

      queue = itemPair.first;
    }
  
  if (queue->second->size () >= m_maxQueueSize)
      return false;

  queue->second->push_back (pitEntry);
  return true;
}

Ptr<pit::Entry>
PitQueue::Pop ()
{
  PerInFaceQueue::iterator queue = m_lastQueue;

  while (queue != m_queues.end () && queue->second->size () == 0) // advance iterator
    {
      queue ++;
    }

  if (queue == m_queues.end ())
    queue = m_queues.begin (); // circle to the beginning

  while (queue != m_queues.end () && queue->second->size () == 0) // advance iterator
    {
      queue ++;
    }
  
  if (queue == m_queues.end ()) // e.g., begin () == end ()
    return 0;

  NS_ASSERT_MSG (queue->second->size () != 0, "Logic error");

  Ptr<pit::Entry> entry = *queue->second->begin ();
  queue->second->pop_front ();

  m_lastQueue = queue;
  return entry;
}

void
PitQueue::Remove (Ptr<Face> face)
{
  if (m_lastQueue->first == face)
    {
      m_lastQueue++;
    }

  m_queues.erase (face);
}

void
PitQueue::Remove (Ptr<pit::Entry> entry)
{
}


} // namespace ndn
} // namespace ns3
