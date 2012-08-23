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
#include "ns3/log.h"

#include "ns3/assert.h"

using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("ndn.PitQueue");

namespace ns3 {
namespace ndn {

PitQueue::PitQueue ()
  // : m_maxQueueSize (20)
  : m_lastQueue (m_queues.end ())
{
}
  
// void
// PitQueue::SetMaxQueueSize (uint32_t size)
// {
//   m_maxQueueSize = size;
// }

// uint32_t
// PitQueue::GetMaxQueueSize () const
// {
//   return m_maxQueueSize;
// }


bool
PitQueue::Enqueue (Ptr<Face> inFace,
		   Ptr<pit::Entry> pitEntry)
{
  PerInFaceQueue::iterator queue = m_queues.find (inFace);
  if (queue == m_queues.end ())
    {
      pair<PerInFaceQueue::iterator, bool> itemPair = m_queues.insert (make_pair (inFace, boost::make_shared<Queue> ()));
      m_lastQueue = m_queues.end (); // for some reason end() iterator is invalidated when new item is inserted
      NS_ASSERT (itemPair.second == true);

      queue = itemPair.first;
    }

  if ((inFace->GetLimits ().GetMaxLimit () == 0 && queue->second->size () > 100) ||
      (inFace->GetLimits ().GetMaxLimit () != 0 && queue->second->size () >= 0.5 * inFace->GetLimits ().GetMaxLimit ()))
    {
      return false;
    }

  Queue::iterator itemIterator = queue->second->insert (queue->second->end (), pitEntry);
  
  shared_ptr<fw::PitQueueTag> tag = pitEntry->GetFwTag<fw::PitQueueTag> ();
  if (tag == shared_ptr<fw::PitQueueTag> ())
    {
      tag = make_shared<fw::PitQueueTag> ();
      pitEntry->AddFwTag (tag);
    }
  tag->InsertQueue (queue->second, itemIterator);
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
    {
      queue = m_queues.begin (); // circle to the beginning
    }

  while (queue != m_queues.end () && queue->second->size () == 0) // advance iterator
    {
      queue ++;
    }
  
  if (queue == m_queues.end ()) // e.g., begin () == end ()
    return 0;

  NS_ASSERT_MSG (queue->second->size () != 0, "Logic error");

  Ptr<pit::Entry> entry = *queue->second->begin ();
  shared_ptr<fw::PitQueueTag> tag = entry->GetFwTag<fw::PitQueueTag> ();
  NS_ASSERT (tag != shared_ptr<fw::PitQueueTag> ());

#ifdef NS3_LOG_ENABLE
  size_t queueSize = queue->second->size ();
#endif
  tag->RemoveFromQueue (queue->second);
#ifdef NS3_LOG_ENABLE
  NS_ASSERT_MSG (queue->second->size () == queueSize-1, "Queue size should be reduced by one");
#endif
    
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

  PerInFaceQueue::iterator queue = m_queues.find (face);
  if (queue == m_queues.end ())
    return;

  for (Queue::iterator pitEntry = queue->second->begin ();
       pitEntry != queue->second->end ();
       pitEntry ++)
    {
      shared_ptr<fw::PitQueueTag> tag = (*pitEntry)->GetFwTag<fw::PitQueueTag> ();
      NS_ASSERT (tag != shared_ptr<fw::PitQueueTag> ());

      tag->RemoveFromQueue (queue->second);
    }

  NS_ASSERT_MSG (queue->second->size () == 0, "Queue size should be 0 by now");
  m_queues.erase (queue);
}

void
PitQueue::Remove (Ptr<pit::Entry> entry)
{
  shared_ptr<fw::PitQueueTag> tag = entry->GetFwTag<fw::PitQueueTag> ();
  if (tag == shared_ptr<fw::PitQueueTag> ())
    return;

  tag->RemoveFromAllQueues ();
}

bool
PitQueue::IsEmpty () const
{
  bool isEmpty = true;

  for (PerInFaceQueue::const_iterator queue = m_queues.begin ();
       queue != m_queues.end ();
       queue ++)
    {
      isEmpty &= (queue->second->size () == 0);
    }

  return isEmpty;
}

void
fw::PitQueueTag::InsertQueue (boost::shared_ptr<PitQueue::Queue> queue, PitQueue::Queue::iterator iterator)
{
  pair<MapOfItems::iterator, bool> item = m_items.insert (make_pair (queue, iterator));
  NS_ASSERT (item.second == true);
}

void
fw::PitQueueTag::RemoveFromAllQueues ()
{
  for (MapOfItems::iterator item = m_items.begin ();
       item != m_items.end ();
       item ++)
    {
      item->first->erase (item->second);
    }
  m_items.clear ();
}

void
fw::PitQueueTag::RemoveFromQueue (boost::shared_ptr<PitQueue::Queue> queue)
{
  MapOfItems::iterator item = m_items.find (queue);
  if (item == m_items.end ())
    return;

  item->first->erase (item->second);
  m_items.erase (item);
}

} // namespace ndn
} // namespace ns3
