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
#include "ns3/tcp-l4-protocol.h"
#include "ns3/node.h"

#include <boost/ref.hpp>
#include <boost/lexical_cast.hpp>

#include "tracers/ccnx-aggregate-app-tracer.h"
#include "tracers/ccnx-aggregate-l3-tracer.h"
#include "tracers/ccnx-rate-l3-tracer.h"
#include "tracers/ccnx-seqs-app-tracer.h"
#include "tracers/ipv4-rate-l3-tracer.h"
#include "tracers/ipv4-seqs-app-tracer.h"
#include "tracers/ccnx-consumer-window-tracer.h"
#include "tracers/ccnx-path-weight-tracer.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

#include <fstream>

using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("CcnxTraceHelper");

namespace ns3 {

CcnxTraceHelper::CcnxTraceHelper ()
  : m_l3RateTrace (0)
  , m_appSeqsTrace (0)
  , m_ipv4RateTrace (0)
  , m_ipv4AppSeqsTrace (0)
  , m_windowsTrace (0)
  , m_windowsTcpTrace (0)
  , m_pathWeightsTrace (0)
{
}

CcnxTraceHelper::~CcnxTraceHelper ()
{
  NS_LOG_FUNCTION (this);
  if (m_l3RateTrace != 0) delete m_l3RateTrace;
  if (m_appSeqsTrace != 0) delete m_appSeqsTrace;
  if (m_ipv4AppSeqsTrace != 0) delete m_ipv4AppSeqsTrace;
  if (m_windowsTrace != 0) delete m_windowsTrace;
  if (m_windowsTcpTrace != 0) delete m_windowsTcpTrace;
  if (m_pathWeightsTrace != 0) delete m_pathWeightsTrace;
  if (m_ipv4RateTrace != 0) delete m_ipv4RateTrace;
  
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
      
      for (std::list<Ptr<CcnxAppTracer> >::iterator app = m_apps.begin ();
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
      
      for (std::list<Ptr<CcnxL3Tracer> >::iterator l3 = m_l3s.begin ();
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
          NS_LOG_DEBUG ("App: " << lexical_cast<string> (appId) << ", typeId: " << (*app).second->GetInstanceTypeId ().GetName ());
          if ((*app).second->GetInstanceTypeId ().GetName () == appName)
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
      trace->SetAveragingPeriod (Seconds (0.2));
      m_l3Rates.push_back (trace);
    }

  if (m_l3Rates.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      m_l3Rates.front ()->PrintHeader (*m_l3RateTrace);
      *m_l3RateTrace << "\n";
    }
}

void
CcnxTraceHelper::EnableIpv4RateL3All (const std::string &file)
{
  NS_LOG_FUNCTION (this);
  m_ipv4RateTrace = new ofstream (file.c_str (), ios::trunc);

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      NS_LOG_DEBUG ("Node: " << lexical_cast<string> ((*node)->GetId ()));

      Ptr<Ipv4RateL3Tracer> trace = Create<Ipv4RateL3Tracer> (boost::ref(*m_ipv4RateTrace), *node);
      trace->SetAveragingPeriod (Seconds (0.2));
      m_ipv4Rates.push_back (trace);
    }

  if (m_ipv4Rates.size () > 0)
    {
      // *m_ipv4RateTrace << "# "; // not necessary for R's read.table
      m_ipv4Rates.front ()->PrintHeader (*m_ipv4RateTrace);
      *m_ipv4RateTrace << "\n";
    }
}


void
CcnxTraceHelper::EnableSeqsAppAll (const std::string &appName, const std::string &trace)
{
  NS_LOG_FUNCTION (this);
  m_appSeqsTrace = new ofstream (trace.c_str (), ios::trunc);

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
          NS_LOG_DEBUG ("App: " << lexical_cast<string> (appId) << ", typeId: " << (*app).second->GetInstanceTypeId ().GetName ());
          if ((*app).second->GetInstanceTypeId ().GetName () == appName)
            {
              Ptr<CcnxSeqsAppTracer> trace = Create<CcnxSeqsAppTracer> (boost::ref(*m_appSeqsTrace),
                                                                        appName,
                                                                        *node,
                                                                        lexical_cast<string> (appId));
              m_appSeqs.push_back (trace);
            }
        }
      
    }

  if (m_appSeqs.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      m_appSeqs.front ()->PrintHeader (*m_appSeqsTrace);
      *m_appSeqsTrace << "\n";
    }
}

void
CcnxTraceHelper::EnableIpv4SeqsAppAll (const std::string &trace)
{
  NS_LOG_FUNCTION (this);
  m_ipv4AppSeqsTrace = new ofstream (trace.c_str (), ios::trunc);

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
          NS_LOG_DEBUG ("App: " << lexical_cast<string> (appId) << ", typeId: " << (*app).second->GetInstanceTypeId ().GetName ());
          if ((*app).second->GetInstanceTypeId ().GetName () == "ns3::PacketSink" ||
              (*app).second->GetInstanceTypeId ().GetName () == "ns3::BulkSendApplication")
            {
              Ptr<Ipv4SeqsAppTracer> trace = Create<Ipv4SeqsAppTracer> (boost::ref(*m_ipv4AppSeqsTrace),
                                                                        *node,
                                                                        lexical_cast<string> (appId));
              m_ipv4AppSeqs.push_back (trace);
            }
        }
      
    }

  if (m_ipv4AppSeqs.size () > 0)
    {
      m_ipv4AppSeqs.front ()->PrintHeader (*m_ipv4AppSeqsTrace);
      *m_ipv4AppSeqsTrace << "\n";
    }
}

void
CcnxTraceHelper::EnableWindowsAll (const std::string &windowTrace)
{
  NS_LOG_FUNCTION (this);
  m_windowsTrace = new ofstream (windowTrace.c_str (), ios::trunc);

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
          if ((*app).second->GetInstanceTypeId ().GetName () == "ns3::CcnxConsumerWindow")
            {
              Ptr<CcnxConsumerWindowTracer> trace = Create<CcnxConsumerWindowTracer> (boost::ref(*m_windowsTrace),
                                                                                      *node,
                                                                                      lexical_cast<string> (appId));
              m_windows.push_back (trace);
            }
        }
      
    }

  if (m_windows.size () > 0)
    {
      m_windows.front ()->PrintHeader (*m_windowsTrace);
      *m_windowsTrace << "\n";
    }
}

void
CcnxTraceHelper::TcpConnect (Ptr<Node> node)
{
  ObjectVectorValue sockets;
  node->GetObject<TcpL4Protocol> ()->GetAttribute ("SocketList", sockets);
  
  uint32_t sockId = 0;      
  for (ObjectVectorValue::Iterator socket = sockets.Begin ();
       socket != sockets.End ();
       socket++, sockId++)
    {
      // std::cout << "Node: " << node->GetId () << ", Socket " << sockId << "\n";
          
      Ptr<TcpCongestionWindowTracer> trace = Create<TcpCongestionWindowTracer> (boost::ref(*m_windowsTcpTrace),
                                                                                node,
                                                                                lexical_cast<string> (sockId));
      m_windowsTcp.push_back (trace);
    }
}

void
CcnxTraceHelper::EnableWindowsTcpAll (const std::string &windowTrace)
{
  NS_LOG_FUNCTION (this);
  m_windowsTcpTrace = new ofstream (windowTrace.c_str (), ios::trunc);

  WindowTracer::PrintHeader (*m_windowsTcpTrace);
  *m_windowsTcpTrace << "\n";
}

void
CcnxTraceHelper::EnablePathWeights (const std::string &pathWeights)
{
  NS_LOG_FUNCTION (this);
  m_pathWeightsTrace = new ofstream (pathWeights.c_str (), ios::trunc);

  CcnxPathWeightTracer::PrintHeader (*m_pathWeightsTrace);
  *m_pathWeightsTrace << "\n";

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      Ptr<CcnxPathWeightTracer> trace = Create<CcnxPathWeightTracer> (boost::ref(*m_pathWeightsTrace),
                                                                      *node);
      m_pathWeights.push_back (trace);
    }
}

} // namespace ns3
