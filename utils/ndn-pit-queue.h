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
#include <set>

#include "ns3/ptr.h"

namespace ns3 {
namespace ndn {

class Face;
namespace pit { class Entry; }

class PitQueue
{
public:
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

private:
  void
  DecreasePerFaceCount (Ptr<Face> inFace);

  void
  UpdateLastVirtTime (Ptr<Face> inFace);
  
private:
  // all maps here consider incoming face (the whole Queue structure is per outgoing face)
  
  double m_lastProcessedVirtualTime;

  typedef std::map< Ptr<Face>, uint32_t > PerInFaceMapOfNumberOfIncomingInterests;
  PerInFaceMapOfNumberOfIncomingInterests m_numberEnqueuedInterests;

  typedef std::map< Ptr<Face>, double > PerInFaceMapOfLastVirtualTimes;
  PerInFaceMapOfLastVirtualTimes m_lastVirtualTime;

  struct Entry
  {
    Entry (Ptr<Face> inFace, Ptr<pit::Entry> pitEntry, double virtualTime);

    bool
    operator < (const Entry &otherEntry) const;

    Ptr<Face> m_inFace;
    Ptr<pit::Entry> m_pitEntry;
    double m_virtualTime;
  };

  typedef std::multiset< Entry > PendingInterestsQueue;
  PendingInterestsQueue m_queue;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_PIT_QUEUE_H
