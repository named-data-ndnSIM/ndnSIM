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

#include "ndn-header-helper.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/object.h"

#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"
#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("ndn.HeaderHelper");

const uint8_t INTEREST_CCNB_BYTES[]       = {0x01, 0xD2};
const uint8_t CONTENT_OBJECT_CCNB_BYTES[] = {0x04, 0x82};

const uint8_t INTEREST_NDNSIM_BYTES[]       = {0x80, 0x00};
const uint8_t CONTENT_OBJECT_NDNSIM_BYTES[] = {0x80, 0x01};

namespace ns3 {
namespace ndn {

HeaderHelper::Type
HeaderHelper::GetNdnHeaderType (Ptr<const Packet> packet)
{
  uint8_t type[2];
  uint32_t read=packet->CopyData (type,2);

  if (read!=2) throw UnknownHeaderException();

  NS_LOG_DEBUG (*packet);
  if (type[0] == INTEREST_CCNB_BYTES[0] && type[1] == INTEREST_CCNB_BYTES[1])
    {
      return HeaderHelper::INTEREST_CCNB;
    }
  else if (type[0] == CONTENT_OBJECT_CCNB_BYTES[0] && type[1] == CONTENT_OBJECT_CCNB_BYTES[1])
    {
      return HeaderHelper::CONTENT_OBJECT_CCNB;
    }
  else if (type[0] == INTEREST_NDNSIM_BYTES[0] && type[1] == INTEREST_NDNSIM_BYTES[1])
    {
      return HeaderHelper::INTEREST_NDNSIM;
    }
  else if (type[0] == CONTENT_OBJECT_NDNSIM_BYTES[0] && type[1] == CONTENT_OBJECT_NDNSIM_BYTES[1])
    {
      return HeaderHelper::CONTENT_OBJECT_NDNSIM;
    }

  NS_LOG_DEBUG (*packet);
  throw UnknownHeaderException();
}

Ptr<const Name>
HeaderHelper::GetName (Ptr<const Packet> p)
{
  Ptr<Packet> packet = p->Copy (); // give upper layers a rw copy of the packet
  try
    {
      HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (p);
      switch (type)
        {
        case HeaderHelper::INTEREST_NDNSIM:
          {
            Ptr<InterestHeader> header = Create<InterestHeader> ();

            // Deserialization. Exception may be thrown
            packet->RemoveHeader (*header);
            NS_ASSERT_MSG (packet->GetSize () == 0, "Payload of Interests should be zero");

            return header->GetNamePtr ();
            break;
          }
        case HeaderHelper::CONTENT_OBJECT_NDNSIM:
          {
            Ptr<ContentObjectHeader> header = Create<ContentObjectHeader> ();

            // Deserialization. Exception may be thrown
            packet->RemoveHeader (*header);
            return header->GetNamePtr ();
            break;
          }
        case HeaderHelper::INTEREST_CCNB:
        case HeaderHelper::CONTENT_OBJECT_CCNB:
          NS_FATAL_ERROR ("ccnb support is broken in this implementation");
          break;
        }

      // exception will be thrown if packet is not recognized
    }
  catch (UnknownHeaderException)
    {
      return 0;
    }

  return 0;
}


} // namespace ndn
} // namespace ns3
