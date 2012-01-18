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
 
#include "ccnx-path-stretch-tag.h"

namespace ns3 {

WeightsPathStretchTag::WeightsPathStretchTag()
    : m_weightPathStretch(0)
{
}

WeightsPathStretchTag::WeightsPathStretchTag(const WeightsPathStretchTag &o)
    : m_weightPathStretch(o.m_weightPathStretch)
{
}

WeightsPathStretchTag &WeightsPathStretchTag::operator = (const WeightsPathStretchTag &o)
{
    if (this == &o)
        return *this;

    m_weightPathStretch = o.m_weightPathStretch;
    return *this;
}

bool WeightsPathStretchTag::operator == (WeightsPathStretchTag const &o) const
{
    return (m_weightPathStretch == o.m_weightPathStretch);
}

TypeId WeightsPathStretchTag::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::WeightsPathStretchTag")
        .SetParent<Tag>()
        .AddConstructor<WeightsPathStretchTag>()
        ;
    return tid;
}

TypeId WeightsPathStretchTag::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

uint32_t WeightsPathStretchTag::GetSerializedSize(void) const
{
    return sizeof(uint32_t);
}

void WeightsPathStretchTag::Serialize(TagBuffer i) const
{
    i.WriteU32(m_weightPathStretch);
}

void WeightsPathStretchTag::Deserialize(TagBuffer i)
{
    m_weightPathStretch = i.ReadU32();
}

void WeightsPathStretchTag::Print(std::ostream &os) const
{
    os << "Path Stretch (Weights) = " << m_weightPathStretch << std::endl;
}

void WeightsPathStretchTag::AddNewHop(uint32_t weight)
{
    m_weightPathStretch +=weight;
}
uint32_t WeightsPathStretchTag::GetValue()
{
  return m_weightPathStretch;
}
} // namespace ns3

