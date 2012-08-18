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

PitQueue::Entry::Entry (Ptr<Face> inFace, Ptr<pit::Entry> pitEntry, double virtualTime)
  : m_inFace (inFace)
  , m_pitEntry (pitEntry)
  , m_virtualTime (virtualTime)
{
}

bool
PitQueue::Entry::operator < (const Entry &otherEntry) const
{
  return this->m_virtualTime < otherEntry.m_virtualTime;
}


bool
PitQueue::Enqueue (Ptr<Face> inFace,
		   Ptr<pit::Entry> pitEntry)
{
  return false;
}

Ptr<pit::Entry>
PitQueue::Pop ()
{
  if (m_queue.size () == 0)
    return 0;

  PendingInterestsQueue::iterator topEntryIterator = m_queue.begin ();
  Entry entry = *topEntryIterator; // copy entry
  m_queue.erase (topEntryIterator); // remove entry

  DecreasePerFaceCount (entry.m_inFace);
  
  if (m_queue.size () == 0)
    m_lastProcessedVirtualTime = 0;
  else
    m_lastProcessedVirtualTime = entry.m_virtualTime;


  return entry.m_pitEntry;
}

void
PitQueue::DecreasePerFaceCount (Ptr<Face> inFace)
{
  PerInFaceMapOfNumberOfIncomingInterests::iterator numberOfEnqueued = m_numberEnqueuedInterests.find (inFace);
  NS_ASSERT_MSG (numberOfEnqueued != m_numberEnqueuedInterests.end () &&
		 numberOfEnqueued->second > 0,
		 "Logic error");
  
  numberOfEnqueued->second --;
  if (numberOfEnqueued->second == 0)
    {
      m_numberEnqueuedInterests.erase (numberOfEnqueued);
      m_lastVirtualTime.erase (inFace);
    }
}

void
PitQueue::Remove (Ptr<Face> face)
{
  m_numberEnqueuedInterests.erase (face);
  m_lastVirtualTime.erase (face);

  for (PendingInterestsQueue::iterator item = m_queue.begin ();
       item != m_queue.end ();
       /* manual iterator advancement */)
    {
      if (item->m_inFace == face)
	{
	  PendingInterestsQueue::iterator toRemove = item;
	  item ++;
	  m_queue.erase (toRemove); // hopefully a safe operation
	}
      else
	item ++;
    }
}

void
PitQueue::Remove (Ptr<pit::Entry> entry)
{
  for (PendingInterestsQueue::iterator item = m_queue.begin ();
       item != m_queue.end ();
       /* manual iterator advancement */)
    {
      // compare addresses
      if (&(*entry) == &(**item))
	{
	  PendingInterestsQueue::iterator toRemove = item;
	  item ++;

	  DecreasePerFaceCount (toRemove->m_inFace);
	  UpdateLastVirtTime (toRemove->m_inFace);
	  
	  m_queue.erase (toRemove); // hopefully a safe operation
	  break; // there should be at most one item for one pit entry
	}
      else
	item ++;
    }  
}

void
PitQueue::UpdateLastVirtTime (Ptr<Face> inFace)
{
  PerInFaceMapOfNumberOfIncomingInterests::iterator numberOfEnqueued = m_numberEnqueuedInterests.find (inFace);
  if (numberOfEnqueued->second == 0)
    return;

  double lastTime = 0;
  for (PendingInterestsQueue::iterator item = m_queue.begin ();
       item != m_queue.end ();
       item ++)
    {
      if (inFace == item->m_inFace)
	{
	  if (lastTime < item->m_virtualTime)
	    lastTime = item->m_virtualTime;
	}
    }

  m_lastVirtualTime [inFace] = lastTime; 
}


} // namespace ndn
} // namespace ns3
