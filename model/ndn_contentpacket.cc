//
//  ndn_contentpacket.cpp
//  Abstraction
//
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ndn_contentpacket.h"

namespace ns3
{
namespace NDNabstraction
{
    ContentPacket::ContentPacket(const struct ccn_charbuf *Name,const void *data,size_t size):Packet()
    {
        ccn_charbuf *output = ccn_charbuf_create();
        int result = ccn_encode_ContentObject(output,Name,data,size);
        
        if(result>=0)
        {
            Ptr<Packet> p = Create<Packet> (output->buf, (uint32_t)output->length);
            this->AddAtEnd (p);
        }
    }
}
}