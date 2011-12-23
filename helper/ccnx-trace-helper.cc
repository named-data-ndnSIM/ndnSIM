/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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

#include "ccnx-trace-helper.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/ccnx-app.h"
#include "ns3/ccnx-face.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/node-list.h"
#include "ns3/object-vector.h"
#include "ns3/simulator.h"
#include "ns3/names.h"

#include <boost/ref.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include <fstream>

using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("CcnxTraceHelper");

namespace ns3 {
    
CcnxAppTracer::CcnxAppTracer (const std::string &app, Ptr<Node> node, const std::string &appId)
  : m_app (app)
  , m_appId (appId)
  , m_nodePtr (node)
{
  m_node = boost::lexical_cast<string> (m_nodePtr->GetId ());

  Connect ();

  string name = Names::FindName (node);
  if (!name.empty ())
    {
      m_node = name;
    }
}

CcnxAppTracer::CcnxAppTracer (const std::string &app, const std::string &node, const std::string &appId)
  : m_app (app)
  , m_appId (appId)
  , m_node (node)
{
  Connect ();
}

void
CcnxAppTracer::Connect ()
{
  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/TransmittedInterests",
                   MakeCallback (&CcnxAppTracer::OutInterests, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/ReceivedNacks",
                   MakeCallback (&CcnxAppTracer::InNacks, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/ReceivedInterests",
                   MakeCallback (&CcnxAppTracer::InInterests, this));
  
  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/TransmittedContentObjects",
                   MakeCallback (&CcnxAppTracer::OutData, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/ReceivedContentObjects",
                   MakeCallback (&CcnxAppTracer::InData, this));
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

CcnxL3Tracer::CcnxL3Tracer (Ptr<Node> node)
: m_nodePtr (node)
{
  m_node = boost::lexical_cast<string> (m_nodePtr->GetId ());

  Connect ();

  string name = Names::FindName (node);
  if (!name.empty ())
    {
      m_node = name;
    }
}

CcnxL3Tracer::CcnxL3Tracer (const std::string &node)
: m_node (node)
{
  Connect ();
}

void
CcnxL3Tracer::Connect ()
{
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/ForwardingStrategy/OutInterests",
                   MakeCallback (&CcnxL3Tracer::OutInterests, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/InInterests",
                   MakeCallback (&CcnxL3Tracer::InInterests, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/DropInterests",
                   MakeCallback (&CcnxL3Tracer::DropInterests, this));

  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/OutNacks",
                   MakeCallback (&CcnxL3Tracer::OutNacks, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/InNacks",
                   MakeCallback (&CcnxL3Tracer::InNacks, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/DropNacks",
                   MakeCallback (&CcnxL3Tracer::DropNacks, this));

  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/OutData",
                   MakeCallback (&CcnxL3Tracer::OutData, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/InData",
                   MakeCallback (&CcnxL3Tracer::InData, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/DropData",
                   MakeCallback (&CcnxL3Tracer::DropData, this));
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
CcnxAggregateAppTracer::CcnxAggregateAppTracer (const std::string &app, Ptr<Node> node, const std::string &appId)
  : CcnxAppTracer (app, node, appId)
  , m_inInterests (0)
  , m_outInterests (0)
  , m_inNacks (0)
  , m_inData (0)
  , m_outData (0)

  , m_inInterestsBytes (0)
  , m_outInterestsBytes (0)
  , m_inNacksBytes (0)
  , m_inDataBytes (0)
  , m_outDataBytes (0)
{
}

CcnxAggregateAppTracer::CcnxAggregateAppTracer (const std::string &app, const std::string &node, const std::string &appId)
  : CcnxAppTracer (app, node, appId)
  , m_inInterests (0)
  , m_outInterests (0)
  , m_inNacks (0)
  , m_inData (0)
  , m_outData (0)

  , m_inInterestsBytes (0)
  , m_outInterestsBytes (0)
  , m_inNacksBytes (0)
  , m_inDataBytes (0)
  , m_outDataBytes (0)
{
}

void
CcnxAggregateAppTracer::Reset ()
{
  m_inInterests = 0;
  m_outInterests = 0;
  m_inNacks = 0;
  m_inData = 0;
  m_outData = 0;

  m_inInterestsBytes = 0;
  m_outInterestsBytes = 0;
  m_inNacksBytes = 0;
  m_inDataBytes = 0;
  m_outDataBytes = 0;
}

void
CcnxAggregateAppTracer::PrintHeader (std::ostream &os) const
{
  os << "NodeId" << "\t"
     << "App" << "\t"
     << "AppId" << "\t"
     << "InInterests" << "\t"
     << "OutInterests" << "\t"
    
     << "InNacks" << "\t"
    
     << "InData" << "\t"
     << "OutData" << "\t"

     << "InInterestsBytes" << "\t"
     << "OutInterestsBytes" << "\t"
    
     << "InNacksBytes" << "\t"
    
     << "InDataBytes" << "\t"
     << "OutDataBytes";
}

void
CcnxAggregateAppTracer::Print (std::ostream &os) const
{
  os << m_node << "\t"
     << m_app << "\t"
     << m_appId << "\t"

     << m_inInterests << "\t"
     << m_outInterests << "\t"
    
     << m_inNacks << "\t"
    
     << m_inData << "\t"
     << m_outData << "\t"

     << m_inInterestsBytes << "\t"
     << m_outInterestsBytes << "\t"
    
     << m_inNacksBytes << "\t"
    
     << m_inDataBytes << "\t"
     << m_outDataBytes;
}

void
CcnxAggregateAppTracer::OutInterests (std::string context,
                                      Ptr<const CcnxInterestHeader> header, Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_outInterests++;
  m_outInterestsBytes += header->GetSerializedSize ();
}

void
CcnxAggregateAppTracer::OutData (std::string context,
                                 Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> payload,
                                 Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_outData++;
  m_outDataBytes += header->GetSerializedSize () + payload->GetSerializedSize ();
}

void
CcnxAggregateAppTracer::InInterests (std::string context,
                                     Ptr<const CcnxInterestHeader> header,
                                     Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inInterests++;
  m_inInterestsBytes += header->GetSerializedSize ();
}

void
CcnxAggregateAppTracer::InNacks (std::string context,
                                 Ptr<const CcnxInterestHeader> header,
                                 Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inNacks++;
  m_inNacksBytes += header->GetSerializedSize ();
}
  
void
CcnxAggregateAppTracer::InData (std::string context,
                                Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> payload,
                                Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inData++;
  m_inDataBytes += header->GetSerializedSize () + payload->GetSerializedSize ();
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

CcnxAggregateL3Tracer::CcnxAggregateL3Tracer (Ptr<Node> node)
  : CcnxL3Tracer (node)
{
  Reset ();
}

CcnxAggregateL3Tracer::CcnxAggregateL3Tracer (const std::string &node)
  : CcnxL3Tracer (node)
{
  Reset ();
}

void
CcnxAggregateL3Tracer::Stats::Reset ()
{
  m_inInterests = 0;
  m_outInterests = 0;
  m_dropInterests = 0;
  m_inNacks = 0;
  m_outNacks = 0;
  m_dropNacks = 0;
  m_inData = 0;
  m_outData = 0;
  m_dropData = 0;
}


void
CcnxAggregateL3Tracer::Reset ()
{
  m_packets.Reset ();
  m_bytes.Reset ();
}


void
CcnxAggregateL3Tracer::PrintHeader (std::ostream &os) const
{
  os << "Node" << "\t"
     << "InInterests" << "\t"
     << "OutInterests" << "\t"
     << "DropInterests" << "\t"
    
     << "InNacks" << "\t"
     << "OutNacks" << "\t"
     << "DropNacks" << "\t"
    
     << "InData" << "\t"
     << "OutData" << "\t"
     << "DropData" << "\t"
    
     << "InInterestsBytes" << "\t"
     << "OutInterestsBytes" << "\t"
     << "DropInterestsBytes" << "\t"
    
     << "InNacksBytes" << "\t"
     << "OutNacksBytes" << "\t"
     << "DropNacksBytes" << "\t"
    
     << "InDataBytes" << "\t"
     << "OutDataBytes" << "\t"
     << "DropDataBytes";
}

void
CcnxAggregateL3Tracer::Print (std::ostream &os) const
{
  os << m_node << "\t"
     << m_packets.m_inInterests   << "\t"
     << m_packets.m_outInterests  << "\t"
     << m_packets.m_dropInterests << "\t"

     << m_packets.m_inNacks   << "\t"
     << m_packets.m_outNacks  << "\t"
     << m_packets.m_dropNacks << "\t"

     << m_packets.m_inData   << "\t"
     << m_packets.m_outData  << "\t"
     << m_packets.m_dropData << "\t"

     << m_bytes.m_inInterests   << "\t"
     << m_bytes.m_outInterests  << "\t"
     << m_bytes.m_dropInterests << "\t"

     << m_bytes.m_inNacks   << "\t"
     << m_bytes.m_outNacks  << "\t"
     << m_bytes.m_dropNacks << "\t"

     << m_bytes.m_inData   << "\t"
     << m_bytes.m_outData  << "\t"
     << m_bytes.m_dropData;
}

void
CcnxAggregateL3Tracer::OutInterests  (std::string context,
                                      Ptr<const CcnxInterestHeader> header, Ptr<const CcnxFace>)
{
  m_packets.m_outInterests++;
  m_bytes.m_outInterests += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::InInterests   (std::string context,
                                      Ptr<const CcnxInterestHeader> header, Ptr<const CcnxFace>)
{
  m_packets.m_inInterests++;
  m_bytes.m_inInterests += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::DropInterests (std::string context,
                                      Ptr<const CcnxInterestHeader> header, Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_packets.m_dropInterests++;
  m_bytes.m_dropInterests += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::OutNacks  (std::string context,
                                  Ptr<const CcnxInterestHeader> header, Ptr<const CcnxFace>)
{
  m_packets.m_outNacks++;
  m_bytes.m_outNacks += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::InNacks   (std::string context,
                                  Ptr<const CcnxInterestHeader> header, Ptr<const CcnxFace>)
{
  m_packets.m_inNacks++;
  m_bytes.m_inNacks += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::DropNacks (std::string context,
                                  Ptr<const CcnxInterestHeader> header, Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_packets.m_dropNacks++;
  m_bytes.m_dropNacks += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::OutData  (std::string context,
                                 Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> payload,
                                 bool fromCache, Ptr<const CcnxFace>)
{
  m_packets.m_outData++;
  m_bytes.m_outData += header->GetSerializedSize () + payload->GetSize ();
}

void
CcnxAggregateL3Tracer::InData   (std::string context,
                                 Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> payload,
                                 Ptr<const CcnxFace>)
{
  m_packets.m_inData++;
  m_bytes.m_inData += header->GetSerializedSize () + payload->GetSize ();
}

void
CcnxAggregateL3Tracer::DropData (std::string context,
                                 Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> payload,
                                 Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_packets.m_dropData++;
  m_bytes.m_dropData += header->GetSerializedSize () + payload->GetSize ();
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

CcnxRateL3Tracer::CcnxRateL3Tracer (std::ostream &os, Ptr<Node> node)
  : CcnxL3Tracer (node)
  , m_os (os)
{
  SetAveragingPeriod (Seconds (1.0));
}

CcnxRateL3Tracer::CcnxRateL3Tracer (std::ostream &os, const std::string &node)
  : CcnxL3Tracer (node)
  , m_os (os)
{
  SetAveragingPeriod (Seconds (1.0));
}

CcnxRateL3Tracer::~CcnxRateL3Tracer ()
{
  m_printEvent.Cancel ();
}

void
CcnxRateL3Tracer::SetAveragingPeriod (const Time &period)
{
  m_period = period;
  m_printEvent.Cancel ();
  m_printEvent = Simulator::Schedule (m_period, &CcnxRateL3Tracer::PeriodicPrinter, this);
}

void
CcnxRateL3Tracer::PeriodicPrinter ()
{
  Print (m_os);
  Reset ();
  
  m_printEvent = Simulator::Schedule (m_period, &CcnxRateL3Tracer::PeriodicPrinter, this);
}

void
CcnxRateL3Tracer::PrintHeader (std::ostream &os) const
{
  os << "Time" << "\t"

     << "Node" << "\t"
     << "FaceId" << "\t"
     << "FaceDescr" << "\t"

     << "Type" << "\t"
     << "Packets" << "\t"
     << "Kilobytes";
}

void
CcnxRateL3Tracer::Reset ()
{
  for (std::map<Ptr<const CcnxFace>, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.begin ();
       stats != m_stats.end ();
       stats++)
    {
      stats->second.get<0> ().Reset ();
      stats->second.get<1> ().Reset ();
    }
}

const double alpha = 0.8;

#define STATS(INDEX) stats->second.get<INDEX> ()
#define RATE(INDEX, fieldName) STATS(INDEX).fieldName / m_period.ToDouble (Time::S)

#define PRINTER(printName, fieldName) \
STATS(2).fieldName = /*new value*/alpha * RATE(0, fieldName) + /*old value*/(1-alpha) * STATS(2).fieldName; \
 STATS(3).fieldName = /*new value*/alpha * RATE(1, fieldName) / 1024.0 + /*old value*/(1-alpha) * STATS(3).fieldName; \
                                                                        \
os << time.ToDouble (Time::S) << "\t"                                   \
 << m_node << "\t"                                                      \
 << stats->first->GetId () << "\t"                                      \
 << *stats->first << "\t"                                               \
 << printName << "\t"                                                   \
 << STATS(2).fieldName  << "\t"                                        \
 << STATS(3).fieldName << "\n";

void
CcnxRateL3Tracer::Print (std::ostream &os) const
{
  for (std::map<Ptr<const CcnxFace>, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.begin ();
       stats != m_stats.end ();
       stats++)
    {
      Time time = Simulator::Now ();

      PRINTER ("InInterests",   m_inInterests);
      PRINTER ("OutInterests",  m_outInterests);
      PRINTER ("DropInterests", m_dropInterests);
      
      PRINTER ("InNacks",   m_inNacks);
      PRINTER ("OutNacks",  m_outNacks);
      PRINTER ("DropNacks", m_dropNacks);

      PRINTER ("InData",   m_inData);
      PRINTER ("OutData",  m_outData);
      PRINTER ("DropData", m_dropData);

      // os << Simulator::Now ().ToDouble (Time::S) << "\t"

      //    << m_node << "\t"
      //    << stats->first->GetId () << "\t"
      //    << *stats->first << "\t"
      //    << "InInterests" << "\t" << 
      //    << stats->second.get<0> ().m_inInterests / m_period.ToDouble (Time::S)  << "\t"
      //    << stats->second.get<1> ().m_inInterests / m_period.ToDouble (Time::S) / 1024.0;

      //    << stats->second.get<0> ().m_outInterests / m_period.ToDouble (Time::S)  << "\t"
      //    << stats->second.get<0> ().m_dropInterests / m_period.ToDouble (Time::S) << "\t"

      //    << stats->second.get<0> ().m_inNacks / m_period.ToDouble (Time::S)   << "\t"
      //    << stats->second.get<0> ().m_outNacks / m_period.ToDouble (Time::S)  << "\t"
      //    << stats->second.get<0> ().m_dropNacks / m_period.ToDouble (Time::S) << "\t"

      //    << stats->second.get<0> ().m_inData / m_period.ToDouble (Time::S)   << "\t"
      //    << stats->second.get<0> ().m_outData / m_period.ToDouble (Time::S)  << "\t"
      //    << stats->second.get<0> ().m_dropData / m_period.ToDouble (Time::S) << "\t"

      //    << stats->second.get<1> ().m_outInterests / m_period.ToDouble (Time::S) / 1024.0  << "\t"
      //    << stats->second.get<1> ().m_dropInterests / m_period.ToDouble (Time::S) / 1024.0 << "\t"

      //    << stats->second.get<1> ().m_inNacks / m_period.ToDouble (Time::S) / 1024.0   << "\t"
      //    << stats->second.get<1> ().m_outNacks / m_period.ToDouble (Time::S) / 1024.0  << "\t"
      //    << stats->second.get<1> ().m_dropNacks / m_period.ToDouble (Time::S) / 1024.0 << "\t"

      //    << stats->second.get<1> ().m_inData / m_period.ToDouble (Time::S) / 1024.0   << "\t"
      //    << stats->second.get<1> ().m_outData / m_period.ToDouble (Time::S) / 1024.0  << "\t"
      //    << stats->second.get<1> ().m_dropData / m_period.ToDouble (Time::S) / 1024.0;
    }
}


void
CcnxRateL3Tracer::OutInterests  (std::string context,
                                      Ptr<const CcnxInterestHeader> header, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_outInterests ++;
  m_stats[face].get<1> ().m_outInterests += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::InInterests   (std::string context,
                                 Ptr<const CcnxInterestHeader> header, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_inInterests ++;
  m_stats[face].get<1> ().m_inInterests += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::DropInterests (std::string context,
                                      Ptr<const CcnxInterestHeader> header, Ccnx::DropReason, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_dropInterests ++;
  m_stats[face].get<1> ().m_dropInterests += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::OutNacks  (std::string context,
                                  Ptr<const CcnxInterestHeader> header, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_outNacks ++;
  m_stats[face].get<1> ().m_outNacks += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::InNacks   (std::string context,
                                  Ptr<const CcnxInterestHeader> header, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_inNacks ++;
  m_stats[face].get<1> ().m_inNacks += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::DropNacks (std::string context,
                             Ptr<const CcnxInterestHeader> header, Ccnx::DropReason, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_dropNacks ++;
  m_stats[face].get<1> ().m_dropNacks += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::OutData  (std::string context,
                            Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> payload,
                            bool fromCache, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_inData ++;
  m_stats[face].get<1> ().m_inData += header->GetSerializedSize () + payload->GetSize ();
}

void
CcnxRateL3Tracer::InData   (std::string context,
                            Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> payload,
                            Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_outData ++;
  m_stats[face].get<1> ().m_outData += header->GetSerializedSize () + payload->GetSize ();
}

void
CcnxRateL3Tracer::DropData (std::string context,
                            Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> payload,
                            Ccnx::DropReason, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_dropData ++;
  m_stats[face].get<1> ().m_dropData += header->GetSerializedSize () + payload->GetSize ();
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

CcnxTraceHelper::CcnxTraceHelper ()
  : m_l3RateTrace (0)
{
}

CcnxTraceHelper::~CcnxTraceHelper ()
{
  NS_LOG_FUNCTION (this);
  if (m_l3RateTrace != 0) delete m_l3RateTrace;
  
  if (m_apps.size () > 0)
    {
      ofstream of;
      if (!m_appTrace.empty ())
        {
          of.open (m_appTrace.c_str (), ios_base::trunc | ios_base::out);
          of << "# ";
          m_apps.front ()->PrintHeader (of);
          of << "\n";
        }
      
      for (std::list<Ptr<CcnxAggregateAppTracer> >::iterator app = m_apps.begin ();
           app != m_apps.end ();
           app++)
        {
          if (!m_appTrace.empty ())
            {
              (*app)->Print (of);
              of << "\n";
            }
          else
            {
              NS_LOG_INFO (*(*app));
            }
        }
    }

  if (m_l3s.size () > 0)
    {
      ofstream of;
      if (!m_l3Trace.empty ())
        {
          of.open (m_l3Trace.c_str (), ios_base::trunc | ios_base::out);
          of << "# ";
          m_l3s.front ()->PrintHeader (of);
          of << "\n";
        }
      
      for (std::list<Ptr<CcnxAggregateL3Tracer> >::iterator l3 = m_l3s.begin ();
           l3 != m_l3s.end ();
           l3++)
        {
          if (!m_l3Trace.empty ())
            {
              (*l3)->Print (of);
              of << "\n";
            }
          else
            {
              NS_LOG_INFO (*(*l3));
            }
        }
    }
}

void
CcnxTraceHelper::SetAppTraceFile (const std::string &appTrace)
{
  NS_LOG_FUNCTION (this << appTrace);
  m_appTrace = appTrace;
}

void
CcnxTraceHelper::SetL3TraceFile (const std::string &l3Trace)
{
  NS_LOG_FUNCTION (this << l3Trace);
  m_l3Trace = l3Trace;
}

void
CcnxTraceHelper::EnableAggregateAppAll (const std::string &appName)
{
  NS_LOG_FUNCTION (this << appName);
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      ObjectVectorValue apps;
      (*node)->GetAttribute ("ApplicationList", apps);

      NS_LOG_DEBUG ("Node: " << lexical_cast<string> ((*node)->GetId ()));
      
      uint32_t appId = 0;
      for (ObjectVectorValue::Iterator app = apps.Begin ();
           app != apps.End ();
           app++, appId++)
        {
          NS_LOG_DEBUG ("App: " << lexical_cast<string> (appId) << ", typeId: " << (*app)->GetInstanceTypeId ().GetName ());
          if ((*app)->GetInstanceTypeId ().GetName () == appName)
            {
              m_apps.push_back (Create<CcnxAggregateAppTracer> (appName,
                                                                *node,
                                                                lexical_cast<string> (appId)));
            }
        }
    }
}

void
CcnxTraceHelper::EnableAggregateL3All ()
{
  NS_LOG_FUNCTION (this);
  
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      NS_LOG_DEBUG ("Node: " << lexical_cast<string> ((*node)->GetId ()));
      
      m_l3s.push_back (Create<CcnxAggregateL3Tracer> (*node));
    }
}

void
CcnxTraceHelper::EnableRateL3All (const std::string &l3RateTrace)
{
  NS_LOG_FUNCTION (this);
  m_l3RateTrace = new ofstream (l3RateTrace.c_str (), ios::trunc);

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      NS_LOG_DEBUG ("Node: " << lexical_cast<string> ((*node)->GetId ()));

      Ptr<CcnxRateL3Tracer> trace = Create<CcnxRateL3Tracer> (boost::ref(*m_l3RateTrace), *node);
      trace->SetAveragingPeriod (Seconds (0.5));
      m_l3Rates.push_back (trace);
    }

  if (m_l3Rates.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      m_l3Rates.front ()->PrintHeader (*m_l3RateTrace);
      *m_l3RateTrace << "\n";
    }
}

} // namespace ns3
