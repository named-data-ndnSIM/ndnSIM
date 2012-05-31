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
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */
 
#include "weights-path-stretch-tag.h"
#include "ns3/node.h"
#include "ns3/names.h"

namespace ns3 {

WeightsPathStretchTag::WeightsPathStretchTag ()
{
}

void
WeightsPathStretchTag::AddPathInfo (Ptr<Node> node, uint32_t weight)
{
  m_infos.push_back (NodeWeightPair (node, weight));
}


TypeId WeightsPathStretchTag::GetTypeId ()
{
  static TypeId tid = TypeId("ns3::WeightsPathStretchTag")
    .SetParent<Tag>()
    .AddConstructor<WeightsPathStretchTag>()
    ;
  return tid;
}

// TypeId
// WeightsPathStretchTag::GetInstanceTypeId () const
// {
//   return GetTypeId ();
// }

uint64_t
WeightsPathStretchTag::GetTotalWeight () const
{
  uint64_t total = 0;
  for (std::list<NodeWeightPair>::const_iterator info = m_infos.begin (); info != m_infos.end (); info++)
    {
      total += info->weight;
    }
  return total;
}

Ptr<Node>
WeightsPathStretchTag::GetSourceNode () const
{
  NS_ASSERT (m_infos.size () > 0);
  return m_infos.front ().node;
}

Ptr<Node>
WeightsPathStretchTag::GetDestinationNode () const
{
  NS_ASSERT (m_infos.size () > 0);
  return m_infos.back ().node;
}

uint32_t WeightsPathStretchTag::GetSerializedSize (void) const
{
  return 0;
  // return sizeof (GetPointer (m_value.node)) + sizeof (m_value.weight);
}

void WeightsPathStretchTag::Serialize (TagBuffer i) const
{
  NS_FATAL_ERROR ("Serialization is not supported for this tag");
  // m_value.node->Ref ();
  // i.WriteU64 (reinterpret_cast<uint64_t> (GetPointer (m_value.node)));
  // i.WriteU32 (m_value.weight);
}

void WeightsPathStretchTag::Deserialize (TagBuffer i)
{
  NS_FATAL_ERROR ("Deserialization is not supported for this tag");
  // m_value.node = Ptr<Node> (reinterpret_cast<Node*> (i.ReadU64 ()), false);
  // m_value.weight = i.ReadU32 ();
}

void WeightsPathStretchTag::Print (std::ostream &os) const
{
  for (std::list<NodeWeightPair>::const_iterator info = m_infos.begin ();
       info != m_infos.end ();
       info ++)
    {
      if (info != m_infos.begin ()) os << ",";
      NS_ASSERT (info->node != 0);

      os << info->node->GetId () << "(" << Names::FindName (info->node) << ")";
      // std::string name = Names::FindName (info->node);
      // if (!name.empty ())
      //   os << name;
      // else
      //   os << info->node->GetId ();
      os << ":" << info->weight;
    }
}

} // namespace ns3

