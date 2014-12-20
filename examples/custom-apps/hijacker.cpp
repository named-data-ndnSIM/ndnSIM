/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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

// hijacker.cc

#include "hijacker.h"
#include "ns3/ndn-name.h"

NS_LOG_COMPONENT_DEFINE ("Hijacker");

namespace ns3 {

// Necessary if you are planning to use ndn::AppHelper
NS_OBJECT_ENSURE_REGISTERED (Hijacker);

TypeId
Hijacker::GetTypeId ()
{
  static TypeId tid = TypeId ("Hijacker")
    .SetParent<ndn::App> ()
    .AddConstructor<Hijacker> ()
    ;

  return tid;
}

Hijacker::Hijacker ()
{
}

void
Hijacker::OnInterest (Ptr<const ndn::Interest> interest)
{
  ndn::App::OnInterest (interest); // forward call to perform app-level tracing
  // do nothing else (hijack interest)

  NS_LOG_DEBUG ("Do nothing for incoming interest for" << interest->GetName ());
}

void
Hijacker::StartApplication ()
{
  App::StartApplication ();

  // equivalent to setting interest filter for "/" prefix
  Ptr<ndn::Fib> fib = GetNode ()->GetObject<ndn::Fib> ();
  Ptr<ndn::fib::Entry> fibEntry = fib->Add (ndn::Name ("/"), m_face, 0);
  fibEntry->UpdateStatus (m_face, ndn::fib::FaceMetric::NDN_FIB_GREEN);
}

void
Hijacker::StopApplication ()
{
  App::StopApplication ();
}

} // namespace ns3

