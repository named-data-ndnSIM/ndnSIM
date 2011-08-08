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
