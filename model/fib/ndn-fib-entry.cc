/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 */

#include "ndn-fib-entry.h"

#include "ns3/ndn-name-components.h"
#include "ns3/log.h"

#define NDN_RTO_ALPHA 0.125
#define NDN_RTO_BETA 0.25
#define NDN_RTO_K 4

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("NdnFibEntry");

namespace ns3
{

//////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////
namespace __ndn_private {

struct NdnFibFaceMetricByFace
{
  typedef NdnFibFaceMetricContainer::type::index<i_face>::type
  type;
};

}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

using namespace __ndn_private;

void
NdnFibFaceMetric::UpdateRtt (const Time &rttSample)
{
  // const Time & this->m_rttSample
  
  //update srtt and rttvar (RFC 2988)
  if (m_sRtt.IsZero ())
    {
      //first RTT measurement
      NS_ASSERT_MSG (m_rttVar.IsZero (), "SRTT is zero, but variation is not");
      
      m_sRtt = rttSample;
      m_rttVar = Time (m_sRtt / 2.0);
    }
  else
    {
      m_rttVar = Time ((1 - NDN_RTO_BETA) * m_rttVar + NDN_RTO_BETA * Abs(m_sRtt - rttSample));
      m_sRtt = Time ((1 - NDN_RTO_ALPHA) * m_sRtt + NDN_RTO_ALPHA * rttSample);
    }
}

/////////////////////////////////////////////////////////////////////

void
NdnFibEntry::UpdateFaceRtt (Ptr<NdnFace> face, const Time &sample)
{
  NdnFibFaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
  NS_ASSERT_MSG (record != m_faces.get<i_face> ().end (),
                 "Update status can be performed only on existing faces of CcxnFibEntry");

  m_faces.modify (record,
                  ll::bind (&NdnFibFaceMetric::UpdateRtt, ll::_1, sample));

  // reordering random access index same way as by metric index
  m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

void
NdnFibEntry::UpdateStatus (Ptr<NdnFace> face, NdnFibFaceMetric::Status status)
{
  NS_LOG_FUNCTION (this << boost::cref(*face) << status);

  NdnFibFaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
  NS_ASSERT_MSG (record != m_faces.get<i_face> ().end (),
                 "Update status can be performed only on existing faces of CcxnFibEntry");

  m_faces.modify (record,
                  (&ll::_1)->*&NdnFibFaceMetric::m_status = status);

  // reordering random access index same way as by metric index
  m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

void
NdnFibEntry::AddOrUpdateRoutingMetric (Ptr<NdnFace> face, int32_t metric)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (face != NULL, "Trying to Add or Update NULL face");

  NdnFibFaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
  if (record == m_faces.get<i_face> ().end ())
    {
      m_faces.insert (NdnFibFaceMetric (face, metric));
    }
  else
  {
    // don't update metric to higher value
    if (record->m_routingCost > metric || record->m_status == NdnFibFaceMetric::NDN_FIB_RED)
      {
        m_faces.modify (record,
                        (&ll::_1)->*&NdnFibFaceMetric::m_routingCost = metric);

        m_faces.modify (record,
                        (&ll::_1)->*&NdnFibFaceMetric::m_status = NdnFibFaceMetric::NDN_FIB_YELLOW);
      }
  }
  
  // reordering random access index same way as by metric index
  m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

void
NdnFibEntry::Invalidate ()
{
  for (NdnFibFaceMetricByFace::type::iterator face = m_faces.begin ();
       face != m_faces.end ();
       face++)
    {
      m_faces.modify (face,
                      (&ll::_1)->*&NdnFibFaceMetric::m_routingCost = std::numeric_limits<uint16_t>::max ());

      m_faces.modify (face,
                      (&ll::_1)->*&NdnFibFaceMetric::m_status = NdnFibFaceMetric::NDN_FIB_RED);
    }
}

const NdnFibFaceMetric &
NdnFibEntry::FindBestCandidate (uint32_t skip/* = 0*/) const
{
  if (m_faces.size () == 0) throw NdnFibEntry::NoFaces ();
  skip = skip % m_faces.size();
  return m_faces.get<i_nth> () [skip];
}

std::ostream& operator<< (std::ostream& os, const NdnFibEntry &entry)
{
  for (NdnFibFaceMetricContainer::type::index<i_nth>::type::iterator metric =
         entry.m_faces.get<i_nth> ().begin ();
       metric != entry.m_faces.get<i_nth> ().end ();
       metric++)
    {
      if (metric != entry.m_faces.get<i_nth> ().begin ())
        os << ", ";

      os << *metric;
    }
  return os;
}

std::ostream& operator<< (std::ostream& os, const NdnFibFaceMetric &metric)
{
  static const std::string statusString[] = {"","g","y","r"};

  os << *metric.m_face << "(" << metric.m_routingCost << ","<< statusString [metric.m_status] << "," << metric.m_face->GetMetric () << ")";
  return os;
}


} // ns3
