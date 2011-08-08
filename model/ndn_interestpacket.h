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
