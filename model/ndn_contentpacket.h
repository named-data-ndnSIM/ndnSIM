//
//  ndn_contentpacket.h
//  Abstraction
//
//  Copyright 2011 UCLA. All rights reserved.
//

#ifndef ndn_contentpacket_h
#define ndn_contentpacket_h

#include "ns3/header.h"
#include <ns3/packet.h>
#include "ccn_ccn.h"

namespace ns3
{
namespace NDNabstraction
{
    class ContentPacket : public Packet 
    {
    public: 
        ContentPacket(const struct ccn_charbuf *Name,const void *data,size_t size);

        
    };
}
}

#endif 