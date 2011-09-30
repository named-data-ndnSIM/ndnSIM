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
#ifndef CCNX_FORWARDING_STRATEGY_H
#define CCNX_FORWARDING_STRATEGY_H

#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/object.h"

#include "ccnx.h"
#include "ccnx-fib.h"

namespace ns3 {

class CcnxFace;
class CcnxInterestHeader;

/**
 * \ingroup ccnx
 * \brief Abstract base class for Ccnx forwarding protocols
 */
class CcnxForwardingStrategy : public Object
{
public:
  static TypeId GetTypeId (void);

  CcnxForwardingStrategy ();
    
  void SetCcnx(Ptr<Ccnx> ccnx);
    
  typedef
  Callback<void, const Ptr<CcnxFace> &, const Ptr<CcnxInterestHeader> &, const Ptr<Packet> &>
  SendCallback;

  virtual bool PropagateInterest  (const Ptr<CcnxFace> &incomingFace,
                                   Ptr<CcnxInterestHeader> &header,
                                   const Ptr<const Packet> &packet,
                                   SendCallback ucb) = 0;
  Ptr<Ccnx> GetCcnx();
    
private:  
  Ptr<Ccnx> m_ccnx;
};

} //namespace ns3

#endif /* CCNX_FORWARDING_STRATEGY_H */
