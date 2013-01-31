/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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

#include "ndn-app-delay-tracer.h"
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
#include <boost/make_shared.hpp>

#include <fstream>

NS_LOG_COMPONENT_DEFINE ("ndn.AppDelayTracer");

using namespace std;

namespace ns3 {
namespace ndn {


boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<AppDelayTracer> > >
AppDelayTracer::InstallAll (const std::string &file)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<AppDelayTracer> > tracers;
  boost::shared_ptr<std::ofstream> outputStream = make_shared<std::ofstream> ();

  outputStream->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);
  if (!outputStream->is_open ())
    return boost::make_tuple (outputStream, tracers);

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      NS_LOG_DEBUG ("Node: " << (*node)->GetId ());

      Ptr<AppDelayTracer> trace = Create<AppDelayTracer> (outputStream, *node);
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

AppDelayTracer::AppDelayTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node)
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

AppDelayTracer::AppDelayTracer (boost::shared_ptr<std::ostream> os, const std::string &node)
: m_node (node)
, m_os (os)
{
  Connect ();
}

AppDelayTracer::~AppDelayTracer ()
{
};


void
AppDelayTracer::Connect ()
{
  Config::ConnectWithoutContext ("/NodeList/"+m_node+"/ApplicationList/*/LastRetransmittedInterestDataDelay",
                                 MakeCallback (&AppDelayTracer::LastRetransmittedInterestDataDelay, this));

  Config::ConnectWithoutContext ("/NodeList/"+m_node+"/ApplicationList/*/FirstInterestDataDelay",
                                 MakeCallback (&AppDelayTracer::FirstInterestDataDelay, this));
}

void
AppDelayTracer::PrintHeader (std::ostream &os) const
{
  os << "Time" << "\t"
     << "Node" << "\t"
     << "AppId" << "\t"
     << "SeqNo" << "\t"

     << "Type" << "\t"
     << "DelayS" << "\t"
     << "DelayUS" << "\t"
     << "RetxCount" << "\t"
     << "HopCount"  << "";
}

void
AppDelayTracer::LastRetransmittedInterestDataDelay (Ptr<App> app, uint32_t seqno, Time delay, int32_t hopCount)
{
  *m_os << Simulator::Now ().ToDouble (Time::S) << "\t"
        << m_node << "\t"
        << app->GetId () << "\t"
        << seqno << "\t"
        << "LastDelay" << "\t"
        << delay.ToDouble (Time::S) << "\t"
        << delay.ToDouble (Time::US) << "\t"
        << 1 << "\t"
        << hopCount << "\n";
}

void
AppDelayTracer::FirstInterestDataDelay (Ptr<App> app, uint32_t seqno, Time delay, uint32_t retxCount, int32_t hopCount)
{
  *m_os << Simulator::Now ().ToDouble (Time::S) << "\t"
        << m_node << "\t"
        << app->GetId () << "\t"
        << seqno << "\t"
        << "FullDelay" << "\t"
        << delay.ToDouble (Time::S) << "\t"
        << delay.ToDouble (Time::US) << "\t"
        << retxCount << "\t"
        << hopCount << "\n";
}


} // namespace ndn
} // namespace ns3
