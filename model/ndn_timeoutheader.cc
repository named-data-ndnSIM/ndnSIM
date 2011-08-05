//
//  ndn_timeoutheader.cpp
//  Abstraction
//
//  Created by Ilya Moiseenko on 04.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

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