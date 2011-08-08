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

#include "ndn_timeoutheader.h"

namespace ns3 
{
namespace NDNabstraction
{
    NS_OBJECT_ENSURE_REGISTERED (TimeoutHeader);
    
    TimeoutHeader::TimeoutHeader()
    {
        m_value = 4000;
    }
    
    TimeoutHeader::TimeoutHeader (uint32_t timeout) 
    {
        m_value = timeout;
    }
    
    TypeId
    TimeoutHeader::GetTypeId ()
    {
        static TypeId tid = TypeId ("ns3::NDNabstraction::TimeoutHeader")
        .SetParent<Header> ()
        .AddConstructor<TimeoutHeader> ()
        ;
        return tid;
    }
    
    TypeId
    TimeoutHeader::GetInstanceTypeId () const
    {
        return GetTypeId ();
    }
    
    uint32_t
    TimeoutHeader::GetSerializedSize () const
    {
        return 4;
    }
    
    void
    TimeoutHeader::Serialize (Buffer::Iterator i) const
    {
        i.WriteU32 ((uint32_t) m_value);
    }
    
    uint32_t
    TimeoutHeader::Deserialize (Buffer::Iterator start)
    {
        Buffer::Iterator i = start;
        m_value = i.ReadU32 ();
            
        uint32_t dist = i.GetDistanceFrom (start);
        NS_ASSERT (dist == GetSerializedSize ());
        return dist;
    }

    void
    TimeoutHeader::Print (std::ostream &os) const
    {
        os << m_value;
    }
    
    uint32_t
    TimeoutHeader::GetValue()
    {
        return m_value;
    }
}    
}
