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

#ifndef CCNX_CONSUMER_H
#define CCNX_CONSUMER_H

#include "ns3/application.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/simulator.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-local-face.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/packet.h"
#include "ns3/boolean.h"
#include "ns3/integer.h"
#include "ns3/uinteger.h"
#include "ns3/random-variable.h"
#include <limits> 
#include "ns3/pointer.h"
#include "ns3/traced-callback.h"
#include "ns3/ccnx-header-helper.h"

#include "ns3/packet.h"
#include "ns3/header.h"

namespace ns3 
{

class CcnxConsumer: public Application
{
public: 
    static TypeId GetTypeId (void);
        
    CcnxConsumer ();
        
    virtual ~CcnxConsumer ();
    
    void HandlePacket (const Ptr<CcnxFace> &face, const Ptr<const Packet> &packet);
        
protected:
    virtual void DoDispose (void);
private:
    // inherited from Application base class.
    virtual void StartApplication (void);    // Called at time specified by Start
    virtual void StopApplication (void);     // Called at time specified by Stop
        
    Time m_offTime;
    Ptr<CcnxNameComponents> m_interestName;
    Time m_interestLifeTime;
    int32_t m_minSuffixComponents;
    int32_t m_maxSuffixComponents;
    bool m_childSelector;
    Ptr<CcnxNameComponents> m_exclude;
    uint32_t m_initialNonce;
        
    EventId         m_sendEvent;    // Eventid of pending "send packet" event
    TypeId          m_tid;
    Ptr<CcnxLocalFace> m_face;
        
    //helpers
    void CancelEvents ();
        
    void Construct (Ptr<Node> n,
                    std::string tid,
                    const Time& offtime,
                    Ptr<CcnxLocalFace> face,
                    Ptr<CcnxNameComponents> nameComponents,
                    const Time& lifetime,
                    const int32_t& minSuffixComponents,
                    const int32_t& maxSuffixComponents,
                    const bool childSelector,
                    Ptr<CcnxNameComponents> exclude,
                    const uint32_t& initialNonce
                    );
        
    // Event handlers
    void StartSending ();
    void StopSending ();
    void SendPacket ();
    //typedef Callback<void,const Ptr<CcnxFace>&,const Ptr<const Packet>& > ProtocolHandler;
     
private:
    void ScheduleNextTx ();
    //typedef Callback<void,const Ptr<CcnxFace>&,const Ptr<const Packet>& > ProtocolHandler;
    
    TracedCallback<const Ptr<CcnxFace>&,const Ptr<const Packet>& > m_interestsTrace;
    TracedCallback<const Ptr<CcnxFace>&,const Ptr<const Packet>& > m_contentObjectsTrace;
};
}
#endif
