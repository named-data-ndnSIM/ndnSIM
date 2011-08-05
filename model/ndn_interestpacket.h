//
//  ndn_interestpacket.h
//  Abstraction
//
//  Copyright 2011 UCLA. All rights reserved.
//

#ifndef ndn_interestpacket_h
#define ndn_interestpacket_h

//#define CCN_INTEREST_LIFETIME_SEC 4
//#define CCN_INTEREST_LIFETIME_MICROSEC (CCN_INTEREST_LIFETIME_SEC * 1000000)

#include "ns3/header.h"
#include <ns3/packet.h>
#include <string>
#include "ndn_timeoutheader.h"
#include "ndn_nonceheader.h"

namespace ns3
{
namespace NDNabstraction
{
    class InterestPacket : public Packet 
    {
        public: 
            InterestPacket(unsigned char *name, uint32_t size);
            
            uint32_t GetName(unsigned char *name);
        
            void AddTimeout(uint32_t milliseconds);
            uint32_t GetTimeout(void);
            void RemoveTimeout(void);
        
            void AddNonce(uint32_t nonce);
            uint32_t GetNonce(void);
            void RemoveNonce(void);
        
            uint32_t maxNameLength;
    };
}
}

#endif 