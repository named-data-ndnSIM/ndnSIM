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


#ifndef NDNSIM_SIMPLE_LIMITS_H
#define NDNSIM_SIMPLE_LIMITS_H

#include "ns3/event-id.h"
#include "ns3/ndn-pit.h"
#include "ns3/ndn-pit-entry.h"
#include "ns3/simulator.h"
#include "ns3/string.h"

#include "ns3/ndn-forwarding-strategy.h"

#include "ns3/ndn-limits.h"

namespace ns3 {
namespace ndn {
namespace fw {

/**
 * \ingroup ndn
 * \brief Strategy implementing per-FIB entry limits
 */
template<class Parent>
class SimpleLimits :
    public Parent
{
private:
  typedef Parent super;

public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Default constructor
   */
  SimpleLimits ()
  { }
  
  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);

  virtual void
  AddFace (Ptr<Face> face)
  {
    ObjectFactory factory (m_limitType);
    Ptr<Limits> limits = factory.template Create<Limits> ();
    face->AggregateObject (limits);

    super::AddFace (face);
  }
  
protected:
  virtual bool
  TrySendOutInterest (Ptr<Face> inFace,
                      Ptr<Face> outFace,
                      Ptr<const InterestHeader> header,
                      Ptr<const Packet> origPacket,
                      Ptr<pit::Entry> pitEntry);
  
  virtual void
  WillSatisfyPendingInterest (Ptr<Face> inFace,
                              Ptr<pit::Entry> pitEntry);

private:
  std::string m_limitType;
};

template<class Parent>
TypeId
SimpleLimits<Parent>::GetTypeId (void)
{
  static TypeId tid = TypeId ((super::GetTypeId ().GetName ()+"::SimpleLimits").c_str ())
    .SetGroupName ("Ndn")
    .template SetParent <super> ()
    .template AddConstructor <SimpleLimits> ()

    .template AddAttribute ("Limit", "Limit type to be used (e.g., ns3::ndn::Limits::Window or ns3::ndn::Limits::Rate)",
                            StringValue ("ns3::ndn::Limits::Window"),
                            MakeStringAccessor (&SimpleLimits<Parent>::m_limitType),
                            MakeStringChecker ())    
    ;
  return tid;
}

template<class Parent>
bool
SimpleLimits<Parent>::TrySendOutInterest (Ptr<Face> inFace,
                                                Ptr<Face> outFace,
                                                Ptr<const InterestHeader> header,
                                                Ptr<const Packet> origPacket,
                                                Ptr<pit::Entry> pitEntry)
{
  // NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  // totally override all (if any) parent processing
  
  pit::Entry::out_iterator outgoing =
    pitEntry->GetOutgoing ().find (outFace);

  if (outgoing != pitEntry->GetOutgoing ().end ())
    {
      // just suppress without any other action
      return false;
    }

  Ptr<Limits> faceLimits = outFace->template GetObject<Limits> ();
  if (faceLimits->IsBelowLimit ())
    {
      faceLimits->BorrowLimit ();
      pitEntry->AddOutgoing (outFace);

      //transmission
      Ptr<Packet> packetToSend = origPacket->Copy ();
      outFace->Send (packetToSend);

      this->DidSendOutInterest (outFace, header, origPacket, pitEntry);      
      return true;
    }
  else
    {
      // NS_LOG_DEBUG ("Face limit for " << header->GetName ());
    }

  return false;
}

template<class Parent>
void
SimpleLimits<Parent>::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  // NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      Ptr<Limits> faceLimits = face->m_face->GetObject<Limits> ();
      faceLimits->ReturnLimit ();
    }

  super::WillEraseTimedOutPendingInterest (pitEntry);
}


template<class Parent>
void
SimpleLimits<Parent>::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                                        Ptr<pit::Entry> pitEntry)
{
  // NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      Ptr<Limits> faceLimits = face->m_face->GetObject<Limits> ();
      faceLimits->ReturnLimit ();
    }
  
  super::WillSatisfyPendingInterest (inFace, pitEntry);
}

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_SIMPLE_LIMITS_H
