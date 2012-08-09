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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndn-consumer-window.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

NS_LOG_COMPONENT_DEFINE ("NdnConsumerWindow");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (NdnConsumerWindow);
    
TypeId
NdnConsumerWindow::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdnConsumerWindow")
    .SetGroupName ("Ndn")
    .SetParent<NdnConsumer> ()
    .AddConstructor<NdnConsumerWindow> ()

    .AddAttribute ("Window", "Initial size of the window",
                   StringValue ("1"),
                   MakeUintegerAccessor (&NdnConsumerWindow::GetWindow, &NdnConsumerWindow::SetWindow),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("PayloadSize", "Average size of content object size (to calculate interest generation rate)",
                   UintegerValue (1040),
                   MakeUintegerAccessor (&NdnConsumerWindow::GetPayloadSize, &NdnConsumerWindow::SetPayloadSize),
                   MakeUintegerChecker<uint32_t>())
    .AddAttribute ("Size", "Amount of data in megabytes to request (relies on PayloadSize parameter)",
                   DoubleValue (-1), // don't impose limit by default
                   MakeDoubleAccessor (&NdnConsumerWindow::GetMaxSize, &NdnConsumerWindow::SetMaxSize),
                   MakeDoubleChecker<double> ())

    .AddTraceSource ("WindowTrace",
                     "Window that controls how many outstanding interests are allowed",
                     MakeTraceSourceAccessor (&NdnConsumerWindow::m_window))
    .AddTraceSource ("InFlight",
                     "Current number of outstanding interests",
                     MakeTraceSourceAccessor (&NdnConsumerWindow::m_window))
    ;

  return tid;
}

NdnConsumerWindow::NdnConsumerWindow ()
  : m_payloadSize (1040)
  , m_inFlight (0)
{
}

void
NdnConsumerWindow::SetWindow (uint32_t window)
{
  m_window = window;
}

uint32_t
NdnConsumerWindow::GetWindow () const
{
  return m_window;
}

uint32_t
NdnConsumerWindow::GetPayloadSize () const
{
  return m_payloadSize;
}

void
NdnConsumerWindow::SetPayloadSize (uint32_t payload)
{
  m_payloadSize = payload;
}

double
NdnConsumerWindow::GetMaxSize () const
{
  if (m_seqMax == 0)
    return -1.0;

  return m_maxSize;
}

void
NdnConsumerWindow::SetMaxSize (double size)
{
  m_maxSize = size;
  if (m_maxSize < 0)
    {
      m_seqMax = 0;
      return;
    }

  m_seqMax = floor(1.0 + m_maxSize * 1024.0 * 1024.0 / m_payloadSize);
  NS_LOG_DEBUG ("MaxSeqNo: " << m_seqMax);
  // std::cout << "MaxSeqNo: " << m_seqMax << "\n";
}


void
NdnConsumerWindow::ScheduleNextPacket ()
{
  if (m_window == static_cast<uint32_t> (0) || m_inFlight >= m_window)
    {
      if (!m_sendEvent.IsRunning ())
        m_sendEvent = Simulator::Schedule (Seconds (m_rtt->RetransmitTimeout ().ToDouble (Time::S) * 0.1), &NdnConsumer::SendPacket, this);
      return;
    }
  
  // std::cout << "Window: " << m_window << ", InFlight: " << m_inFlight << "\n";
  if (!m_sendEvent.IsRunning ())
    {
      m_inFlight++;
      m_sendEvent = Simulator::ScheduleNow (&NdnConsumer::SendPacket, this);
    }
}

///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////

void
NdnConsumerWindow::OnContentObject (const Ptr<const NdnContentObjectHeader> &contentObject,
                                     Ptr<Packet> payload)
{
  NdnConsumer::OnContentObject (contentObject, payload);

  m_window = m_window + 1;

  if (m_inFlight > static_cast<uint32_t> (0)) m_inFlight--;
  ScheduleNextPacket ();
}

void
NdnConsumerWindow::OnNack (const Ptr<const NdnInterestHeader> &interest, Ptr<Packet> payload)
{
  NdnConsumer::OnNack (interest, payload);
  if (m_inFlight > static_cast<uint32_t> (0)) m_inFlight--;

  if (m_window > static_cast<uint32_t> (0))
    {
      // m_window = 0.5 * m_window;//m_window - 1;
      m_window = m_window - 1;
    }
}

void
NdnConsumerWindow::OnTimeout (uint32_t sequenceNumber)
{
  if (m_inFlight > static_cast<uint32_t> (0)) m_inFlight--;
  NdnConsumer::OnTimeout (sequenceNumber);
}

} // namespace ns3
