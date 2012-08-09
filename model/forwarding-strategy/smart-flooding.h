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

#ifndef NDNSIM_SMART_FLOODING_H
#define NDNSIM_SMART_FLOODING_H

#include "green-yellow-red.h"

namespace ns3 {
namespace ndnSIM {

/**
 * \ingroup ccnx
 */
class SmartFlooding :
    public GreenYellowRed
{
public:
  static TypeId GetTypeId (void);

  /**
   * @brief Default constructor
   */
  SmartFlooding ();

  // inherited
  virtual bool
  DoPropagateInterest (const Ptr<NdnFace> &incomingFace,
                       Ptr<NdnInterestHeader> header,
                       const Ptr<const Packet> &packet,
                       Ptr<NdnPitEntry> pitEntry);

private:
  typedef GreenYellowRed super;
};

} // namespace ndnSIM
} // namespace ns3

#endif // NDNSIM_SMART_FLOODING_H
