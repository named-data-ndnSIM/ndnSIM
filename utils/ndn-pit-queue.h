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

class PitQueue
{
public:
  PitQueue ();
  
  void
  SetMaxQueueSize (uint32_t size);

  uint32_t
  GetMaxQueueSize () const;

  bool
  Enqueue (Ptr<Face> inFace,
           Ptr<pit::Entry> pitEntry);

  Ptr<pit::Entry>
  Pop ();

  // cleanup procedures
  void
  Remove (Ptr<Face> face);

  void
  Remove (Ptr<pit::Entry> entry);

public:  
  typedef std::list< Ptr<pit::Entry> > Queue;
  typedef std::map< Ptr<Face>, boost::shared_ptr<Queue> > PerInFaceQueue;

private:
  
  uint32_t m_maxQueueSize;
  PerInFaceQueue::iterator m_lastQueue; // last queue from which interest was taken
  PerInFaceQueue m_queues;
};

namespace fw {

class PitQueueTag :
    public Tag
{
public:
  virtual
  ~PitQueueTag () { };

  typedef boost::tuple< boost::shared_ptr<PitQueue::Queue>, PitQueue::Queue::iterator > Item;
};

} // namespace fw

} // namespace ndn
} // namespace ns3

#endif // NDN_PIT_QUEUE_H
