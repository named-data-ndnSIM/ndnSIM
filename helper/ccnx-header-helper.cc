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

#include "ccnx-header-helper.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/object.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("CcnxHeaderHelper");


namespace ns3
{

CcnxHeaderHelper::Type
CcnxHeaderHelper::GetCcnxHeaderType (Ptr<const Packet> packet)
{
  uint8_t type[2];
  uint32_t read=packet->CopyData (type,2);

  if (read!=2) throw CcnxUnknownHeaderException();

  NS_LOG_DEBUG (*packet);
  if (type[0] == INTEREST_BYTE0 && type[1] == INTEREST_BYTE1)
    {
      return CcnxHeaderHelper::INTEREST;
    }
  else if (type[0] == CONTENT_OBJECT_BYTE0 && type[1] == CONTENT_OBJECT_BYTE1)
    {
      return CcnxHeaderHelper::CONTENT_OBJECT;
    }

  NS_LOG_DEBUG (*packet);
  throw CcnxUnknownHeaderException();
}

} // namespace ns3
