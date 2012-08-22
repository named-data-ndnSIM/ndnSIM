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

#ifndef NDN_PIT_QUEUE_H
#define NDN_PIT_QUEUE_H

#include <map>
#include <list>

#include "ns3/ptr.h"
#include "ns3/ndn-fw-tag.h"

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace ns3 {
namespace ndn {

class Face;
namespace pit { class Entry; }

/**
 * @ingroup ndn
 * @brief Queue for PIT entries, interests for which cannot be immediately forwarded
 */
class PitQueue
{
public:
  /**
   * @brief Default constructor
   */
  PitQueue ();

  /**
   * @brief Set maximum queue size
   * @param size per-incoming face maximum queue size
   *
   * Each per-incoming-face queue will have this maximum size
   */
  void
  SetMaxQueueSize (uint32_t size);

  /**
   * @brief Get current maximum queue size
   * @returns per-incoming face maximum queue size
   */
  uint32_t
  GetMaxQueueSize () const;

  /**
   * @brief Enqueue PIT entry for delayed processing
   * @param inFace incoming face to which queue PIT entry should enqueued
   * @param pitEntry smart pointer to PIT entry
   * return true if successfully enqueued, false if limit is reached or some other reason
   */
  bool
  Enqueue (Ptr<Face> inFace,
           Ptr<pit::Entry> pitEntry);

  /**
   * @brief Get next PIT entry
   * @returns next PIT entry or 0 if no entries available
   *
   * This method implement round-robin (in future weighted round-robin) to pick elements from different per-in-face queues
   */
  Ptr<pit::Entry>
  Pop ();

  /**
   * @brief Remove all references to face from all queues and enqueued PIT entries
   * @param face smart pointer to face
   */
  void
  Remove (Ptr<Face> face);

  /**
   * @brief Remove all references to PIT entry from queues
   * @param entry smart pointer to PIT entry
   */
  static void
  Remove (Ptr<pit::Entry> entry);

  /**
   * @brief Check if queue is empty
   */
  bool
  IsEmpty () const;
  
public:  
  typedef std::list< Ptr<pit::Entry> > Queue;
  typedef std::map< Ptr<Face>, boost::shared_ptr<Queue> > PerInFaceQueue;

private:
  // uint32_t m_maxQueueSize;
  PerInFaceQueue::iterator m_lastQueue; // last queue from which interest was taken
  PerInFaceQueue m_queues;
};

namespace fw {

/**
 * @ingroup ndn
 * @brief Forwarding strategy tag that stores queue-related information in PIT entries
 */
class PitQueueTag :
    public Tag
{
public:
  // map based on addresses, should be good enough
  typedef std::map< boost::shared_ptr<PitQueue::Queue>, PitQueue::Queue::iterator > MapOfItems;

public:
  /**
   * @brief Virtual destructor
   */
  virtual
  ~PitQueueTag () { };  

  /**
   * @brief Remember in which queue at which position PIT entry is enqueued
   * @brief item     smart pointer to Queue
   * @brief iterator queue's iterator
   */
  void
  InsertQueue (boost::shared_ptr<PitQueue::Queue> item, PitQueue::Queue::iterator iterator);

  /**
   * @brief Remove PIT entry from all queues
   */
  void
  RemoveFromAllQueues ();

  /**
   * @brief Remove PIT entry from the specified queue
   * @param queue Queue from which PIT entry should be removed
   */
  void
  RemoveFromQueue (boost::shared_ptr<PitQueue::Queue> queue);
  
private:
  MapOfItems m_items;
};

} // namespace fw

} // namespace ndn
} // namespace ns3

#endif // NDN_PIT_QUEUE_H
