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

const double MIN_WEIGHT = 0.01;

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
		   Ptr<pit::Entry> pitEntry,
                   double updatedWeight)
{
  if (updatedWeight < MIN_WEIGHT) updatedWeight = MIN_WEIGHT;
  
  PerInFaceQueue::iterator queue = m_queues.find (inFace);
  if (queue == m_queues.end ())
    {
      pair<PerInFaceQueue::iterator, bool> itemPair =
        m_queues.insert (make_pair (inFace, boost::make_shared< WeightedQueue > (make_tuple (Queue (), updatedWeight, 0))));
      m_lastQueue = m_queues.end (); // for some reason end() iterator is invalidated when new item is inserted
      NS_ASSERT (itemPair.second == true);

      queue = itemPair.first;
    }

  if ((inFace->GetLimits ().GetMaxLimit () == 0 && queue->second->get<0> ().size () > 100) ||
      (inFace->GetLimits ().GetMaxLimit () != 0 && queue->second->get<0> ().size () >= 0.5 * inFace->GetLimits ().GetMaxLimit ()))
    {
      return false;
    }

  Queue::iterator itemIterator = queue->second->get<0> ().insert (queue->second->get<0> ().end (), pitEntry);
  
  shared_ptr<fw::PitQueueTag> tag = pitEntry->GetFwTag<fw::PitQueueTag> ();
  if (tag == shared_ptr<fw::PitQueueTag> ())
    {
      tag = make_shared<fw::PitQueueTag> ();
      pitEntry->AddFwTag (tag);
    }
  tag->InsertQueue (queue->second, itemIterator);

  UpdateWeightedRounds ();
  return true;
}


Ptr<pit::Entry>
PitQueue::Pop ()
{
  PerInFaceQueue::iterator queue = m_lastQueue;

  // if (queue != m_queues.end () &&
  //     true)
  //   {
  //     queue ++; // actually implement round robin...
  //   } 

  while (queue != m_queues.end () && queue->second->get<0> ().size () == 0) // advance iterator
    {
      queue ++;
      m_serviceCounter = 0;
    }

  if (queue == m_queues.end ())
    {
      queue = m_queues.begin (); // circle to the beginning
      m_serviceCounter = 0;
    }

  while (queue != m_queues.end () && queue->second->get<0> ().size () == 0) // advance iterator
    {
      queue ++;
      m_serviceCounter = 0;
    }
  
  if (queue == m_queues.end ()) // e.g., begin () == end ()
    return 0;

  NS_ASSERT_MSG (queue->second->get<0> ().size () != 0, "Logic error");

  Ptr<pit::Entry> entry = *queue->second->get<0> ().begin ();
  shared_ptr<fw::PitQueueTag> tag = entry->GetFwTag<fw::PitQueueTag> ();
  NS_ASSERT (tag != shared_ptr<fw::PitQueueTag> ());

#ifdef NS3_LOG_ENABLE
  size_t queueSize = queue->second->get<0> ().size ();
#endif
  tag->RemoveFromQueue (queue->second);
#ifdef NS3_LOG_ENABLE
  NS_ASSERT_MSG (queue->second->get<0> ().size () == queueSize-1, "Queue size should be reduced by one");
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

  for (Queue::iterator pitEntry = queue->second->get<0> ().begin ();
       pitEntry != queue->second->get<0> ().end ();
       pitEntry ++)
    {
      shared_ptr<fw::PitQueueTag> tag = (*pitEntry)->GetFwTag<fw::PitQueueTag> ();
      NS_ASSERT (tag != shared_ptr<fw::PitQueueTag> ());

      tag->RemoveFromQueuesExcept (queue->second);
    }

  NS_ASSERT_MSG (queue->second->get<0> ().size () == 0, "Queue size should be 0 by now");
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
      isEmpty &= (queue->second->get<0> ().size () == 0);
    }

  return isEmpty;
}

bool
PitQueue::IsEmpty (Ptr<Face> inFace) const
{
  PerInFaceQueue::const_iterator queue = m_queues.find (inFace);
  if (queue == m_queues.end ())
    return true;

  return queue->second->get<0> ().size () == 0;
}


void
PitQueue::UpdateWeightedRounds ()
{
  double minWeight = 100.0;
  for (PerInFaceQueue::const_iterator queue = m_queues.begin ();
       queue != m_queues.end ();
       queue ++)
    {
      if (queue->second->get<1> () < minWeight)
        minWeight = queue->second->get<1> ();
    }

  for (PerInFaceQueue::const_iterator queue = m_queues.begin ();
       queue != m_queues.end ();
       queue ++)
    {
      queue->second->get<2> () = static_cast<uint32_t>((queue->second->get<1> () + 0.5) / minWeight);
    }
}


////////////////////////////////////////////////////////////////////////////////

void
fw::PitQueueTag::InsertQueue (boost::shared_ptr<PitQueue::WeightedQueue> queue, PitQueue::Queue::iterator iterator)
{
  // std::cerr << "size before: " << m_items.size () << " item: " << (*iterator)->GetPrefix ();
  pair<MapOfItems::iterator, bool> item = m_items.insert (make_pair (queue, iterator));
  // std::cerr << " and after: " << m_items.size () << std::endl; 
  NS_ASSERT_MSG (item.second == true, "Should be a new tag for PIT entry, but something is wrong");
}

void
fw::PitQueueTag::RemoveFromAllQueues ()
{
  for (MapOfItems::iterator item = m_items.begin ();
       item != m_items.end ();
       item ++)
    {
      item->first->get<0> ().erase (item->second);
    }
  m_items.clear ();
}

void
fw::PitQueueTag::RemoveFromQueue (boost::shared_ptr<PitQueue::WeightedQueue> queue)
{
  MapOfItems::iterator item = m_items.find (queue);
  if (item == m_items.end ())
    return;

  item->first->get<0> ().erase (item->second);
  m_items.erase (item);
}

void
fw::PitQueueTag::RemoveFromQueuesExcept (boost::shared_ptr<PitQueue::WeightedQueue> queue)
{
  for (MapOfItems::iterator item = m_items.begin ();
       item != m_items.end (); )
    {
      if (item->first == queue)
        {
          item ++;
          continue;
        }

      item->first->get<0> ().erase (item->second);

      MapOfItems::iterator itemToDelete = item;
      item ++;
      m_items.erase (itemToDelete);
    }
}

bool
fw::PitQueueTag::IsLastOneInQueues () const
{
  bool lastOne = true;
  
  for (MapOfItems::const_iterator item = m_items.begin ();
       item != m_items.end ();
       item ++)
    {
      lastOne &= (item->first->get<0> ().size () == 1);
    }

  return lastOne;
}


} // namespace ndn
} // namespace ns3
