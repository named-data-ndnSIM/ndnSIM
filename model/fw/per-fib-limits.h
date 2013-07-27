/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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


#ifndef NDNSIM_PER_FIB_LIMITS_H
#define NDNSIM_PER_FIB_LIMITS_H

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
 * @brief Strategy implementing per-FIB entry limits
 */
template<class Parent>
class PerFibLimits :
    public Parent
{
private:
  typedef Parent super;

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
  PerFibLimits ()
  { }

  /// \copydoc ForwardingStrategy::WillEraseTimedOutPendingInterest
  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);

  /// \copydoc ForwardingStrategy::AddFace
  virtual void
  AddFace (Ptr<Face> face)
  {
    super::AddFace (face);

    if (face->GetObject<Limits> () == 0)
      {
        NS_FATAL_ERROR ("At least per-face limits should be enabled");
        exit (1);
      }
  }

  /// \copydoc ForwardingStrategy::DidAddFibEntry
  virtual void
  DidAddFibEntry (Ptr<fib::Entry> fibEntry)
  {
    ObjectFactory factory;
    factory.SetTypeId (fibEntry->m_faces.begin ()->GetFace ()->GetObject<Limits> ()->GetInstanceTypeId ());

    Ptr<Limits> limits = factory.template Create<Limits> ();
    fibEntry->AggregateObject (limits);

    super::DidAddFibEntry (fibEntry);
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
LogComponent PerFibLimits<Parent>::g_log = LogComponent (PerFibLimits<Parent>::GetLogName ().c_str ());

template<class Parent>
std::string
PerFibLimits<Parent>::GetLogName ()
{
  return super::GetLogName ()+".PerFibLimits";
}

template<class Parent>
TypeId
PerFibLimits<Parent>::GetTypeId (void)
{
  static TypeId tid = TypeId ((super::GetTypeId ().GetName ()+"::PerFibLimits").c_str ())
    .SetGroupName ("Ndn")
    .template SetParent <super> ()
    .template AddConstructor <PerFibLimits> ()
    ;
  return tid;
}

template<class Parent>
bool
PerFibLimits<Parent>::CanSendOutInterest (Ptr<Face> inFace,
                                          Ptr<Face> outFace,
                                          Ptr<const Interest> interest,
                                          Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  Ptr<Limits> fibLimits = pitEntry->GetFibEntry ()->template GetObject<Limits> ();
  // no checks for the limit here. the check should be somewhere elese

  if (fibLimits->IsBelowLimit ())
    {
      if (super::CanSendOutInterest (inFace, outFace, interest, pitEntry))
        {
          fibLimits->BorrowLimit ();
          return true;
        }
    }

  return false;
}

template<class Parent>
void
PerFibLimits<Parent>::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  Ptr<Limits> fibLimits = pitEntry->GetFibEntry ()->template GetObject<Limits> ();

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      for (uint32_t i = 0; i <= face->m_retxCount; i++)
        fibLimits->ReturnLimit ();
    }

  super::WillEraseTimedOutPendingInterest (pitEntry);
}


template<class Parent>
void
PerFibLimits<Parent>::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                                  Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  Ptr<Limits> fibLimits = pitEntry->GetFibEntry ()->template GetObject<Limits> ();

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      for (uint32_t i = 0; i <= face->m_retxCount; i++)
        fibLimits->ReturnLimit ();
    }

  super::WillSatisfyPendingInterest (inFace, pitEntry);
}

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_PER_FIB_LIMITS_H
