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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef CCNX_FLOODING_STRATEGY_H
#define CCNX_FLOODING_STRATEGY_H

#include "ccnx-forwarding-strategy.h"

namespace ns3 
{
    
class CcnxFace;
class CcnxInterestHeader;
    
/**
 * \ingroup ccnx
 * \brief Flooding strategy
 *
 * \todo Describe
 */
class CcnxFloodingStrategy : public CcnxForwardingStrategy
{
public:
  static TypeId GetTypeId (void);

  /**
   * @brief Default constructor
   */
  CcnxFloodingStrategy ();

  // inherited from  CcnxForwardingStrategy
  virtual bool
  PropagateInterest (Ptr<CcnxPitEntry> pitEntry, 
                     const Ptr<CcnxFace> &incomingFace,
                     Ptr<CcnxInterestHeader> &header,
                     const Ptr<const Packet> &packet);

private:
  bool m_smartFlooding;
};
    
} //namespace ns3

#endif /* CCNX_FLOODING_STRATEGY_H */
