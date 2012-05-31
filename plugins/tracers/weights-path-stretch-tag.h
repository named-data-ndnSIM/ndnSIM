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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */
 
#include "ns3/tag.h"
 
#ifndef PATH_STRETCH_TAG_H
#define PATH_STRETCH_TAG_H

namespace ns3 {

class Node;
class Packet;

//#define PATH_SPLICING_MAX_N_HOPS 38
//#define PATH_SPLICING_MAX_SLICE_INDEX 16

class WeightsPathStretchTag : public Tag
{
public:
  struct NodeWeightPair
  {
    NodeWeightPair () : node (0), weight (0) { }
    NodeWeightPair (Ptr<Node> _node, uint32_t _weight) : node (_node), weight (_weight) { }
    
    Ptr<Node> node;
    uint32_t  weight;
  };
  
  static TypeId
  GetTypeId ();

  WeightsPathStretchTag ();
  virtual ~WeightsPathStretchTag () { };

  void
  AddPathInfo (Ptr<Node> node, uint32_t weight);

  uint64_t
  GetTotalWeight () const;

  Ptr<Node>
  GetSourceNode () const;

  Ptr<Node>
  GetDestinationNode () const;

  const std::list<NodeWeightPair> &
  GetInfos () const
  { return m_infos; }

  // from Tag
  virtual uint32_t
  GetSerializedSize (void) const;
  
  virtual void
  Serialize (TagBuffer i) const;

  virtual void
  Deserialize (TagBuffer i);

  virtual void
  Print (std::ostream &os) const;
  
private:
  std::list<NodeWeightPair> m_infos;
};

// class DelaysPathStretchTag : public Tag
// {
// public:
//   DelaysPathStretchTag();

//   static TypeId GetTypeId(void);
//   virtual TypeId GetInstanceTypeId(void) const;

//   virtual uint32_t GetSerializedSize(void) const;
//   virtual void Serialize(TagBuffer i) const;
//   virtual void Deserialize(TagBuffer i);
//   virtual void Print(std::ostream &os) const;

//   int GetNHops();
//   void AddNewHop(double delay);
//   int GetCurrentHop();
//   void RemoveCurrentHop();
//   //void RandomizeTags(UniformVariable &rand, uint32_t max);

// private:
//   //PathSplicingPathTag(const PathSplicingPathTag &o);
//   //PathSplicingPathTag &operator = (const PathSplicingPathTag &o);

//   //bool operator == (PathSplicingPathTag const &o) const;

// private:
//   std::list<double> m_delays;
// };


} // namespace ns3


#endif /* PATH_STRETCH_TAG_H */
