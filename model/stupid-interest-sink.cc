//
//  stupid-interest-sink.cpp
//  Abstraction
//
//  Created by Ilya Moiseenko on 10.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "stupid-interest-sink.h"
#include "ns3/address.h"
#include "ns3/address-utils.h"
#include "ns3/log.h"
#include "ns3/inet-socket-address.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"

using namespace std;

namespace ns3 {
    
    NS_LOG_COMPONENT_DEFINE ("StupidInterestSink");
    NS_OBJECT_ENSURE_REGISTERED (StupidInterestSink);
    
    TypeId 
    StupidInterestSink::GetTypeId (void)
    {
        static TypeId tid = TypeId ("ns3::StupidInterestSink")
        .SetParent<Application> ()
        .AddConstructor<StupidInterestSink> ()
        .AddAttribute ("Local", "The Address on which to Bind the rx socket.",
                       AddressValue (),
                       MakeAddressAccessor (&StupidInterestSink::m_local),
                       MakeAddressChecker ())
        .AddAttribute ("Protocol", "The type id of the protocol to use for the rx socket.",
                       TypeIdValue (UdpSocketFactory::GetTypeId ()),
                       MakeTypeIdAccessor (&StupidInterestSink::m_tid),
                       MakeTypeIdChecker ())
        .AddTraceSource ("Rx", "A packet has been received",
                         MakeTraceSourceAccessor (&StupidInterestSink::m_rxTrace))
        ;
        return tid;
    }
    
    StupidInterestSink::StupidInterestSink ()
    {
        NS_LOG_FUNCTION (this);
        m_socket = 0;
        m_totalRx = 0;
    }
    
    StupidInterestSink::~StupidInterestSink()
    {
        NS_LOG_FUNCTION (this);
    }
    
    uint32_t StupidInterestSink::GetTotalRx () const
    {
        return m_totalRx;
    }
    
    Ptr<Socket>
    StupidInterestSink::GetListeningSocket (void) const
    {
        NS_LOG_FUNCTION (this);
        return m_socket;
    }
    
    std::list<Ptr<Socket> >
    StupidInterestSink::GetAcceptedSockets (void) const
    {
        NS_LOG_FUNCTION (this);
        return m_socketList;
    }
    
    void StupidInterestSink::DoDispose (void)
    {
        NS_LOG_FUNCTION (this);
        m_socket = 0;
        m_socketList.clear ();
        
        // chain up
        Application::DoDispose ();
    }
    
    
    // Application Methods
    void StupidInterestSink::StartApplication ()    // Called at time specified by Start
    {
        NS_LOG_FUNCTION (this);
        // Create the socket if not already
        if (!m_socket)
        {
            m_socket = Socket::CreateSocket (GetNode (), m_tid);
            m_socket->Bind (m_local);
            m_socket->Listen ();
            m_socket->ShutdownSend ();
            if (addressUtils::IsMulticast (m_local))
            {
                Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket);
                if (udpSocket)
                {
                    // equivalent to setsockopt (MCAST_JOIN_GROUP)
                    udpSocket->MulticastJoinGroup (0, m_local);
                }
                else
                {
                    NS_FATAL_ERROR ("Error: joining multicast on a non-UDP socket");
                }
            }
        }
        
        m_socket->SetRecvCallback (MakeCallback (&StupidInterestSink::HandleRead, this));
        m_socket->SetAcceptCallback (
                                     MakeNullCallback<bool, Ptr<Socket>, const Address &> (),
                                     MakeCallback (&StupidInterestSink::HandleAccept, this));
        m_socket->SetCloseCallbacks (
                                     MakeCallback (&StupidInterestSink::HandlePeerClose, this),
                                     MakeCallback (&StupidInterestSink::HandlePeerError, this));
    }
    
    void StupidInterestSink::StopApplication ()     // Called at time specified by Stop
    {
        NS_LOG_FUNCTION (this);
        while(!m_socketList.empty ()) //these are accepted sockets, close them
        {
            Ptr<Socket> acceptedSocket = m_socketList.front ();
            m_socketList.pop_front ();
            acceptedSocket->Close ();
        }
        if (m_socket) 
        {
            m_socket->Close ();
            m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
        }
    }
    
    void StupidInterestSink::HandleRead (Ptr<Socket> socket)
    {
        NS_LOG_FUNCTION (this << socket);
        Ptr<Packet> packet;
        Address from;
        while (packet = socket->RecvFrom (from))
        {
            if (packet->GetSize () == 0)
            { //EOF
                break;
            }
            if (InetSocketAddress::IsMatchingType (from))
            {
                m_totalRx += packet->GetSize ();
                InetSocketAddress address = InetSocketAddress::ConvertFrom (from);
                NS_LOG_INFO ("Received " << packet->GetSize () << " bytes from " <<
                             address.GetIpv4 () << " [" << address << "]"
                             << " total Rx " << m_totalRx);
                //cast address to void , to suppress 'address' set but not used 
                //compiler warning in optimized builds
                (void) address;
            }
            m_rxTrace (packet, from);
        }
    }
    
    void StupidInterestSink::HandlePeerClose (Ptr<Socket> socket)
    {
        NS_LOG_INFO ("PktSink, peerClose");
    }
    
    void StupidInterestSink::HandlePeerError (Ptr<Socket> socket)
    {
        NS_LOG_INFO ("PktSink, peerError");
    }
    
    
    void StupidInterestSink::HandleAccept (Ptr<Socket> s, const Address& from)
    {
        NS_LOG_FUNCTION (this << s << from);
        s->SetRecvCallback (MakeCallback (&StupidInterestSink::HandleRead, this));
        m_socketList.push_back (s);
    }
    
} // Namespace ns3
