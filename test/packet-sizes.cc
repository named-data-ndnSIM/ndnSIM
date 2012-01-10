/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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

#include "ns3/core-module.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-header-helper.h"
#include "ns3/header.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/nstime.h"
#include "ns3/string.h"
#include "ns3/buffer.h"
#include "ns3/packet.h"
#include "ns3/log.h"

using namespace ns3;
#include <fstream>

NS_LOG_COMPONENT_DEFINE ("PacketSizes");

int
main (int argc, char *argv[])
{
  NS_LOG_INFO ("Test started");

  uint32_t size = 1024;
  std::string namePrefixStr = "/1";
  uint32_t start=0, end=100;

  CommandLine	cmd;
  cmd.AddValue ("size",  "ContentObject payload size", size);
  cmd.AddValue ("name",  "Prefix",                     namePrefixStr);
  cmd.AddValue ("start", "Range start", start);
  cmd.AddValue ("end",   "Range end",   end);
  cmd.Parse (argc, argv);
 
  CcnxNameComponents namePrefixValue;
  std::istringstream is (namePrefixStr);
  is >> namePrefixValue;

  Packet::EnablePrinting ();
  Packet::EnableChecking ();

  double interestSize = 0.0;
  double nackSize = 0.0;
  double contentObjectSize = 0.0;

  double progress = start;
  double step = (end-start)/100.0;

  progress += step;

  NS_LOG_INFO (progress << ", " << step);

  for (uint32_t currentSize = start; currentSize < end; currentSize++)
    {
      Ptr<CcnxNameComponents> namePrefix = Create<CcnxNameComponents> (namePrefixValue);
      namePrefix->Add (currentSize);

      NS_LOG_LOGIC (boost::cref (*namePrefix));

      // Interest Packet (doesn't have a payload)
      CcnxInterestHeader interestHeader;

      interestHeader.SetName (namePrefix);
      interestHeader.SetInterestLifetime (Seconds (4.0));
      interestHeader.SetNonce (10101010);
      
      Ptr<Packet> interestPacket =  Create<Packet> (0);
      interestPacket->AddHeader (interestHeader);

      interestSize = interestSize + (1.0*interestPacket->GetSize () - interestSize) / (currentSize - start + 1);

      

      // NACK
      interestHeader.SetNack (CcnxInterestHeader::NACK_GIVEUP_PIT);

      Ptr<Packet> nackPacket = Create<Packet> (0);
      nackPacket->AddHeader (interestHeader);
      
      nackSize = nackSize + (1.0*nackPacket->GetSize () - nackSize) / (currentSize - start + 1);

      // ContentObject
      CcnxContentObjectHeader coHeader;
      CcnxContentObjectTail   coTrailer;
      
      coHeader.SetName (namePrefix);
      
      Ptr<Packet> contentObject = Create<Packet> (size);

      contentObject->AddHeader (coHeader);
      contentObject->AddTrailer (coTrailer);

      contentObjectSize = contentObjectSize + (1.0*contentObject->GetSize () - contentObjectSize ) / (currentSize - start + 1);

      NS_LOG_DEBUG (interestSize << ", " << nackSize << ", " << contentObjectSize);

      if (currentSize >= progress) 
	{
	  NS_LOG_INFO ("Current: " << currentSize << "/" << end);
	  progress += step;
	}
    }

  NS_LOG_INFO ("Avg interest: " << interestSize << ", avg nack: " << nackSize << ", avg contentObject: " << contentObjectSize);

  return 0;
}
