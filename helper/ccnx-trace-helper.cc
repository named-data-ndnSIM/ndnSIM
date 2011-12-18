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
#include "ns3/node-list.h"
#include "ns3/object-vector.h"

#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("CcnxTraceHelper");

namespace ns3 {
    
CcnxAppTracer::CcnxAppTracer (const std::string &app, const std::string &node, const std::string &appId)
  : m_app (app)
  , m_appId (appId)
  , m_node (node)
{
  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+appId+"/$"+m_app+"/TransmittedInterests",
                   MakeCallback (&CcnxAppTracer::OutInterests, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+appId+"/$"+m_app+"/ReceivedNacks",
                   MakeCallback (&CcnxAppTracer::InNacks, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+appId+"/$"+m_app+"/ReceivedInterests",
                   MakeCallback (&CcnxAppTracer::InInterests, this));
  
  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+appId+"/$"+m_app+"/TransmittedContentObjects",
                   MakeCallback (&CcnxAppTracer::OutData, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+appId+"/$"+m_app+"/ReceivedContentObjects",
                   MakeCallback (&CcnxAppTracer::InData, this));
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

CcnxL3Tracer::CcnxL3Tracer (const std::string &node)
: m_node (node)
{
  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/ForwardingStrategy/OutInterests",
                   MakeCallback (&CcnxL3Tracer::OutInterests, this));
  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/InInterests",
                   MakeCallback (&CcnxL3Tracer::InInterests, this));
  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/DropInterests",
                   MakeCallback (&CcnxL3Tracer::DropInterests, this));

  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/OutNacks",
                   MakeCallback (&CcnxL3Tracer::OutNacks, this));
  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/InNacks",
                   MakeCallback (&CcnxL3Tracer::InNacks, this));
  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/DropNacks",
                   MakeCallback (&CcnxL3Tracer::DropNacks, this));

  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/OutData",
                   MakeCallback (&CcnxL3Tracer::OutData, this));
  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/InData",
                   MakeCallback (&CcnxL3Tracer::InData, this));
  Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/DropData",
                   MakeCallback (&CcnxL3Tracer::DropData, this));
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

CcnxAggregateAppTracer::CcnxAggregateAppTracer (const std::string &app, const std::string &node, const std::string &appId)
  : CcnxAppTracer (app, node, appId)
  , m_inInterests (0)
  , m_outInterests (0)
  , m_inNacks (0)
  , m_inData (0)
  , m_outData (0)
{
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
     << "OutData";
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
     << m_outData;
}

void
CcnxAggregateAppTracer::OutInterests (std::string context,
                                      Ptr<const CcnxInterestHeader>, Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_outInterests++;
}

void
CcnxAggregateAppTracer::OutData (std::string context,
                                 Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                                 Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_outData++;
}

void
CcnxAggregateAppTracer::InInterests (std::string context,
                                     Ptr<const CcnxInterestHeader>,
                                     Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inInterests++;
}

void
CcnxAggregateAppTracer::InNacks (std::string context,
                                 Ptr<const CcnxInterestHeader>,
                                 Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inNacks++;
}
  
void
CcnxAggregateAppTracer::InData (std::string context,
                                Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                                Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inData++;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


CcnxAggregateL3Tracer::CcnxAggregateL3Tracer (const std::string &node)
  : CcnxL3Tracer (node)
  , m_inInterests (0)
  , m_outInterests (0)
  , m_dropInterests (0)
  , m_inNacks (0)
  , m_outNacks (0)
  , m_dropNacks (0)
  , m_inData (0)
  , m_outData (0)
  , m_dropData (0)
{
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
     << "DropData";
}

void
CcnxAggregateL3Tracer::Print (std::ostream &os) const
{
  os << m_node << "\t"
     << m_inInterests   << "\t"
     << m_outInterests  << "\t"
     << m_dropInterests << "\t"
  
     << m_inNacks   << "\t"
     << m_outNacks  << "\t"
     << m_dropNacks << "\t"
  
     << m_inData   << "\t"
     << m_outData  << "\t"
     << m_dropData;
}

void
CcnxAggregateL3Tracer::OutInterests  (std::string context,
                                      Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>)
{
  m_outInterests++;
}

void
CcnxAggregateL3Tracer::InInterests   (std::string context,
                                      Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>)
{
  m_inInterests++;
}

void
CcnxAggregateL3Tracer::DropInterests (std::string context,
                                      Ptr<const CcnxInterestHeader>, Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_dropInterests++;
}

void
CcnxAggregateL3Tracer::OutNacks  (std::string context,
                                  Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>)
{
  m_outNacks++;
}

void
CcnxAggregateL3Tracer::InNacks   (std::string context,
                                  Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>)
{
  m_inNacks++;
}

void
CcnxAggregateL3Tracer::DropNacks (std::string context,
                                  Ptr<const CcnxInterestHeader>, Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_dropNacks++;
}

void
CcnxAggregateL3Tracer::OutData  (std::string context,
                                 Ptr<const CcnxContentObjectHeader>, bool fromCache, Ptr<const CcnxFace>)
{
  m_inData++;
}

void
CcnxAggregateL3Tracer::InData   (std::string context,
                                 Ptr<const CcnxContentObjectHeader>, Ptr<const CcnxFace>)
{
  m_outData++;
}

void
CcnxAggregateL3Tracer::DropData (std::string context,
                                 Ptr<const CcnxContentObjectHeader>, Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_dropData++;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

CcnxTraceHelper::~CcnxTraceHelper ()
{
  NS_LOG_FUNCTION (this);
  
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
                                                                lexical_cast<string> ((*node)->GetId ()),
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
      
      m_l3s.push_back (Create<CcnxAggregateL3Tracer> (lexical_cast<string> ((*node)->GetId ())));
    }
}


} // namespace ns3
