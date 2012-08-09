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
namespace ndn {
namespace fw {

/**
 * \ingroup ndn
 * \brief Abstract base class for Ndn forwarding strategies
 */
class Nacks :
    public ForwardingStrategy
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief Actual processing of incoming Ndn interests. Note, interests do not have payload
   * 
   * Processing Interest packets
   * @param face    incoming face
   * @param header  deserialized Interest header
   * @param packet  original packet
   */
  virtual void
  OnInterest (const Ptr<Face> &face,
              Ptr<InterestHeader> &header,
              const Ptr<const Packet> &p);

protected:
  // using NdnForwardingStrategy::PropagateInterest; // some strange c++ cheating

  virtual void
  DidReceiveDuplicateInterest (const Ptr<Face> &face,
                               Ptr<InterestHeader> &header,
                               const Ptr<const Packet> &packet,
                               Ptr<pit::Entry> pitEntry);
  
  virtual void
  DidExhaustForwardingOptions (const Ptr<Face> &incomingFace,
                               Ptr<InterestHeader> header,
                               const Ptr<const Packet> &packet,
                               Ptr<pit::Entry> pitEntry);

  virtual void
  OnNack (const Ptr<Face> &face,
          Ptr<InterestHeader> &header,
          const Ptr<const Packet> &p);

  virtual void
  DidReceiveValidNack (const Ptr<Face> &incomingFace,
                       uint32_t nackCode,
                       Ptr<pit::Entry> pitEntry);
  
protected:  
  bool m_nacksEnabled;

  TracedCallback<Ptr<const InterestHeader>,
                 Ptr<const Face> > m_outNacks; ///< @brief trace of outgoing NACKs

  TracedCallback<Ptr<const InterestHeader>,
                 Ptr<const Face> > m_inNacks; ///< @brief trace of incoming NACKs

  TracedCallback<Ptr<const InterestHeader>,
                 Ptr<const Face> > m_dropNacks; ///< @brief trace of dropped NACKs

private:
  typedef ForwardingStrategy super;
};

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_NACKS
