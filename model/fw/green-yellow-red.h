/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 */
#ifndef NDNSIM_GREEN_YELLOW_RED_H
#define NDNSIM_GREEN_YELLOW_RED_H

#include "nacks.h"

namespace ns3 {
namespace ndn {
namespace fw {

/**
 * \ingroup ndn
 */
class GreenYellowRed :
    public Nacks
{
public:
  static TypeId
  GetTypeId (void);

protected:
  virtual void
  WillSatisfyPendingInterest (Ptr<Face> inFace,
                              Ptr<pit::Entry> pitEntry);

  virtual bool
  DoPropagateInterest (Ptr<Face> inFace,
                       Ptr<const InterestHeader> header,
                       Ptr<const Packet> origPacket,
                       Ptr<pit::Entry> pitEntry);
  virtual void
  DidReceiveValidNack (Ptr<Face> incomingFace,
                       uint32_t nackCode,
                       Ptr<const InterestHeader> header,
                       Ptr<const Packet> origPacket,
                       Ptr<pit::Entry> pitEntry);

private:
  typedef Nacks super;
};

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_GREEN_YELLOW_RED
