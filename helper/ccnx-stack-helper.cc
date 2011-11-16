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
 *          Ilya Moiseenko <iliamo@cs.ucla.edu> 
 */

/**
 * \ingroup ccnx
 * \defgroup CcnxStackModel Ccnx Stack Model
 *
 * \section CcnxStackTracingModel Tracing in the Ccnx Stack
 *
 * The ccnx stack provides a number of trace sources in its various
 * protocol implementations.  These trace sources can be hooked using your own 
 * custom trace code, or you can use our helper functions in some cases to 
 * arrange for tracing to be enabled.
 *
 * \subsection CcnxStackCcnxTracingModel Tracing in Ccnx
 *
 * The Ccnx layer three protocol provides three trace hooks.  These are the 
 * "Tx" (ns3::CcnxL3Protocol::m_txTrace), "Rx" (ns3::CcnxL3Protocol::m_rxTrace) 
 * and "Drop" (ns3::CcnxL3Protocol::m_dropTrace) trace sources.
 *
 * The "Tx" trace is fired in a number of situations, all of which indicate that
 * a given packet is about to be sent down to a given ns3::CcnxFace.
 *
 * - \todo list Tx trace events
 *
 * The "Rx" trace is fired when a packet is passed from the device up to the
 * ns3::CcnxL3Protocol::Receive function.
 *
 * - In the receive function, the CcnxFaceList is iterated, and if the
 *   CcnxFace corresponding to the receiving device is found to be in the
 *   UP state, the trace is fired.
 *
 * The "Drop" trace is fired in any case where the packet is dropped (in both
 * the transmit and receive paths).
 *
 * - \todo list Drop trace events
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/names.h"
#include "ns3/packet-socket-factory.h"
#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/core-config.h"
#include "ns3/ccnx-forwarding-strategy.h"
#include "ns3/ccnx-net-device-face.h"
#include "ns3/ccnx-l3-protocol.h"
#include "ns3/ccnx-fib.h"
#include "ns3/node-list.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-routing-helper.h"
#include "ns3/ipv4-global-routing-ordered-nexthops.h"
#include "ns3/global-router-interface.h"
#include "ns3/ipv4-global-routing-helper.h"

#include "ccnx-face-container.h"
#include "ccnx-stack-helper.h"
#include "ccnx-forwarding-helper.h"

#include <limits>
#include <map>
#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("CcnxStackHelper");

namespace ns3 {

// Things are going to work differently here with respect to trace
// file handling than in most places because the Tx and Rx trace
// sources we are interested in are going to multiplex receive and
// transmit callbacks for all Ccnx and face pairs through one
// callback.  We want packets to or from each distinct pair to go to
// an individual file, so we have got to demultiplex the Ccnx and face
// pair into a corresponding Ptr<PcapFileWrapper> at the callback.
//
// A complication in this situation is that the trace sources are
// hooked on a protocol basis.  There is no trace source hooked by an
// Ccnx and face pair.  This means that if we naively proceed to hook,
// say, a drop trace for a given Ccnx with face 0, and then hook for
// Ccnx with face 1 we will hook the drop trace twice and get two
// callbacks per event.  What we need to do is to hook the event once,
// and that will result in a single callback per drop event, and the
// trace source will provide the face which we filter on in the trace
// sink.
// 
// This has got to continue to work properly after the helper has been
// destroyed; but must be cleaned up at the end of time to avoid
// leaks.  Global maps of protocol/face pairs to file objects seems to
// fit the bill.
//
typedef std::pair<Ptr<Ccnx>, uint32_t> FacePairCcnx; 
typedef std::map<FacePairCcnx, Ptr<PcapFileWrapper> > FaceFileMapCcnx;
typedef std::map<FacePairCcnx, Ptr<OutputStreamWrapper> > FaceStreamMapCcnx;

static FaceFileMapCcnx g_faceFileMapCcnx; /**< A mapping of Ccnx/face pairs to pcap files */
static FaceStreamMapCcnx g_faceStreamMapCcnx; /**< A mapping of Ccnx/face pairs to ascii streams */

    
CcnxStackHelper::CcnxStackHelper ()
    : m_forwardingHelper (Ccnx::NDN_FLOODING)
{
}
    
CcnxStackHelper::CcnxStackHelper (Ccnx::ForwardingStrategy strategy)
    : m_forwardingHelper (strategy)
{
}
    
CcnxStackHelper::~CcnxStackHelper ()
{
}

CcnxStackHelper::CcnxStackHelper (const CcnxStackHelper &o)
{
}

CcnxStackHelper &
CcnxStackHelper::operator = (const CcnxStackHelper &o)
{
  if (this == &o)
    {
      return *this;
    }
  return *this;
}

void 
CcnxStackHelper::SetForwardingStrategy (Ccnx::ForwardingStrategy strategy)
{
  CcnxForwardingHelper newForwardingHelper (strategy);
  m_forwardingHelper = newForwardingHelper;
}

Ptr<CcnxFaceContainer>
CcnxStackHelper::Install (NodeContainer c) const
{
  Ptr<CcnxFaceContainer> faces = Create<CcnxFaceContainer> ();
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      faces->AddAll (Install (*i));
    }
  return faces;
}

Ptr<CcnxFaceContainer>
CcnxStackHelper::InstallAll (void) const
{
  return Install (NodeContainer::GetGlobal ());
}

// void
// CcnxStackHelper::CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId)
// {
//   ObjectFactory factory;
//   factory.SetTypeId (typeId);
//   Ptr<Object> protocol = factory.Create <Object> ();
//   node->AggregateObject (protocol);
// }

Ptr<CcnxFaceContainer>
CcnxStackHelper::Install (Ptr<Node> node) const
{
  // NS_ASSERT_MSG (m_forwarding, "SetForwardingHelper() should be set prior calling Install() method");
  Ptr<CcnxFaceContainer> faces = Create<CcnxFaceContainer> ();
  
  if (node->GetObject<Ccnx> () != 0)
    {
      NS_FATAL_ERROR ("CcnxStackHelper::Install (): Installing " 
                      "a CcnxStack to a node with an existing Ccnx object");
      return 0;
    }

  Ptr<CcnxFib> fib = CreateObject<CcnxFib> ();
  node->AggregateObject (fib);

  Ptr<CcnxL3Protocol> ccnx = CreateObject<CcnxL3Protocol> ();
  node->AggregateObject (ccnx);

  Ptr<CcnxPit> pit = ccnx->GetPit();
  NS_LOG_INFO("NODE->GetNDevices()="<<node->GetNDevices());
    
  for (uint32_t index=0; index < node->GetNDevices (); index++)
    {
      Ptr<PointToPointNetDevice> device = DynamicCast<PointToPointNetDevice>(node->GetDevice(index));
      if(device == 0)
        continue;
        
      Ptr<CcnxNetDeviceFace> face = Create<CcnxNetDeviceFace> (node->GetDevice (index));
      face->SetNode (node);
      uint32_t __attribute__ ((unused)) face_id = ccnx->AddFace (face);
      NS_LOG_LOGIC ("Node " << node->GetId () << ": added CcnxNetDeviceFace as face #" << face_id);
      // Setup bucket filtering
      // Assume that we know average data packet size, and this size is equal default size
      // Set maximum buckets (averaging over 1 second)
      
      DataRateValue dataRate;
      device->GetAttribute ("DataRate", dataRate);
      NS_LOG_INFO("DataRate for this link is " << dataRate.Get());
      pit->maxBucketsPerFace[face->GetId()] = 0.1 * dataRate.Get().GetBitRate () / 8 /(NDN_DEFAULT_DATA_SIZE + sizeof(CcnxInterestHeader));
      NS_LOG_INFO("maxBucketsPerFace["<<face->GetId()<<"] = " << pit->maxBucketsPerFace[face->GetId()]); 
      pit->leakSize[face->GetId()] = 0.97 * NDN_INTEREST_RESET_PERIOD / SECOND * dataRate.Get().GetBitRate () / 8 / (NDN_DEFAULT_DATA_SIZE + sizeof(CcnxInterestHeader));
      NS_LOG_INFO("pit->leakSize["<<face->GetId()<<"] = " << pit->leakSize[face->GetId()]);
        
        
      if(face->IsLocal()==true)
        NS_LOG_INFO("Face #" << face_id << " is turned on");
      face->SetUp ();
      faces->Add (face);
    }
    
  m_forwardingHelper.SetForwarding (ccnx, pit);

  ccnx->ScheduleLeakage ();
    
  return faces;
}

Ptr<CcnxFaceContainer>
CcnxStackHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return Install (node);
}


void
CcnxStackHelper::AddRoute (Ptr<Node> node, std::string prefix, Ptr<CcnxFace> face, int32_t metric)
{
  Ptr<CcnxFib>  fib  = node->GetObject<CcnxFib> ();

  CcnxNameComponentsValue prefixValue;
  prefixValue.DeserializeFromString (prefix, MakeCcnxNameComponentsChecker ());
  fib->Add (prefixValue.Get (), face, metric);
}

void
CcnxStackHelper::AddRoute (std::string nodeName, std::string prefix, uint32_t faceId, int32_t metric)
{
  NS_LOG_LOGIC ("[" << nodeName << "]$ route add " << prefix << " via " << faceId << " metric " << metric);
  
  Ptr<Node> node = Names::Find<Node> (nodeName);
  NS_ASSERT_MSG (node != 0, "Node [" << nodeName << "] does not exist");
  
  Ptr<Ccnx>     ccnx = node->GetObject<Ccnx> ();
  NS_ASSERT_MSG (ccnx != 0, "Ccnx stack should be installed on the node");

  Ptr<CcnxFace> face = ccnx->GetFace (faceId);
  NS_ASSERT_MSG (face != 0, "Face with ID [" << faceId << "] does not exist on node [" << nodeName << "]");

  AddRoute (node, prefix, face, metric);
}
/*
void
CcnxStackHelper::AddRoute (Ptr<Node> node, std::string prefix, uint32_t faceId, int32_t metric)
{
    NS_LOG_LOGIC ("[" << nodeName << "]$ route add " << prefix << " via " << faceId << " metric " << metric);
    
  NS_ASSERT_MSG (node != 0, "Node does not exist");
        
  Ptr<Ccnx>     ccnx = node->GetObject<Ccnx> ();
  Ptr<CcnxFib>  fib  = node->GetObject<CcnxFib> ();
  Ptr<CcnxFace> face = ccnx->GetFace (faceId);
  NS_ASSERT_MSG (node != 0, "Face with ID [" << faceId << "] does not exist on node [" << nodeName << "]");
        
  CcnxNameComponentsValue prefixValue;
  prefixValue.DeserializeFromString (prefix, MakeCcnxNameComponentsChecker ());
  fib->Add (prefixValue.Get (), face, metric);
}
*/

static void
CcnxL3ProtocolRxTxSink (Ptr<const Packet> p, Ptr<Ccnx> ccnx, uint32_t face)
{
  NS_LOG_FUNCTION (p << ccnx << face);

  //
  // Since trace sources are independent of face, if we hook a source
  // on a particular protocol we will get traces for all of its faces.
  // We need to filter this to only report faces for which the user 
  // has expressed interest.
  //
  FacePairCcnx pair = std::make_pair (ccnx, face);
  if (g_faceFileMapCcnx.find (pair) == g_faceFileMapCcnx.end ())
    {
      NS_LOG_INFO ("Ignoring packet to/from face " << face);
      return;
    }

  Ptr<PcapFileWrapper> file = g_faceFileMapCcnx[pair];
  file->Write (Simulator::Now (), p);
}

bool
CcnxStackHelper::PcapHooked (Ptr<Ccnx> ccnx)
{
  for (FaceFileMapCcnx::const_iterator i = g_faceFileMapCcnx.begin (); 
       i != g_faceFileMapCcnx.end (); 
       ++i)
    {
      if ((*i).first.first == ccnx)
        {
          return true;
        }
    }
  return false;
}

void 
CcnxStackHelper::EnablePcapCcnxInternal (std::string prefix, Ptr<Ccnx> ccnx, uint32_t face, bool explicitFilename)
{
  NS_LOG_FUNCTION (prefix << ccnx << face);

  //
  // We have to create a file and a mapping from protocol/face to file 
  // irrespective of how many times we want to trace a particular protocol.
  //
  PcapHelper pcapHelper;

  std::string filename;
  if (explicitFilename)
    {
      filename = prefix;
    }
  else
    {
      filename = pcapHelper.GetFilenameFromInterfacePair (prefix, ccnx, face);
    }

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out, PcapHelper::DLT_RAW);

  //
  // However, we only hook the trace source once to avoid multiple trace sink
  // calls per event (connect is independent of face).
  //
  if (!PcapHooked (ccnx))
    {
      //
      // Ptr<Ccnx> is aggregated to node and CcnxL3Protocol is aggregated to 
      // node so we can get to CcnxL3Protocol through Ccnx.
      //
      Ptr<CcnxL3Protocol> ccnxL3Protocol = ccnx->GetObject<CcnxL3Protocol> ();
      NS_ASSERT_MSG (ccnxL3Protocol, "CcnxStackHelper::EnablePcapCcnxInternal(): "
                     "m_ccnxEnabled and ccnxL3Protocol inconsistent");

      bool result = ccnxL3Protocol->TraceConnectWithoutContext ("Tx", MakeCallback (&CcnxL3ProtocolRxTxSink));
      NS_ASSERT_MSG (result == true, "CcnxStackHelper::EnablePcapCcnxInternal():  "
                     "Unable to connect ccnxL3Protocol \"Tx\"");

      result = ccnxL3Protocol->TraceConnectWithoutContext ("Rx", MakeCallback (&CcnxL3ProtocolRxTxSink));
      NS_ASSERT_MSG (result == true, "CcnxStackHelper::EnablePcapCcnxInternal():  "
                     "Unable to connect ccnxL3Protocol \"Rx\"");
      // cast result to void, to suppress ‘result’ set but not used compiler-warning
      // for optimized builds
      (void) result;
    }

  g_faceFileMapCcnx[std::make_pair (ccnx, face)] = file;
}

static void
CcnxL3ProtocolDropSinkWithoutContext (
  Ptr<OutputStreamWrapper> stream,
  Ptr<const Packet> packet,
  CcnxL3Protocol::DropReason reason, 
  Ptr<Ccnx> ccnx, 
  uint32_t face)
{
  //
  // Since trace sources are independent of face, if we hook a source
  // on a particular protocol we will get traces for all of its faces.
  // We need to filter this to only report faces for which the user 
  // has expressed interest.
  //
  FacePairCcnx pair = std::make_pair (ccnx, face);
  if (g_faceStreamMapCcnx.find (pair) == g_faceStreamMapCcnx.end ())
    {
      NS_LOG_INFO ("Ignoring packet to/from face " << face);
      return;
    }

  *stream->GetStream () << "d " << Simulator::Now ().GetSeconds () << " " << *packet << std::endl;
}

static void
CcnxL3ProtocolDropSinkWithContext (
  Ptr<OutputStreamWrapper> stream,
  std::string context,
  Ptr<const Packet> packet,
  CcnxL3Protocol::DropReason reason, 
  Ptr<Ccnx> ccnx, 
  uint32_t face)
{
  //
  // Since trace sources are independent of face, if we hook a source
  // on a particular protocol we will get traces for all of its faces.
  // We need to filter this to only report faces for which the user 
  // has expressed interest.
  //
  FacePairCcnx pair = std::make_pair (ccnx, face);
  if (g_faceStreamMapCcnx.find (pair) == g_faceStreamMapCcnx.end ())
    {
      NS_LOG_INFO ("Ignoring packet to/from face " << face);
      return;
    }

  *stream->GetStream () << "d " << Simulator::Now ().GetSeconds () << " " << context << "(" << face << ") " 
                        << *packet << std::endl;
}

bool
CcnxStackHelper::AsciiHooked (Ptr<Ccnx> ccnx)
{
  for (  FaceStreamMapCcnx::const_iterator i = g_faceStreamMapCcnx.begin (); 
         i != g_faceStreamMapCcnx.end (); 
         ++i)
    {
      if ((*i).first.first == ccnx)
        {
          return true;
        }
    }
  return false;
}

void 
CcnxStackHelper::EnableAsciiCcnxInternal (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  Ptr<Ccnx> ccnx, 
  uint32_t face,
  bool explicitFilename)
{
  //
  // Our trace sinks are going to use packet printing, so we have to 
  // make sure that is turned on.
  //
  Packet::EnablePrinting ();

  //
  // If we are not provided an OutputStreamWrapper, we are expected to create 
  // one using the usual trace filename conventions and hook WithoutContext
  // since there will be one file per context and therefore the context would
  // be redundant.
  //
  if (stream == 0)
    {
      //
      // Set up an output stream object to deal with private ofstream copy 
      // constructor and lifetime issues.  Let the helper decide the actual
      // name of the file given the prefix.
      //
      // We have to create a stream and a mapping from protocol/face to 
      // stream irrespective of how many times we want to trace a particular 
      // protocol.
      //
      AsciiTraceHelper asciiTraceHelper;

      std::string filename;
      if (explicitFilename)
        {
          filename = prefix;
        }
      else
        {
          filename = asciiTraceHelper.GetFilenameFromInterfacePair (prefix, ccnx, face);
        }

      Ptr<OutputStreamWrapper> theStream = asciiTraceHelper.CreateFileStream (filename);

      //
      // However, we only hook the trace sources once to avoid multiple trace sink
      // calls per event (connect is independent of face).
      //
      if (!AsciiHooked (ccnx))
        {
          //
          // The drop sink for the CcnxL3Protocol uses a different signature than
          // the default sink, so we have to cook one up for ourselves.  We can get
          // to the Ptr<CcnxL3Protocol> through our Ptr<Ccnx> since they must both 
          // be aggregated to the same node.
          //
          Ptr<CcnxL3Protocol> ccnxL3Protocol = ccnx->GetObject<CcnxL3Protocol> ();
          bool __attribute__ ((unused)) result = ccnxL3Protocol->TraceConnectWithoutContext ("Drop", 
                                                                                             MakeBoundCallback (&CcnxL3ProtocolDropSinkWithoutContext, theStream));
          NS_ASSERT_MSG (result == true, "CcnxStackHelper::EanableAsciiCcnxInternal():  "
                         "Unable to connect ccnxL3Protocol \"Drop\"");
        }

      g_faceStreamMapCcnx[std::make_pair (ccnx, face)] = theStream;
      return;
    }

  //
  // If we are provided an OutputStreamWrapper, we are expected to use it, and
  // to provide a context.  We are free to come up with our own context if we
  // want, and use the AsciiTraceHelper Hook*WithContext functions, but for 
  // compatibility and simplicity, we just use Config::Connect and let it deal
  // with the context.
  //
  // We need to associate the ccnx/face with a stream to express interest
  // in tracing events on that pair, however, we only hook the trace sources 
  // once to avoid multiple trace sink calls per event (connect is independent
  // of face).
  //
  if (!AsciiHooked (ccnx))
    {
      Ptr<Node> node = ccnx->GetObject<Node> ();
      std::ostringstream oss;

      //
      // This has all kinds of parameters coming with, so we have to cook up our
      // own sink.
      //
      oss.str ("");
      oss << "/NodeList/" << node->GetId () << "/$ns3::CcnxL3Protocol/Drop";
      Config::Connect (oss.str (), MakeBoundCallback (&CcnxL3ProtocolDropSinkWithContext, stream));
    }

  g_faceStreamMapCcnx[std::make_pair (ccnx, face)] = stream;
}

void
CcnxStackHelper::InstallFakeGlobalRoutes ()
{
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node ++)
    {
      NS_ASSERT_MSG ((*node)->GetObject<Ipv4> () != 0,
                     "InternetStack should be installed on all nodes");

      NS_ASSERT_MSG (Ipv4RoutingHelper::GetRouting<Ipv4GlobalRoutingOrderedNexthops>
                     (
                      (*node)->GetObject<Ipv4> ()->GetRoutingProtocol ()
                     ),
                     "InternetStack should have Ipv4GlobalRoutingOrderedNexthops as routing protocol");
      // Example:
      //
      // Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingUnorderedNexthops");
      // stack.SetRoutingHelper (ipv4RoutingHelper);
      //
      
      Ptr<GlobalRouter> globalRouter = (*node)->GetObject<GlobalRouter> ();
      if (globalRouter == 0) continue;

      globalRouter->InjectRoute (Ipv4Address((*node)->GetId ()), Ipv4Mask("255.255.255.255"));
    }

  Ipv4GlobalRoutingHelper::PopulateAllPossibleRoutingTables ();
}

void
CcnxStackHelper::InstallRouteTo (Ptr<Node> destNode)
{
  std::ostringstream destPrefix;
  destPrefix << "/" << destNode->GetId ();

  Ipv4Address destIpv4 = Ipv4Address(destNode->GetId ());
  
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node ++)
    {
      if (destNode == *node) continue;
      
      Ptr<Ccnx> ccnx = (*node)->GetObject<Ccnx> ();
      NS_ASSERT_MSG (ccnx != 0, "CCNx stack should be installed on all nodes");

      Ptr<Ipv4> ipv4 = (*node)->GetObject<Ipv4> ();
      NS_ASSERT_MSG (ipv4 != 0,
                     "InternetStack should be installed on all nodes");
      
      Ptr<Ipv4GlobalRoutingOrderedNexthops> routing =
        Ipv4RoutingHelper::GetRouting<Ipv4GlobalRoutingOrderedNexthops> (ipv4->GetRoutingProtocol ());
      NS_ASSERT_MSG (routing != 0, "Ipv4GlobalRoutingOrderedNexthops should be used in InternetStack");

      Ptr<Ipv4GlobalRoutingOrderedNexthops::EntryContainer>
        routes = routing->Lookup (destIpv4);

      NS_ASSERT_MSG (routes != 0, "Should not happen... Call the developer");

      BOOST_FOREACH (const Ipv4RoutingTableEntry &entry, *routes)
        {
          Ptr<NetDevice> netDevice = ipv4->GetNetDevice (entry.GetInterface ());
          NS_ASSERT_MSG (netDevice != 0, "Should never happen. Call the popos");
          
          Ptr<CcnxFace> face = ccnx->GetFaceByNetDevice (netDevice);
          NS_ASSERT_MSG (face != 0, "Definitely should never happen. Call the president");
            
          AddRoute (*node, destPrefix.str(), face, entry.GetMetric ());
        }
    }
}

void
CcnxStackHelper::InstallRoutesToAll ()
{
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node ++)
    {
      InstallRouteTo (*node);
    }
}


} // namespace ns3
