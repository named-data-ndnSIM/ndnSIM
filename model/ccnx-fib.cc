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

#include "ccnx-fib.h"

#include "ccnx.h"
#include "ccnx-face.h"
#include "ccnx-interest-header.h"

#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/log.h"

#define NDN_RTO_ALPHA 0.125
#define NDN_RTO_BETA 0.25
#define NDN_RTO_K 4

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

namespace ns3 {


//////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////
namespace __ccnx_private {

struct CcnxFibFaceMetricByFace
{
  typedef CcnxFibFaceMetricContainer::type::index<i_face>::type
  type;
};

}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
NS_LOG_COMPONENT_DEFINE ("CcnxFib");
    
using namespace __ccnx_private;

TypeId 
CcnxFib::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxFib")
    .SetParent<Object> ()
    .SetGroupName ("Ccnx")
    .AddConstructor<CcnxFib> ()

  ;
  return tid;
}

/////////////////////////////////////////////////////////////////////

void
CcnxFibFaceMetric::UpdateRtt (const Time &rttSample)
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
CcnxFibEntry::UpdateFaceRtt (Ptr<CcnxFace> face, const Time &sample)
{
  CcnxFibFaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
  NS_ASSERT_MSG (record != m_faces.get<i_face> ().end (),
                 "Update status can be performed only on existing faces of CcxnFibEntry");

  m_faces.modify (record,
                  ll::bind (&CcnxFibFaceMetric::UpdateRtt, ll::_1, sample));

  // reordering random access index same way as by metric index
  m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

void
CcnxFibEntry::UpdateStatus (Ptr<CcnxFace> face, CcnxFibFaceMetric::Status status)
{
  NS_LOG_FUNCTION (this << boost::cref(*face) << status);

  CcnxFibFaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
  NS_ASSERT_MSG (record != m_faces.get<i_face> ().end (),
                 "Update status can be performed only on existing faces of CcxnFibEntry");

  m_faces.modify (record,
                  (&ll::_1)->*&CcnxFibFaceMetric::m_status = status);

  // reordering random access index same way as by metric index
  m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

void
CcnxFibEntry::AddOrUpdateRoutingMetric (Ptr<CcnxFace> face, int32_t metric)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (face != NULL, "Trying to Add or Update NULL face");

  CcnxFibFaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
  if (record == m_faces.get<i_face> ().end ())
    {
      m_faces.insert (CcnxFibFaceMetric (face, metric));
    }
  else
  {
    m_faces.modify (record,
                    (&ll::_1)->*&CcnxFibFaceMetric::m_routingCost = metric);
  }
  
  // reordering random access index same way as by metric index
  m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

const CcnxFibFaceMetric &
CcnxFibEntry::FindBestCandidate (uint32_t skip/* = 0*/) const
{
  if (m_faces.size () == 0) throw CcnxFibEntry::NoFaces ();
  skip = skip % m_faces.size();
  return m_faces.get<i_nth> () [skip];
}


CcnxFib::CcnxFib ()
{
}

void
CcnxFib::NotifyNewAggregate ()
{
  if (m_node == 0)
      m_node = this->GetObject<Node>();
  Object::NotifyNewAggregate ();
}

void 
CcnxFib::DoDispose (void)
{
  m_fib.clear ();
  m_node = 0;
  Object::DoDispose ();
}


CcnxFibEntryContainer::type::iterator
CcnxFib::LongestPrefixMatch (const CcnxInterestHeader &interest) const
{
  const CcnxNameComponents &name = interest.GetName ();
  for (size_t componentsCount = name.GetComponents ().size ()+1;
       componentsCount > 0;
       componentsCount--)
    {
      CcnxNameComponents subPrefix (name.GetSubComponents (componentsCount-1));
      CcnxFibEntryContainer::type::iterator match = m_fib.find (subPrefix);
      if (match != m_fib.end())
        return match;
    }
  
  return m_fib.end ();
}


CcnxFibEntryContainer::type::iterator
CcnxFib::Add (const CcnxNameComponents &prefix, Ptr<CcnxFace> face, int32_t metric)
{
// CcnxFibFaceMetric
  NS_LOG_FUNCTION(this << prefix << face << metric);
  CcnxFibEntryContainer::type::iterator entry = m_fib.find (prefix);
  if (entry == m_fib.end ())
    {
      entry = m_fib.insert (m_fib.end (), CcnxFibEntry (prefix));
      // insert new
    }

  NS_ASSERT_MSG (face != NULL, "Trying to modify NULL face");
  m_fib.modify (entry,
                ll::bind (&CcnxFibEntry::AddOrUpdateRoutingMetric, ll::_1, face, metric));
    
  return entry;
}
    
void
CcnxFib::Remove (const CcnxFibEntry &entry, Ptr<CcnxFace> face)
{
  NS_LOG_FUNCTION (this);

  m_fib.modify (m_fib.iterator_to (entry),
                ll::bind (&CcnxFibEntry::RemoveFace, ll::_1, face));
  if (entry.m_faces.size () == 0)
    {
      m_fib.erase (m_fib.iterator_to (entry));
    }
}

void
CcnxFib::RemoveFromAll (Ptr<CcnxFace> face)
{
  NS_LOG_FUNCTION (this);

  for_each (m_fib.begin (), m_fib.end (), 
            ll::bind (&CcnxFib::Remove, this, ll::_1, face));
}

/**
 * \brief Get number of FIB entry (for python bindings)
 */
uint32_t 
CcnxFib::GetCcnxFibEntryCount () const
{
  return m_fib.size ();
}

/**
 * \brief Get FIB entry by index (for python bindings)
 */
const CcnxFibEntry &
CcnxFib::GetCcnxFibEntry (uint32_t index)
{
  NS_ASSERT (0 <= index && index < m_fib.size ());
  return m_fib.get <i_nth> () [index];
}


std::ostream& operator<< (std::ostream& os, const CcnxFib &fib)
{
  os << "Node " << Names::FindName (fib.m_node) << "\n";
  os << "  Dest prefix      Interfaces(Costs)                  \n";
  os << "+-------------+--------------------------------------+\n";
  
  for (CcnxFibEntryContainer::type::iterator entry = fib.m_fib.begin ();
       entry != fib.m_fib.end ();
       entry++)
    {
      os << entry->GetPrefix () << "\t" << *entry << "\n";
    }
  return os;
}

std::ostream& operator<< (std::ostream& os, const CcnxFibEntry &entry)
{
  for (CcnxFibFaceMetricContainer::type::index<i_nth>::type::iterator metric =
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

std::ostream& operator<< (std::ostream& os, const CcnxFibFaceMetric &metric)
{
  static const std::string statusString[] = {"","g","y","r"};

  os << *metric.m_face << "(" << metric.m_routingCost << ","<< statusString [metric.m_status] << ")";
  return os;
}

// void CcnxFib::resetProbing()
// {
//     for(FibRangeIterator fib = _fib.begin(); fib != _fib.end(); fib++)
//         VALUE(fib).needsProbing = true;
// }

// void CcnxFib::updateInterfaceStatus( int interface, int status )
// {
// 	for( FibRangeIterator fib = _fib.begin(); fib!=_fib.end(); fib++ )
// 	{
// 		VALUE(fib).updateStatus( interface, status );
// 	}
// }

} // namespace ns3
