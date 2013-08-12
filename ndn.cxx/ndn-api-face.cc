/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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

#include "ndn-api-face.h"
#include "detail/pending-interests-container.h"
#include "detail/registered-prefix-container.h"

#include <ns3/random-variable.h>

#include <ns3/ndn-l3-protocol.h>
#include <ns3/ndn-interest.h>
#include <ns3/ndn-data.h>
#include <ns3/ndn-face.h>
#include <ns3/ndn-fib.h>

#include <ns3/packet.h>
#include <ns3/log.h>

using namespace std;
using namespace boost;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ndn.ApiFace");

namespace ns3 {
namespace ndn {

using namespace detail;

class ApiFacePriv
{
public:
  ApiFacePriv ()
    : m_rand (0, std::numeric_limits<uint32_t>::max ())
  {
  }

  ns3::UniformVariable m_rand; // nonce generator

  PendingInterestContainer m_pendingInterests;
  RegisteredPrefixContainer m_expectedInterests;
};


ApiFace::ApiFace (Ptr<Node> node)
  : Face (node)
  , m_this (new ApiFacePriv ())
{
  NS_LOG_FUNCTION (this << boost::cref (*this));

  NS_ASSERT_MSG (GetNode ()->GetObject<L3Protocol> () != 0,
                 "NDN stack should be installed on the node " << GetNode ());

  GetNode ()->GetObject<L3Protocol> ()->AddFace (this);
  this->SetUp (true);
  this->SetFlags (APPLICATION);
}

ApiFace::~ApiFace ()
{
  NS_LOG_FUNCTION (this << boost::cref (*this));

  delete m_this;
}

void
ApiFace::Shutdown ()
{
  NS_LOG_FUNCTION (this << boost::cref (*this));

  if (!IsUp ())
    {
      return;
    }

  this->SetUp (false);

  m_this->m_pendingInterests.clear ();
  m_this->m_expectedInterests.clear ();

  GetNode ()->GetObject<L3Protocol> ()->RemoveFace (this);
}

void
ApiFace::ExpressInterest (Ptr<Interest> interest,
                          DataCallback onData,
                          TimeoutCallback onTimeout/* = MakeNullCallback< void, Ptr<Interest> > ()*/)
{
  NS_LOG_INFO (">> I " << interest->GetName ());

  if (interest->GetNonce () == 0)
    {
      interest->SetNonce (m_this->m_rand.GetValue ());
    }

  // Record the callback
  bool needToActuallyExpressInterest = false;
  PendingInterestContainer::iterator entry = m_this->m_pendingInterests.find_exact (interest->GetName ());
  if (entry == m_this->m_pendingInterests.end ())
    {
      pair<PendingInterestContainer::iterator, bool> status =
        m_this->m_pendingInterests.insert (interest->GetName (), Create <PendingInterestEntry> (interest));

      entry = status.first;

      needToActuallyExpressInterest = true;
    }
  entry->payload ()->AddCallbacks (onData, onTimeout);

  if (needToActuallyExpressInterest)
    {
      Simulator::ScheduleNow (&Face::ReceiveInterest, this, interest);
    }
}

void
ApiFace::SetInterestFilter (Ptr<const Name> prefix, InterestCallback onInterest)
{
  NS_LOG_DEBUG ("== setInterestFilter " << *prefix << " (" << GetNode ()->GetId () << ")");

  RegisteredPrefixContainer::iterator entry = m_this->m_expectedInterests.find_exact (*prefix);
  if (entry == m_this->m_expectedInterests.end ())
    {
      pair<RegisteredPrefixContainer::iterator, bool> status =
        m_this->m_expectedInterests.insert (*prefix, Create < RegisteredPrefixEntry > (prefix));

      entry = status.first;
    }

  entry->payload ()->AddCallback (onInterest);

  // creating actual face
  Ptr<ndn::Fib> fib = GetNode ()->GetObject<ndn::Fib> ();
  Ptr<ndn::fib::Entry> fibEntry = fib->Add (prefix, this, 0);
  fibEntry->UpdateStatus (this, ndn::fib::FaceMetric::NDN_FIB_GREEN);
}

void
ApiFace::ClearInterestFilter (Ptr<const Name> prefix)
{
  RegisteredPrefixContainer::iterator entry = m_this->m_expectedInterests.find_exact (*prefix);
  if (entry == m_this->m_expectedInterests.end ())
    return;

  entry->payload ()->ClearCallback ();
  m_this->m_expectedInterests.erase (entry);
}

void
ApiFace::Put (Ptr<Data> data)
{
  NS_LOG_INFO (">> D " << data->GetName ());

  Simulator::ScheduleNow (&Face::ReceiveData, this, data);
}


///////////////////////////////////////////////
// private stuff


bool
ApiFace::SendInterest (Ptr<const Interest> interest)
{
  NS_LOG_FUNCTION (this << interest);

  NS_LOG_DEBUG ("<< I " << interest->GetName ());

  if (!IsUp ())
    {
      return false;
    }

  // the app cannot set several filters for the same prefix
  RegisteredPrefixContainer::iterator entry = m_this->m_expectedInterests.longest_prefix_match (interest->GetName ());
  if (entry == m_this->m_expectedInterests.end ())
    {
      return false;
    }

  if (!entry->payload ()->m_callback.IsNull ())
    entry->payload ()->m_callback (entry->payload ()->GetPrefix (), interest);
  return true;
}

bool
ApiFace::SendData (Ptr<const Data> data)
{
  // data has been send out from NDN stack towards the application
  NS_LOG_DEBUG ("<< D " << data->GetName ());
  // NS_LOG_FUNCTION (this << data);

  if (!IsUp ())
    {
      return false;
    }

  PendingInterestContainer::iterator entry = m_this->m_pendingInterests.longest_prefix_match (data->GetName ());
  if (entry == m_this->m_pendingInterests.end ())
    {
      return false;
    }

  while (entry != m_this->m_pendingInterests.end ())
    {
      entry->payload ()->ProcessOnData (entry->payload ()->GetInterest (), data);
      m_this->m_pendingInterests.erase (entry);

      entry = m_this->m_pendingInterests.longest_prefix_match (data->GetName ());
    }
  m_this->m_pendingInterests.erase (entry);

  return true;
}

std::ostream&
ApiFace::Print (std::ostream &os) const
{
  os << "dev=ApiFace(" << GetId () << ")";
  return os;
}


}
}
