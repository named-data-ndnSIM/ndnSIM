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


#ifndef NDNSIM_PER_OUT_FACE_LIMITS_H
#define NDNSIM_PER_OUT_FACE_LIMITS_H

#include "ns3/event-id.h"
#include "ns3/log.h"
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
 * @ingroup ndn-fw
 * @brief Strategy implementing per-outgoing face limits
 */
template<class Parent>
class PerOutFaceLimits :
    public Parent
{
private:
  typedef Parent super;

public:
  /**
   * @brief Get TypeId of the class
   */
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
  PerOutFaceLimits ()
  { }

  /// \copydoc ForwardingStrategy::WillEraseTimedOutPendingInterest
  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);

  /// \copydoc ForwardingStrategy::AddFace
  virtual void
  AddFace (Ptr<Face> face)
  {
    ObjectFactory factory (m_limitType);
    Ptr<Limits> limits = factory.template Create<Limits> ();
    face->AggregateObject (limits);

    super::AddFace (face);
  }
  
protected:
  /// \copydoc ForwardingStrategy::CanSendOutInterest
  virtual bool
  CanSendOutInterest (Ptr<Face> inFace,
                      Ptr<Face> outFace,
                      Ptr<const Interest> interest,
                      Ptr<pit::Entry> pitEntry);
  
  /// \copydoc ForwardingStrategy::WillSatisfyPendingInterest
  virtual void
  WillSatisfyPendingInterest (Ptr<Face> inFace,
                              Ptr<pit::Entry> pitEntry);

protected:
  static LogComponent g_log; ///< @brief Logging variable
  
private:
  std::string m_limitType;
};

template<class Parent>
LogComponent PerOutFaceLimits<Parent>::g_log = LogComponent (PerOutFaceLimits<Parent>::GetLogName ().c_str ());

template<class Parent>
std::string
PerOutFaceLimits<Parent>::GetLogName ()
{
  return super::GetLogName ()+".PerOutFaceLimits";
}

template<class Parent>
TypeId
PerOutFaceLimits<Parent>::GetTypeId (void)
{
  static TypeId tid = TypeId ((super::GetTypeId ().GetName ()+"::PerOutFaceLimits").c_str ())
    .SetGroupName ("Ndn")
    .template SetParent <super> ()
    .template AddConstructor <PerOutFaceLimits> ()

    .AddAttribute ("Limit", "Limit type to be used (e.g., ns3::ndn::Limits::Window or ns3::ndn::Limits::Rate)",
                   StringValue ("ns3::ndn::Limits::Window"),
                   MakeStringAccessor (&PerOutFaceLimits<Parent>::m_limitType),
                   MakeStringChecker ())    
    ;
  return tid;
}

template<class Parent>
bool
PerOutFaceLimits<Parent>::CanSendOutInterest (Ptr<Face> inFace,
                                              Ptr<Face> outFace,
                                              Ptr<const Interest> interest,
                                              Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  
  Ptr<Limits> faceLimits = outFace->template GetObject<Limits> ();
  if (faceLimits->IsBelowLimit ())
    {
      if (super::CanSendOutInterest (inFace, outFace, interest, pitEntry))
        {
          faceLimits->BorrowLimit ();
          return true;
        }
    }
  
  return false;
}

template<class Parent>
void
PerOutFaceLimits<Parent>::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      Ptr<Limits> faceLimits = face->m_face->GetObject<Limits> ();
      for (uint32_t i = 0; i <= face->m_retxCount; i++)
        faceLimits->ReturnLimit ();
    }

  super::WillEraseTimedOutPendingInterest (pitEntry);
}


template<class Parent>
void
PerOutFaceLimits<Parent>::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                                      Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      Ptr<Limits> faceLimits = face->m_face->GetObject<Limits> ();
      for (uint32_t i = 0; i <= face->m_retxCount; i++)
        faceLimits->ReturnLimit ();
    }
  
  super::WillSatisfyPendingInterest (inFace, pitEntry);
}

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_PER_OUT_FACE_LIMITS_H
