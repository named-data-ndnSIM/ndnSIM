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

//#define NDN_DEBUG_OSPF	0
//#define NDN_DEBUG_OSPF_NODES 0

//#define NDN_DUMP_FIB		0
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

struct ChangeStatus
{
  ChangeStatus (CcnxFibFaceMetric::Status status) : m_status (status) { }
  void operator() (CcnxFibFaceMetric &entry)
  {
    entry.m_status = m_status;
  }
private:
  CcnxFibFaceMetric::Status m_status;
};

struct ChangeMetric
{
  ChangeMetric (int32_t metric) : m_metric (metric) { }
  void operator() (CcnxFibFaceMetric &entry)
  {
    entry.m_routingCost = m_metric;
  }
private:
  int32_t m_metric;
};

// struct SearchByFace {
//   /**
//    * \brief To perform effective searches by CcnxFace
//    */
//   bool
//   operator() (const CcnxFibFaceMetric &m, const Ptr<CcnxFace> &face) const
//   {
//     return *(m.m_face) < *face;
//   } 

//   /**
//    * \brief To perform effective searches by CcnxFace
//    */
//   bool
//   operator() (const Ptr<CcnxFace> &face, const CcnxFibFaceMetric &m) const
//   {
//     return *face < *(m.m_face);
//   } 
// };

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

void
CcnxFibFaceMetric::UpdateRtt::operator() (CcnxFibFaceMetric &entry)
{
  // const Time & this->m_rttSample
  
  //update srtt and rttvar (RFC 2988)
  if (entry.m_sRtt.IsZero ())
    {
      //first RTT measurement
      NS_ASSERT_MSG (entry.m_rttVar.IsZero (), "SRTT is zero, but variation is not");
      
      entry.m_sRtt = m_rttSample;
      entry.m_rttVar = Time (entry.m_sRtt / 2.0);
    }
  else
    {
      entry.m_rttVar = Time ((1 - NDN_RTO_BETA) * entry.m_rttVar + NDN_RTO_BETA * Abs(entry.m_sRtt - m_rttSample));
      entry.m_sRtt = Time ((1 - NDN_RTO_ALPHA) * entry.m_sRtt + NDN_RTO_ALPHA * m_rttSample);
    }
}

void
CcnxFibEntry::UpdateStatus::operator () (CcnxFibEntry &entry)
{
  CcnxFibFaceMetricByFace::type::iterator record = entry.m_faces.get<i_face> ().find (m_face);
  NS_ASSERT_MSG (record != entry.m_faces.get<i_face> ().end (),
                 "Update status can be performed only on existing faces of CcxnFibEntry");

  entry.m_faces.modify (record, ChangeStatus (m_status));

  // reordering random access index same way as by metric index
  entry.m_faces.get<i_nth> ().rearrange (entry.m_faces.get<i_metric> ().begin ());
}

void
CcnxFibEntry::AddOrUpdateRoutingMetric::operator () (CcnxFibEntry &entry)
{
  NS_LOG_FUNCTION(this);
  NS_ASSERT_MSG (m_face != NULL, "Trying to Add or Update NULL face");

  CcnxFibFaceMetricByFace::type::iterator record = entry.m_faces.get<i_face> ().find (m_face);
  if (record == entry.m_faces.get<i_face> ().end ())
    {
      entry.m_faces.insert (CcnxFibFaceMetric (m_face, m_metric));
    }
  else
  {
      entry.m_faces.modify (record, ChangeMetric (m_metric));
    }
  // reordering random access index same way as by metric index
  entry.m_faces.get<i_nth> ().rearrange (entry.m_faces.get<i_metric> ().begin ());
}

void
CcnxFibEntry::UpdateFaceRtt::operator() (CcnxFibEntry &entry)
{
  CcnxFibFaceMetricContainer::type::iterator metric = entry.m_faces.find (m_face);
  NS_ASSERT_MSG (metric != entry.m_faces.end (),
                 "Something wrong. Cannot find entry for the face in FIB");

  entry.m_faces.modify (metric, CcnxFibFaceMetric::UpdateRtt (m_rttSample));
}
    
Ptr<CcnxFace>
CcnxFibEntry::FindBestCandidate (int skip/* = 0*/) const
{
  skip = skip % m_faces.size();
  return m_faces.get<i_nth> () [skip].GetFace ();
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
  m_node = 0;
  clear ();
  Object::DoDispose ();
}


CcnxFibEntryContainer::type::iterator
CcnxFib::LongestPrefixMatch (const CcnxInterestHeader &interest) const
{
  const CcnxNameComponents &name = interest.GetName ();
  for (size_t componentsCount = name.GetComponents ().size ();
       componentsCount > 0;
       componentsCount--)
    {
      CcnxNameComponents subPrefix (name.GetSubComponents (componentsCount));
      CcnxFibEntryContainer::type::iterator match = find (subPrefix);
      if (match != end())
        return match;
    }
  
  return end ();
}


CcnxFibEntryContainer::type::iterator
CcnxFib::Add (const CcnxNameComponents &prefix, Ptr<CcnxFace> face, int32_t metric)
{
// CcnxFibFaceMetric
  NS_LOG_FUNCTION(this << prefix << face << metric);
  CcnxFibEntryContainer::type::iterator entry = find (prefix);
  if (entry == end ())
    {
      entry = insert (end (), CcnxFibEntry (prefix));
      // insert new
    }

  NS_ASSERT_MSG (face != NULL, "Trying to modify NULL face");
  modify (entry, CcnxFibEntry::AddOrUpdateRoutingMetric (face, metric));
    
  return entry;
}
    

std::ostream& operator<< (std::ostream& os, const CcnxFib &fib)
{
  os << "Node " << Names::FindName (fib.m_node) << "\n";
  os << "  Dest prefix      Interfaces(Costs)                  \n";
  os << "+-------------+--------------------------------------+\n";
  
  for (CcnxFibEntryContainer::type::iterator entry = fib.begin ();
       entry != fib.end ();
       entry++)
    {
      os << *entry << "\n";
    }
  return os;
}

std::ostream& operator<< (std::ostream& os, const CcnxFibEntry &entry)
{
  os << *entry.m_prefix << "\t";
  
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
