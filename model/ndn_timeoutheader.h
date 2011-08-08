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

#include "ns3/header.h"
#include <iostream>

namespace ns3
{
namespace NDNabstraction
{
        
    class TimeoutHeader: public Header
    {
    public:
        TimeoutHeader();
        TimeoutHeader(uint32_t timeout);
        uint32_t GetValue();
        
        
        // Header serialization/deserialization
        static TypeId GetTypeId ();
        TypeId GetInstanceTypeId () const;
        uint32_t GetSerializedSize () const;
        void Serialize (Buffer::Iterator start) const;
        uint32_t Deserialize (Buffer::Iterator start);
        void Print (std::ostream &os) const;
        
    private:
        uint32_t m_value;
    };
}
}
