/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 *
 */

#include "ndn-face.h"

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"

// #include "ns3/weights-path-stretch-tag.h"

#include <boost/ref.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.Face");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (Face);

TypeId
Face::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::Face")
    .SetParent<Object> ()
    .SetGroupName ("Ndn")
    .AddAttribute ("Id", "Face id (unique integer for the Ndn stack on this node)",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (0),
                   MakeUintegerAccessor (&Face::m_id),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("BucketMax", "Maximum size of leaky bucket",
                   DoubleValue (-1.0),
                   MakeDoubleAccessor (&Face::m_bucketMax),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("BucketLeak", "Normalized bucket leak size",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&Face::m_bucketLeak),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("RandomizeLimitChecking", "Whether or not to randomize the limit checking procedure. false (persistent) by default",
                   BooleanValue (false),
                   MakeBooleanAccessor (&Face::m_randomizeLimitChecking),
                   MakeBooleanChecker ())
                   
    // .AddAttribute ("MetricTagging", "Enable metric tagging (path-stretch calculation)",
    //                BooleanValue (false),
    //                MakeBooleanAccessor (&Face::m_enableMetricTagging),
    //                MakeBooleanChecker ())

    .AddTraceSource ("NdnTx", "Transmitted packet trace",
                     MakeTraceSourceAccessor (&Face::m_txTrace))
    .AddTraceSource ("NdnRx", "Received packet trace",
                     MakeTraceSourceAccessor (&Face::m_rxTrace))
    .AddTraceSource ("NdnDrop", "Dropped packet trace",
                     MakeTraceSourceAccessor (&Face::m_dropTrace))
    ;
  return tid;
}

/** 
 * By default, Ndn face are created in the "down" state
 *  with no IP addresses.  Before becoming useable, the user must 
 * invoke SetUp on them once an Ndn address and mask have been set.
 */
Face::Face (Ptr<Node> node) 
  : m_node (node)
  , m_bucket (0.0)
  , m_bucketMax (-1.0)
  , m_bucketLeak (0.0)
  , m_protocolHandler (MakeNullCallback<void,const Ptr<Face>&,const Ptr<const Packet>&> ())
  , m_ifup (false)
  , m_id ((uint32_t)-1)
  , m_lastLeakTime (0)
  , m_metric (0)
  // , m_enableMetricTagging (false)
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT_MSG (node != 0, "node cannot be NULL. Check the code");
}

Face::~Face ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Face::Face (const Face &)
{
}

Face& Face::operator= (const Face &)
{
  return *this;
}

Ptr<Node>
Face::GetNode () const
{
  return m_node;
}

void
Face::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_protocolHandler = handler;
}

bool
Face::IsBelowLimit ()
{
  NS_LOG_FUNCTION_NOARGS ();

  /// \todo Implement tracing, if requested
  
  if (!IsUp ()){
    NS_LOG_INFO("Face is not up.");
    return false;
  }

  LeakBucket ();
  
  if (m_bucketMax > 0)
    {
      NS_LOG_DEBUG ("Limits enabled: " << m_bucketMax << ", current: " << m_bucket);
      if (m_bucket+1.0 > m_bucketMax)
        {
          //NS_LOG_DEBUG ("Returning false");
          return false;
        }

      if (m_randomizeLimitChecking)
        {
          static NormalVariable acceptanceProbability (m_bucketMax, m_bucketMax/6.0, m_bucketMax/2.0);
          // static UniformVariable acceptanceProbability (0, m_bucketMax);
          double value = acceptanceProbability.GetValue ();
          if (value > m_bucketMax)
            value -= m_bucketMax;
      
          if (m_bucket < value)
            {
              m_bucket += 1.0;
              return true;
            }
          else
            return false;
        }
      else
        {
          m_bucket += 1.0;
          return true;
        }
    }
  
  return true;
}

bool
Face::Send (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (boost::cref (*this) << packet << packet->GetSize ());
  NS_LOG_DEBUG (*packet);

  if (!IsUp ())
    {
      m_dropTrace (packet);
      return false;
    }

  // if (m_enableMetricTagging)
  //   {
  //     // update path information
  //     Ptr<const WeightsPathStretchTag> origTag = packet->RemovePacketTag<WeightsPathStretchTag> ();
  //     Ptr<WeightsPathStretchTag> tag;
  //     if (origTag == 0)
  //       {
  //         tag = CreateObject<WeightsPathStretchTag> (); // create a new tag
  //       }
  //     else
  //       {
  //         tag = CreateObject<WeightsPathStretchTag> (*origTag); // will update existing tag
  //       }

  //     tag->AddPathInfo (m_node, GetMetric ());
  //     packet->AddPacketTag (tag);
  //   }

  bool ok = SendImpl (packet);
  if (ok)
    {
      m_txTrace (packet);
      return true;
    }
  else
    {
      m_dropTrace (packet);
      return false;
    }
}

bool
Face::Receive (const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (boost::cref (*this) << packet << packet->GetSize ());

  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  m_rxTrace (packet);
  m_protocolHandler (this, packet);
  
  return true;
}

void
Face::LeakBucket ()
{
  if (m_lastLeakTime.IsZero ())
    {
      m_lastLeakTime = Simulator::Now ();
      return;
    }

  Time interval = Simulator::Now () - m_lastLeakTime;
  const double leak = m_bucketLeak * interval.ToDouble (Time::S);
  if (leak >= 1.0)
    {
      m_bucket = std::max (0.0, m_bucket - leak);
      m_lastLeakTime = Simulator::Now ();
    }

  // NS_LOG_DEBUG ("max: " << m_bucketMax << ", Current bucket: " << m_bucket << ", leak size: " << leak << ", interval: " << interval << ", " << m_bucketLeak);
}

void
Face::SetBucketMax (double bucket)
{
  NS_LOG_FUNCTION (this << bucket);
  m_bucketMax = bucket;
}

void
Face::SetBucketLeak (double leak)
{
  NS_LOG_FUNCTION (this << leak);
  m_bucketLeak = leak;
}

void
Face::SetMetric (uint16_t metric)
{
  NS_LOG_FUNCTION (metric);
  m_metric = metric;
}

uint16_t
Face::GetMetric (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_metric;
}

/**
 * These are face states and may be distinct from 
 * NetDevice states, such as found in real implementations
 * (where the device may be down but face state is still up).
 */

bool 
Face::IsUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifup;
}

void 
Face::SetUp (bool up/* = true*/)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifup = up;
}

bool
Face::operator== (const Face &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (),
                 "Faces of different nodes should not be compared to each other");

  return (m_id == face.m_id);
}

bool
Face::operator< (const Face &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (),
                 "Faces of different nodes should not be compared to each other");

  return (m_id < face.m_id);
}

std::ostream&
Face::Print (std::ostream &os) const
{
  os << "id=" << GetId ();
  return os;
}

std::ostream&
operator<< (std::ostream& os, const Face &face)
{
  face.Print (os);
  return os;
}

} // namespace ndn
} // namespace ns3

