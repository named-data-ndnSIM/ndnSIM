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

#ifndef NDNSIM_FLOODING_H
#define NDNSIM_FLOODING_H

#include "nacks.h"
#include "ns3/log.h"

namespace ns3 {
namespace ndn {
namespace fw {

/**
 * @ingroup ndn-fw
 * @brief Simple flooding strategy
 *
 * Interests will be forwarded to all available faces available for a route (FIB entry).
 * If there are no available GREEN or YELLOW faces, interests is dropped.
 *
 * Usage example:
 * @code
 *     ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::Flooding");
 *     ...
 *     ndnHelper.Install (nodes);
 * @endcode
 */
class Flooding :
    public Nacks
{
private:
  typedef Nacks super;

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
  Flooding ();

protected:
  // inherited from  Nacks/ForwardingStrategy
  virtual bool
  DoPropagateInterest (Ptr<Face> inFace,
                       Ptr<const Interest> interest,
                       Ptr<pit::Entry> pitEntry);

protected:
  static LogComponent g_log;
};

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_FLOODING
