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
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */


#ifndef NDNSIM_BEST_ROUTE_H
#define NDNSIM_BEST_ROUTE_H

#include "green-yellow-red.h"
#include "ns3/log.h"

namespace ns3 {
namespace ndn {
namespace fw {

/**
 * @ingroup ndn-fw
 * @brief Best route strategy
 */
class BestRoute :
    public GreenYellowRed
{
private:
  typedef GreenYellowRed super;

public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Helper function to retrieve logging name for the forwarding strategy
   */
  static std::string
  GetLogName ();
  
  /**
   * @brief Default constructor
   */
  BestRoute ();
        
  // from super
  virtual bool
  DoPropagateInterest (Ptr<Face> incomingFace,
                       Ptr<const Interest> interest,
                       Ptr<pit::Entry> pitEntry);
protected:
  static LogComponent g_log;
};

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_BEST_ROUTE_H
