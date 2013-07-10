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
 *         Zhenkai Zhu <zhenkai@cs.ucla.edu>
 *         Chaoyi Bian <bcy@pku.edu.cn>
 */

#include "ndn-api-face.h"

#include <boost/throw_exception.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

#include <ns3/packet.h>

#include <ns3/ndn-interest.h>
#include <ns3/ndn-content-object.h>
#include <ns3/ndn-face.h>
#include <ns3/ndn-fib.h>
#include <ns3/log.h>

typedef boost::error_info<struct tag_errmsg, std::string> errmsg_info_str;
typedef boost::error_info<struct tag_errmsg, int> errmsg_info_int;

using namespace std;
using namespace boost;
using namespace ns3;

#define INIT_LOGGER NS_LOG_COMPONENT_DEFINE
#define _LOG_DEBUG NS_LOG_DEBUG
#define _LOG_TRACE NS_LOG_TRACE
#define _LOG_INFO NS_LOG_INFO


INIT_LOGGER ("ndn.ApiFace");

namespace ns3 {
namespace ndn {

// ApiFace::ApiFace()
//   : m_rand (0, std::numeric_limits<uint32_t>::max ())
// {
// }

// ApiFace::~ApiFace()
// {
// }

// int
// ApiFace::publishRawData (const std::string &name, const char *buf, size_t len, int freshness)
// {
//   // NS_LOG_INFO ("Requesting Interest: \n" << interestHeader);
//   _LOG_INFO (">> publishRawData " << name);

//   static ndn::ContentObjectTail trailer;
  
//   ndn::ContentObject data;
//   data.SetName (name);
//   data.SetFreshness (Seconds (freshness));

//   Ptr<Packet> packet = Create<Packet> (reinterpret_cast<const uint8_t*> (buf), len);
//   // packet->AddPacketTag (CreateObject<TypeTag> (TypeTag::DATA));
//   packet->AddHeader (data);
//   packet->AddTrailer (trailer);

//   m_protocolHandler (packet);

//   m_transmittedContentObjects (&data, packet, this, m_face);

//   return 0;
// }


// void
// RawDataCallback2StringDataCallback (ApiFace::StringDataCallback callback, std::string str, const char *buf, size_t len)
// {
//   callback (str, string (buf, len));
// }

// int
// ApiFace::sendInterestForString (const std::string &strInterest, const StringDataCallback &strDataCallback/*, int retry*/)
// {
//   return sendInterest (strInterest, boost::bind (RawDataCallback2StringDataCallback, strDataCallback, _1, _2, _3));
// }

// int ApiFace::sendInterest (const string &strInterest, const RawDataCallback &rawDataCallback)
// {
//   // NS_LOG_INFO ("Requesting Interest: \n" << interestHeader);
//   _LOG_INFO (">> I " << strInterest);
//   Ptr<ndn::Name> name = Create<ndn::Name> (strInterest);

//   ndn::Interest interestHeader;
//   interestHeader.SetNonce            (m_rand.GetValue ());
//   interestHeader.SetName             (*name);
//   interestHeader.SetInterestLifetime (Seconds (9.9)); // really long-lived interests

//   Ptr<Packet> packet = Create<Packet> ();
//   // packet->AddPacketTag (CreateObject<TypeTag> (TypeTag::INTEREST));
//   packet->AddHeader (interestHeader);

//   // NS_LOG_DEBUG (interestHeader);
  
//   // Record the callback
//   FilterEntryContainer<RawDataCallback>::iterator entry = m_dataCallbacks.find_exact (*name);
//   if (entry == m_dataCallbacks.end ())
//     {
//       pair<FilterEntryContainer<RawDataCallback>::iterator, bool> status =
//         m_dataCallbacks.insert (*name, Create< FilterEntry<RawDataCallback> > (name));

//       entry = status.first;
//     }
//   entry->payload ()->AddCallback (rawDataCallback);

//   m_protocolHanler (packet);
//   m_transmittedInterests (&interestHeader, this, m_face);
  
//   return 0;
// }

// int ApiFace::setInterestFilter (const string &prefix, const InterestCallback &interestCallback)
// {
//   NS_LOG_DEBUG ("== setInterestFilter " << prefix << " (" << GetNode ()->GetId () << ")");
//   Ptr<ndn::Name> name = Create<ndn::Name> (prefix);

//   FilterEntryContainer<InterestCallback>::iterator entry = m_interestCallbacks.find_exact (*name);
//   if (entry == m_interestCallbacks.end ())
//     {
//       pair<FilterEntryContainer<InterestCallback>::iterator, bool> status =
//         m_interestCallbacks.insert (*name, Create < FilterEntry<InterestCallback> > (name));

//       entry = status.first;
//     }

//   entry->payload ()->AddCallback (interestCallback);

//   // creating actual face
//   Ptr<ndn::Fib> fib = GetNode ()->GetObject<ndn::Fib> ();
//   Ptr<ndn::fib::Entry> fibEntry = fib->Add (name, m_face, 0);
//   fibEntry->UpdateStatus (m_face, ndn::fib::FaceMetric::NDN_FIB_GREEN);

//   return 0;
// }

// void
// ApiFace::clearInterestFilter (const std::string &prefix)
// {
//   Ptr<ndn::Name> name = Create<ndn::Name> (prefix);

//   FilterEntryContainer<InterestCallback>::iterator entry = m_interestCallbacks.find_exact (*name);
//   if (entry == m_interestCallbacks.end ())
//     return;

//   entry->payload ()->ClearCallback ();
// }

// void
// ApiFace::OnInterest (const Ptr<const ndn::Interest> &interest, Ptr<Packet> packet)
// {
//   ndn::App::OnInterest (interest, packet);

//   // the app cannot set several filters for the same prefix
//   FilterEntryContainer<InterestCallback>::iterator entry = m_interestCallbacks.longest_prefix_match (interest->GetName ());
//   if (entry == m_interestCallbacks.end ())
//     {
//       _LOG_DEBUG ("No Interest callback set");
//       return;
//     }
  
//   entry->payload ()->m_callback (lexical_cast<string> (interest->GetName ()));  
// }

// void
// ApiFace::OnContentObject (const Ptr<const ndn::ContentObject> &contentObject,
//                               Ptr<Packet> payload)
// {
//   ndn::App::OnContentObject (contentObject, payload);
//   NS_LOG_DEBUG ("<< D " << contentObject->GetName ());

//   FilterEntryContainer<RawDataCallback>::iterator entry = m_dataCallbacks.longest_prefix_match (contentObject->GetName ());
//   if (entry == m_dataCallbacks.end ())
//     {
//       _LOG_DEBUG ("No Data callback set");
//       return;
//     }

//   while (entry != m_dataCallbacks.end ())
//     {
//       entry->payload ()->m_callback (lexical_cast<string> (contentObject->GetName ()), payload);
//       m_dataCallbacks.erase (entry);

//       entry = m_dataCallbacks.longest_prefix_match (contentObject->GetName ());
//     }
// }

}
}
