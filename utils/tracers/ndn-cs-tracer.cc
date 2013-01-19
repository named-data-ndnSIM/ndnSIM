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
 * Author: Xiaoyan Hu <x......u@gmail.com>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndn-cs-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"

#include "ns3/ndn-app.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/log.h"

#include <boost/lexical_cast.hpp>

#include <fstream>

NS_LOG_COMPONENT_DEFINE ("ndn.CsTracer");

using namespace std;

namespace ns3 {
namespace ndn {


boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<CsTracer> > >
CsTracer::InstallAll (const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;
  
  std::list<Ptr<CsTracer> > tracers;
  boost::shared_ptr<std::ofstream> outputStream (new std::ofstream ());
  outputStream->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

  if (!outputStream->is_open ())
    return boost::make_tuple (outputStream, tracers);

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      NS_LOG_DEBUG ("Node: " << (*node)->GetId ());

      Ptr<CsTracer> trace = Create<CsTracer> (outputStream, *node);
      trace->SetAveragingPeriod (averagingPeriod);
      tracers.push_back (trace);
    }

  if (tracers.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      tracers.front ()->PrintHeader (*outputStream);
      *outputStream << "\n";
    }

  return boost::make_tuple (outputStream, tracers);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

CsTracer::CsTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node)
: m_nodePtr (node)
, m_os (os)
{
  m_node = boost::lexical_cast<string> (m_nodePtr->GetId ());

  Connect ();

  string name = Names::FindName (node);
  if (!name.empty ())
    {
      m_node = name;
    }
}

CsTracer::CsTracer (boost::shared_ptr<std::ostream> os, const std::string &node)
: m_node (node)
, m_os (os)
{
  Connect ();
}

CsTracer::~CsTracer ()
{
};


void
CsTracer::Connect ()
{
  Config::ConnectWithoutContext ("/NodeList/"+m_node+"/$ns3::ndn::ContentStore/CacheHits",
                                 MakeCallback (&CsTracer::CacheHits, this));
  Config::ConnectWithoutContext ("/NodeList/"+m_node+"/$ns3::ndn::ContentStore/CacheMisses",
                                 MakeCallback (&CsTracer::CacheMisses, this));

  Reset ();  
}


void
CsTracer::SetAveragingPeriod (const Time &period)
{
  m_period = period;
  m_printEvent.Cancel ();
  m_printEvent = Simulator::Schedule (m_period, &CsTracer::PeriodicPrinter, this);
}

void
CsTracer::PeriodicPrinter ()
{
  Print (*m_os);
  Reset ();
  
  m_printEvent = Simulator::Schedule (m_period, &CsTracer::PeriodicPrinter, this);
}

void
CsTracer::PrintHeader (std::ostream &os) const
{
  os << "Time" << "\t"

     << "Node" << "\t"

     << "Type" << "\t"
     << "Packets" << "\t";
}

void
CsTracer::Reset ()
{
  m_stats.Reset();
}

#define PRINTER(printName, fieldName)           \
  os << time.ToDouble (Time::S) << "\t"         \
  << m_node << "\t"                             \
  << printName << "\t"                          \
  << m_stats.fieldName << "\n";


void
CsTracer::Print (std::ostream &os) const
{
  Time time = Simulator::Now ();

  PRINTER ("CacheHits",   m_cacheHits);
  PRINTER ("CacheMisses", m_cacheMisses);
}

void 
CsTracer::CacheHits (Ptr<const InterestHeader>, Ptr<const ContentObjectHeader>)
{
  m_stats.m_cacheHits ++;
}

void 
CsTracer::CacheMisses (Ptr<const InterestHeader>)
{
  m_stats.m_cacheMisses ++;
}


} // namespace ndn
} // namespace ns3
