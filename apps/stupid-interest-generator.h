//
//  ndn_stupidinterestgenerator.h
//  Abstraction
//
//  Created by Ilya Moiseenko on 05.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ns3/application.h"
#include "ns3/log.h"
#include "ns3/address.h"
#include "ns3/random-variable.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include <ccn/ccn.h>
#include "ns3/udp-socket-factory.h"

namespace ns3 
{

    
//namespace NDNabstraction
//{
    class Socket; //dynamic linking works in a somehow strange way
    
    class StupidInterestGenerator: public Application
    {
    public: 
        static TypeId GetTypeId (void);
        
        StupidInterestGenerator ();
        
        virtual ~StupidInterestGenerator();
        
                
    protected:
        virtual void DoDispose (void);
    private:
        // inherited from Application base class.
        virtual void StartApplication (void);    // Called at time specified by Start
        virtual void StopApplication (void);     // Called at time specified by Stop
        
        //Time m_onTime;
        Time m_offTime;
        
        Address         m_peer;         // Peer address
        Ptr<Socket>     m_socket;
        EventId         m_startStopEvent;     // Event id for next start or stop event
        EventId         m_sendEvent;    // Eventid of pending "send packet" event
        TypeId          m_tid;
        
        //helpers
        void CancelEvents ();
        
        void Construct (Ptr<Node> n,
                        const Address &remote,
                        std::string tid,
                        const RandomVariable& ontime,
                        const RandomVariable& offtime,
                        uint32_t size);
        
        // Event handlers
        void StartSending ();
        void StopSending ();
        void SendPacket ();
        
    private:
        void ScheduleNextTx ();
        void ScheduleStartEvent ();
        void ScheduleStopEvent ();
        void ConnectionSucceeded (Ptr<Socket>);
        void ConnectionFailed (Ptr<Socket>);
        void Ignore (Ptr<Socket>);

    };
//}
}
