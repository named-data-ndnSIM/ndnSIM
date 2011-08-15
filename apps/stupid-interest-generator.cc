/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
//  ndn_stupidinterestgenerator.cpp
//  Abstraction
//
//  Created by Ilya Moiseenko on 05.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "stupid-interest-generator.h"

#include "ns3/socket-factory.h"
#include "ns3/simulator.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("StupidInterestGenerator");

namespace ns3
{
    
NS_OBJECT_ENSURE_REGISTERED (StupidInterestGenerator);
    
TypeId
StupidInterestGenerator::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::StupidInterestGenerator")
        .SetParent<Application> ()
        .AddConstructor<StupidInterestGenerator> ()
        .AddAttribute ("Remote", "The address of the destination",
                       AddressValue (),
                       MakeAddressAccessor (&StupidInterestGenerator::m_peer),
                       MakeAddressChecker ())
        .AddAttribute ("OffTime", "Time interval between packets",
                       TimeValue (Seconds (0.1)),
                       MakeTimeAccessor (&StupidInterestGenerator::m_offTime),
                       MakeTimeChecker ())
        .AddAttribute ("Protocol", "The type of protocol to use.",
                       TypeIdValue (UdpSocketFactory::GetTypeId ()),
                       MakeTypeIdAccessor (&StupidInterestGenerator::m_tid),
                       MakeTypeIdChecker ())
        ;
	return tid;
}

StupidInterestGenerator::StupidInterestGenerator ()
{
	NS_LOG_FUNCTION_NOARGS ();
	m_socket = 0;
	//m_connected = false;
	//m_residualBits = 0;
	//m_lastStartTime = Seconds (0);
	//m_totBytes = 0;
}
    
StupidInterestGenerator::~StupidInterestGenerator()
{
	NS_LOG_FUNCTION_NOARGS ();
}
    
void
StupidInterestGenerator::DoDispose (void)
{
	NS_LOG_FUNCTION_NOARGS ();
        
	m_socket = 0;
	// chain up
	Application::DoDispose ();
}
    
// Application Methods
void StupidInterestGenerator::StartApplication () // Called at time specified by Start
{
	NS_LOG_FUNCTION_NOARGS ();
        
	// Create the socket if not already
	if (!m_socket)
	{
		m_socket = Socket::CreateSocket (GetNode (), m_tid);
		m_socket->Bind ();
		m_socket->Connect (m_peer);
		m_socket->SetAllowBroadcast (true);
		m_socket->ShutdownRecv ();
	}
	// Insure no pending event
	CancelEvents ();
	// If we are not yet connected, there is nothing to do here
	// The ConnectionComplete upcall will start timers at that time
	//if (!m_connected) return;
	ScheduleStartEvent ();
}
    
void StupidInterestGenerator::StopApplication () // Called at time specified by Stop
{
	NS_LOG_FUNCTION_NOARGS ();
        
	CancelEvents ();
	if(m_socket != 0)
	{
		m_socket->Close ();
	}
	else
	{
		NS_LOG_WARN ("OnOffApplication found null socket to close in StopApplication");
	}
}
    
void StupidInterestGenerator::CancelEvents ()
{
	NS_LOG_FUNCTION_NOARGS ();
        
	Simulator::Cancel (m_sendEvent);
	Simulator::Cancel (m_startStopEvent);
}

void StupidInterestGenerator::ScheduleStartEvent ()
{  // Schedules the event to start sending data (switch to the "On" state)
	NS_LOG_FUNCTION_NOARGS ();
        
	Time offInterval = Seconds (m_offTime);
	NS_LOG_LOGIC ("start at " << offInterval);
	m_startStopEvent = Simulator::Schedule (offInterval, &StupidInterestGenerator::StartSending, this);
}
    
// Event handlers
void StupidInterestGenerator::StartSending ()
{
	NS_LOG_FUNCTION_NOARGS ();
	//m_lastStartTime = Simulator::Now ();
	ScheduleNextTx ();  // Schedule the send packet event
	//ScheduleStopEvent ();
}
    
void StupidInterestGenerator::StopSending ()
{
	NS_LOG_FUNCTION_NOARGS ();
	CancelEvents ();
        
	ScheduleStartEvent ();
}
    
// Private helpers
void StupidInterestGenerator::ScheduleNextTx ()
{
	NS_LOG_FUNCTION_NOARGS ();
        

	Time nextTime = Seconds(0.); //send now
	m_sendEvent = Simulator::Schedule (nextTime,
									   &StupidInterestGenerator::SendPacket, this);
}

        
void StupidInterestGenerator::SendPacket ()
{
	// NS_LOG_FUNCTION_NOARGS ();
	// NS_LOG_LOGIC ("sending packet at " << Simulator::Now ());
	// NS_ASSERT (m_sendEvent.IsExpired ());
        
	// NameBuilder name;
	// name("prefix1")("prefix2")("filename");
	CcnxInterestHeader ();

	CcnxContentObjectHeader ();
		
	// const ccn_charbuf *output = name.GetName();
	// Ptr<InterestPacket> packet = Create<InterestPacket>(name,(uint32_t)output->length);
	// packet->AddTimeout(4000);
	// UniformVariable var;
	// packet->AddNonce(var.GetInteger(1,10000));
	// m_socket->Send(packet);
        
	// ScheduleStartEvent();
}

}
