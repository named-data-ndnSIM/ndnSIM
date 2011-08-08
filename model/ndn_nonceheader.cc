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
