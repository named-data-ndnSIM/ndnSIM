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
 */
#ifndef NDNSIM_NACKS_H
#define NDNSIM_NACKS_H

#include "ns3/ndn-forwarding-strategy.h"

namespace ns3 {

namespace ndnSIM {

/**
 * \ingroup ndn
 * \brief Abstract base class for Ndn forwarding strategies
 */
class Nacks :
    public NdnForwardingStrategy
{
public:
  static TypeId
  GetTypeId (void);

  /**
   * \brief Actual processing of incoming Ndn interests. Note, interests do not have payload
   * 
   * Processing Interest packets
   * @param face    incoming face
   * @param header  deserialized Interest header
   * @param packet  original packet
   */
  virtual void
  OnInterest (const Ptr<NdnFace> &face,
              Ptr<NdnInterestHeader> &header,
              const Ptr<const Packet> &p);

protected:
  // using NdnForwardingStrategy::PropagateInterest; // some strange c++ cheating

  virtual void
  DidReceiveDuplicateInterest (const Ptr<NdnFace> &face,
                               Ptr<NdnInterestHeader> &header,
                               const Ptr<const Packet> &packet,
                               Ptr<NdnPitEntry> pitEntry);
  
  virtual void
  DidExhaustForwardingOptions (const Ptr<NdnFace> &incomingFace,
                               Ptr<NdnInterestHeader> header,
                               const Ptr<const Packet> &packet,
                               Ptr<NdnPitEntry> pitEntry);

  virtual void
  OnNack (const Ptr<NdnFace> &face,
          Ptr<NdnInterestHeader> &header,
          const Ptr<const Packet> &p);

  virtual void
  DidReceiveValidNack (const Ptr<NdnFace> &incomingFace,
                       uint32_t nackCode,
                       Ptr<NdnPitEntry> pitEntry);
  
protected:  
  bool m_nacksEnabled;

  TracedCallback<Ptr<const NdnInterestHeader>,
                 Ptr<const NdnFace> > m_outNacks; ///< @brief trace of outgoing NACKs

  TracedCallback<Ptr<const NdnInterestHeader>,
                 Ptr<const NdnFace> > m_inNacks; ///< @brief trace of incoming NACKs

  TracedCallback<Ptr<const NdnInterestHeader>,
                 Ptr<const NdnFace> > m_dropNacks; ///< @brief trace of dropped NACKs

private:
  typedef NdnForwardingStrategy super;
};

} // namespace ndnSIM
} // namespace ns3

#endif // NDNSIM_NACKS
