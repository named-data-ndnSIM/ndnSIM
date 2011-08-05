//
//  ndn_nonceheader.cpp
//  Abstraction
//
//  Created by Ilya Moiseenko on 04.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ndn_nonceheader.h"


namespace ns3 
{
    namespace NDNabstraction
    {
        NS_OBJECT_ENSURE_REGISTERED (NonceHeader);
        
        NonceHeader::NonceHeader()
        {
            m_value = 0;
        }
        
        NonceHeader::NonceHeader (uint32_t nonce) 
        {
            m_value = nonce;
        }
        
        TypeId
        NonceHeader::GetTypeId ()
        {
            static TypeId tid = TypeId ("ns3::NDNabstraction::NonceHeader")
            .SetParent<Header> ()
            .AddConstructor<NonceHeader> ()
            ;
            return tid;
        }
        
        TypeId
        NonceHeader::GetInstanceTypeId () const
        {
            return GetTypeId ();
        }
        
        uint32_t
        NonceHeader::GetSerializedSize () const
        {
            return 4;
        }
        
        void
        NonceHeader::Serialize (Buffer::Iterator i) const
        {
            i.WriteU32 ((uint32_t) m_value);
        }
        
        uint32_t
        NonceHeader::Deserialize (Buffer::Iterator start)
        {
            Buffer::Iterator i = start;
            m_value = i.ReadU32 ();
            
            uint32_t dist = i.GetDistanceFrom (start);
            NS_ASSERT (dist == GetSerializedSize ());
            return dist;
        }
        
        void
        NonceHeader::Print (std::ostream &os) const
        {
            os << m_value;
        }
        
        uint32_t
        NonceHeader::GetValue()
        {
            return m_value;
        }
    }    
}