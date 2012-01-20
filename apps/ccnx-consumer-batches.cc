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

#include "ccnx-consumer-batches.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/batches.h"

NS_LOG_COMPONENT_DEFINE ("CcnxConsumerBatches");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (CcnxConsumerBatches);
    
TypeId
CcnxConsumerBatches::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxConsumerBatches")
    .SetParent<CcnxConsumer> ()
    .AddConstructor<CcnxConsumerBatches> ()

    .AddAttribute ("Batches", "Batches to schedule. Should be vector, containing pairs of time and amount",
                   // TypeId::ATTR_SET, 
                   StringValue (""),
                   MakeBatchesAccessor (&CcnxConsumerBatches::GetBatch, &CcnxConsumerBatches::SetBatch),
                   MakeBatchesChecker ())
    ;

  return tid;
}

CcnxConsumerBatches::CcnxConsumerBatches ()
{
}

void
CcnxConsumerBatches::SetBatch (const Batches &batches)
{
  // std::cout << "Batches: " << batches << "\n";
  for (Batches::const_iterator i = batches.begin (); i != batches.end (); i++)
    {
      Simulator::Schedule (i->get<0> (), &CcnxConsumerBatches::AddBatch, this, i->get<1> ());
    }
}

void
CcnxConsumerBatches::AddBatch (uint32_t amount)
{
  // std::cout << Simulator::Now () << " adding batch of " << amount << "\n";
  m_seqMax += amount;
  m_rtt->ClearSent (); // this is important, otherwise RTT estimation for the new batch will be affected by previous batch history
  ScheduleNextPacket ();
}

void
CcnxConsumerBatches::ScheduleNextPacket ()
{
  if (!m_sendEvent.IsRunning ())
    m_sendEvent = Simulator::Schedule (Seconds(0.00001), &CcnxConsumer::SendPacket, this);
}

///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////

} // namespace ns3
