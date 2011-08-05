//
//  ndn_nonceheader.h
//  Abstraction
//
//  Created by Ilya Moiseenko on 04.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ns3/header.h"
#include <iostream>

namespace ns3
{
    namespace NDNabstraction
    {
        class NonceHeader: public Header
        {
        public:
            NonceHeader();
            NonceHeader(uint32_t nonce);
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