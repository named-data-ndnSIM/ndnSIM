//
//  ndn_interestpacket.cc
//  Abstraction
//
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ndn_interestpacket.h"

namespace ns3
{
namespace NDNabstraction
{
    InterestPacket::InterestPacket(unsigned char *name, uint32_t size):Packet ((uint8_t const *)name,size)
    {
        maxNameLength = 10240;
    }
    
    uint32_t 
    InterestPacket::GetName(unsigned char *name)
    {
        //uint32_t Packet::CopyData (uint8_t *buffer, uint32_t size) const
        return CopyData((uint8_t*) name, maxNameLength); 
    }
    
    void 
    InterestPacket::AddTimeout(uint32_t milliseconds)
    {
        TimeoutHeader tHeader (milliseconds);
        AddHeader (tHeader);    
    }
    
    uint32_t
    InterestPacket::GetTimeout(void)
    {
        TimeoutHeader tHeader;
        PeekHeader(tHeader);
        return tHeader.GetValue();
    }
    
    void
    InterestPacket::RemoveTimeout(void)
    {
        TimeoutHeader tHeader;
        RemoveHeader(tHeader);
    }
    
    void 
    InterestPacket::AddNonce(uint32_t nonce)
    {
        NonceHeader tHeader (nonce);
        AddHeader (tHeader);    
    }
    
    uint32_t
    InterestPacket::GetNonce(void)
    {
        NonceHeader tHeader;
        PeekHeader(tHeader);
        return tHeader.GetValue();
    }
    
    void
    InterestPacket::RemoveNonce(void)
    {
        NonceHeader tHeader;
        RemoveHeader(tHeader);
    }
}
}