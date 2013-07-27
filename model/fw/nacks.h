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
 * @ingroup ndn-fw
 * @brief Implementation of experimental NACK messages (enables with EnableNACKs option)
 */
class Nacks :
    public ForwardingStrategy
{
private:
  typedef ForwardingStrategy super;
  
public:
  static TypeId
  GetTypeId ();
  
  // from super
  virtual void
  OnInterest (Ptr<Face> face,
              Ptr<Interest> interest);

protected:
  // from super
  virtual void
  DidReceiveDuplicateInterest (Ptr<Face> inFace,
                               Ptr<const Interest> interest,
                               Ptr<pit::Entry> pitEntry);

  // from super
  virtual void
  DidExhaustForwardingOptions (Ptr<Face> inFace,
                               Ptr<const Interest> interest,
                               Ptr<pit::Entry> pitEntry);

  virtual void
  OnNack (Ptr<Face> inFace,
          Ptr<Interest> nack);

  virtual void
  DidReceiveValidNack (Ptr<Face> inFace,
                       uint32_t nackCode,
                       Ptr<const Interest> nack,
                       Ptr<pit::Entry> pitEntry);
  
protected:  
  bool m_nacksEnabled;

  TracedCallback<Ptr<const Interest>,
                 Ptr<const Face> > m_outNacks; ///< @brief trace of outgoing NACKs

  TracedCallback<Ptr<const Interest>,
                 Ptr<const Face> > m_inNacks; ///< @brief trace of incoming NACKs

  TracedCallback<Ptr<const Interest>,
                 Ptr<const Face> > m_dropNacks; ///< @brief trace of dropped NACKs
};

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_NACKS
