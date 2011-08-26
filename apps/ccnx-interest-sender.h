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

namespace ns3 
{
    
class Socket;
    
class CcnxInterestSender: public Application
{
public: 
    static TypeId GetTypeId (void);
        
    CcnxInterestSender ();
        
    virtual ~CcnxInterestSender ();
        
protected:
    virtual void DoDispose (void);
private:
    // inherited from Application base class.
    virtual void StartApplication (void);    // Called at time specified by Start
    virtual void StopApplication (void);     // Called at time specified by Stop
        
    //Time m_onTime;
    Time m_offTime;
    CcnxNameComponents m_interestName;
  
    Time m_interestLifeTime;
    int32_t m_minSuffixComponents;
    int32_t m_maxSuffixComponents;
    bool m_childSelector;
    CcnxNameComponents m_exclude;
    uint32_t m_initialNonce;
    
    //EventId         m_startStopEvent;     // Event id for next start or stop event
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
        
private:
    void ScheduleNextTx ();
    //void ScheduleStartEvent ();
    //void ScheduleStopEvent ();
    void ConnectionSucceeded (Ptr<Socket>);
    void ConnectionFailed (Ptr<Socket>);
    void Ignore (Ptr<Socket>);
        
};
}
