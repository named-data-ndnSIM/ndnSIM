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

#include "ccnx-consumer-window.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

NS_LOG_COMPONENT_DEFINE ("CcnxConsumerWindow");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (CcnxConsumerWindow);
    
TypeId
CcnxConsumerWindow::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxConsumerWindow")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxConsumer> ()
    .AddConstructor<CcnxConsumerWindow> ()

    .AddAttribute ("Window", "Initial size of the window",
                   StringValue ("1"),
                   MakeUintegerAccessor (&CcnxConsumerWindow::GetWindow, &CcnxConsumerWindow::SetWindow),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("PayloadSize", "Average size of content object size (to calculate interest generation rate)",
                   UintegerValue (1040),
                   MakeUintegerAccessor (&CcnxConsumerWindow::GetPayloadSize, &CcnxConsumerWindow::SetPayloadSize),
                   MakeUintegerChecker<uint32_t>())
    .AddAttribute ("Size", "Amount of data in megabytes to request (relies on PayloadSize parameter)",
                   DoubleValue (-1), // don't impose limit by default
                   MakeDoubleAccessor (&CcnxConsumerWindow::GetMaxSize, &CcnxConsumerWindow::SetMaxSize),
                   MakeDoubleChecker<double> ())

    .AddTraceSource ("WindowTrace",
                     "Window that controls how many outstanding interests are allowed",
                     MakeTraceSourceAccessor (&CcnxConsumerWindow::m_window))
    .AddTraceSource ("InFlight",
                     "Current number of outstanding interests",
                     MakeTraceSourceAccessor (&CcnxConsumerWindow::m_window))
    ;

  return tid;
}

CcnxConsumerWindow::CcnxConsumerWindow ()
  : m_payloadSize (1040)
  , m_inFlight (0)
{
}

void
CcnxConsumerWindow::SetWindow (uint32_t window)
{
  m_window = window;
}

uint32_t
CcnxConsumerWindow::GetWindow () const
{
  return m_window;
}

uint32_t
CcnxConsumerWindow::GetPayloadSize () const
{
  return m_payloadSize;
}

void
CcnxConsumerWindow::SetPayloadSize (uint32_t payload)
{
  m_payloadSize = payload;
}

double
CcnxConsumerWindow::GetMaxSize () const
{
  if (m_seqMax == 0)
    return -1.0;

  return m_maxSize;
}

void
CcnxConsumerWindow::SetMaxSize (double size)
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
CcnxConsumerWindow::ScheduleNextPacket ()
{
  if (m_window == static_cast<uint32_t> (0) || m_inFlight >= m_window)
    {
      if (!m_sendEvent.IsRunning ())
        m_sendEvent = Simulator::Schedule (Seconds (m_rtt->RetransmitTimeout ().ToDouble (Time::S) * 0.1), &CcnxConsumer::SendPacket, this);
      return;
    }
  
  // std::cout << "Window: " << m_window << ", InFlight: " << m_inFlight << "\n";
  if (!m_sendEvent.IsRunning ())
    {
      m_inFlight++;
      m_sendEvent = Simulator::ScheduleNow (&CcnxConsumer::SendPacket, this);
    }
}

///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////

void
CcnxConsumerWindow::OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                                     Ptr<Packet> payload)
{
  CcnxConsumer::OnContentObject (contentObject, payload);

  m_window = m_window + 1;

  if (m_inFlight > static_cast<uint32_t> (0)) m_inFlight--;
  ScheduleNextPacket ();
}

void
CcnxConsumerWindow::OnNack (const Ptr<const CcnxInterestHeader> &interest, Ptr<Packet> payload)
{
  CcnxConsumer::OnNack (interest, payload);
  if (m_inFlight > static_cast<uint32_t> (0)) m_inFlight--;

  if (m_window > static_cast<uint32_t> (0))
    {
      // m_window = 0.5 * m_window;//m_window - 1;
      m_window = m_window - 1;
    }
}

void
CcnxConsumerWindow::OnTimeout (uint32_t sequenceNumber)
{
  if (m_inFlight > static_cast<uint32_t> (0)) m_inFlight--;
  CcnxConsumer::OnTimeout (sequenceNumber);
}

} // namespace ns3
