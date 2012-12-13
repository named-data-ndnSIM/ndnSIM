/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

// custom-app.cc

#include "custom-app.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndn-app-face.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"

#include "ns3/ndn-fib.h"
#include "ns3/random-variable.h"

NS_LOG_COMPONENT_DEFINE ("CustomApp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CustomApp);

// register NS-3 type
TypeId
CustomApp::GetTypeId ()
{
  static TypeId tid = TypeId ("CustomApp")
    .SetParent<ndn::App> ()
    .AddConstructor<CustomApp> ()
    ;
  return tid;
}

// Processing upon start of the application
void
CustomApp::StartApplication ()
{
  // initialize ndn::App
  ndn::App::StartApplication ();

  // Create a name components object for name ``/prefix/sub``
  Ptr<ndn::NameComponents> prefix = Create<ndn::NameComponents> (); // now prefix contains ``/``
  prefix->Add ("prefix"); // now prefix contains ``/prefix``
  prefix->Add ("sub"); // now prefix contains ``/prefix/sub``

  /////////////////////////////////////////////////////////////////////////////
  // Creating FIB entry that ensures that we will receive incoming Interests //
  /////////////////////////////////////////////////////////////////////////////

  // Get FIB object
  Ptr<ndn::Fib> fib = GetNode ()->GetObject<ndn::Fib> ();

  // Add entry to FIB
  // Note that ``m_face`` is cretaed by ndn::App
  Ptr<ndn::fib::Entry> fibEntry = fib->Add (*prefix, m_face, 0);

  Simulator::Schedule (Seconds (1.0), &CustomApp::SendInterest, this);
}

// Processing when application is stopped
void
CustomApp::StopApplication ()
{
  // cleanup ndn::App
  ndn::App::StopApplication ();
}

void
CustomApp::SendInterest ()
{
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////
  
  Ptr<ndn::NameComponents> prefix = Create<ndn::NameComponents> ("/prefix/sub"); // another way to create name

  // Create and configure ndn::InterestHeader
  ndn::InterestHeader interestHeader;
  UniformVariable rand (0,std::numeric_limits<uint32_t>::max ());
  interestHeader.SetNonce            (rand.GetValue ());
  interestHeader.SetName             (prefix);
  interestHeader.SetInterestLifetime (Seconds (1.0));

  // Create packet and add ndn::InterestHeader
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (interestHeader);

  NS_LOG_DEBUG ("Sending Interest packet for " << *prefix);
  
  // Forward packet to lower (network) layer
  m_protocolHandler (packet);

  // Call trace (for logging purposes)
  m_transmittedInterests (&interestHeader, this, m_face);
}

// Callback that will be called when Interest arrives
void
CustomApp::OnInterest (const Ptr<const ndn::InterestHeader> &interest, Ptr<Packet> origPacket)
{
  NS_LOG_DEBUG ("Received Interest packet for " << interest->GetName ());

  // Create and configure ndn::ContentObjectHeader and ndn::ContentObjectTail
  // (header is added in front of the packet, tail is added at the end of the packet)

  // Note that Interests send out by the app will not be sent back to the app !
  
  ndn::ContentObjectHeader data;
  data.SetName (Create<ndn::NameComponents> (interest->GetName ())); // data will have the same name as Interests

  ndn::ContentObjectTail trailer; // doesn't require any configuration

  // Create packet and add header and trailer
  Ptr<Packet> packet = Create<Packet> (1024);
  packet->AddHeader (data);
  packet->AddTrailer (trailer);

  NS_LOG_DEBUG ("Sending ContentObject packet for " << data.GetName ());

  // Forward packet to lower (network) layer
  m_protocolHandler (packet);

  // Call trace (for logging purposes)
  m_transmittedContentObjects (&data, packet, this, m_face);
}

// Callback that will be called when Data arrives
void
CustomApp::OnContentObject (const Ptr<const ndn::ContentObjectHeader> &contentObject,
                            Ptr<Packet> payload)
{
  NS_LOG_DEBUG ("Receiving ContentObject packet for " << contentObject->GetName ());

  std::cout << "DATA received for name " << contentObject->GetName () << std::endl;
}


} // namespace ns3
