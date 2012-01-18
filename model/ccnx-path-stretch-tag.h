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
    WeightsPathStretchTag();
    WeightsPathStretchTag(const WeightsPathStretchTag &o);
    WeightsPathStretchTag &operator = (const WeightsPathStretchTag &o);

    bool operator == (WeightsPathStretchTag const &o) const;

    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;

    virtual uint32_t GetSerializedSize(void) const;
    virtual void Serialize(TagBuffer i) const;
    virtual void Deserialize(TagBuffer i);
    virtual void Print(std::ostream &os) const;

    void AddNewHop(uint32_t weight);
    uint32_t GetValue();
    
private:
    uint32_t m_weightPathStretch;
};

class DelaysPathStretchTag : public Tag
{
public:
    DelaysPathStretchTag();
    //PathSplicingPathTag(const PathSplicingPathTag &o);
    //PathSplicingPathTag &operator = (const PathSplicingPathTag &o);

    //bool operator == (PathSplicingPathTag const &o) const;

    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;

    virtual uint32_t GetSerializedSize(void) const;
    virtual void Serialize(TagBuffer i) const;
    virtual void Deserialize(TagBuffer i);
    virtual void Print(std::ostream &os) const;

    int GetNHops();
    void AddNewHop(double delay);
    int GetCurrentHop();
    void RemoveCurrentHop();
    //void RandomizeTags(UniformVariable &rand, uint32_t max);

private:
    std::list<double> m_delays;
};


} // namespace ns3


#endif /* PATH_STRETCH_TAG_H */