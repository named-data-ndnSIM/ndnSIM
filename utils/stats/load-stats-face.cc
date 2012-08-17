/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "load-stats-face.h"

#include "ns3/log.h"

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>

using namespace boost::mpl;
using namespace boost::tuples;

NS_LOG_COMPONENT_DEFINE ("ndn.LoadStatsFace");

namespace ns3 {
namespace ndn {
namespace ndnSIM {

std::ostream &
operator << (std::ostream &os, const LoadStats::stats_tuple &tuple);

void
LoadStatsFace::Step ()
{
  m_count.Step ();
  m_satisfied.Step ();
  m_unsatisfied.Step ();
  m_tx.Step ();
  m_rx.Step ();
}

struct update_retval
{
  update_retval (const LoadStats &count, const LoadStats &other, LoadStats::stats_tuple &tuple)
    : count_ (count)
    , other_ (other)
    , tuple_ (tuple) {}
  const LoadStats &count_;
  const LoadStats &other_;
  LoadStats::stats_tuple &tuple_;
    
  template< typename U >
  void operator () (U)
  {
    if (count_.GetStats ().get<U::value> () < LoadStats::PRECISION)
      tuple_.get<U::value> () = -1;
    else
      tuple_.get<U::value> () = other_.GetStats ().get<U::value> () / count_.GetStats ().get<U::value> ();
  }
};

LoadStats::stats_tuple
LoadStatsFace::GetSatisfiedRatio () const
{
  LoadStats::stats_tuple retval;
  for_each< range_c<int, 0, length<LoadStats::stats_tuple>::value> >
    (update_retval (m_count, m_satisfied, retval));

  // NS_LOG_DEBUG (retval.get<0> () << ", " << retval.get<1> () << ", " << retval.get<2> ());
  return retval;
}

LoadStats::stats_tuple
LoadStatsFace::GetUnsatisfiedRatio () const
{
  LoadStats::stats_tuple retval;
  for_each< range_c<int, 0, length<LoadStats::stats_tuple>::value> >
    (update_retval (m_count, m_unsatisfied, retval));

  // NS_LOG_DEBUG (retval.get<0> () << ", " << retval.get<1> () << ", " << retval.get<2> ());
  return retval;
}
  
LoadStatsFace &
LoadStatsFace::operator += (const LoadStatsFace &load)
{
  m_count       += load.m_count;
  m_satisfied   += load.m_satisfied;
  m_unsatisfied += load.m_unsatisfied;
  m_tx          += load.m_tx;
  m_rx          += load.m_rx;

  return *this;
}

bool
LoadStatsFace::IsZero () const
{
  return m_count.IsZero () && m_satisfied.IsZero () && m_unsatisfied.IsZero () && m_tx.IsZero () && m_rx.IsZero ();
}

struct print_tuple
{
  print_tuple (const LoadStats::stats_tuple &tuple, std::ostream &os)
    : tuple_ (tuple)
    , os_ (os)
  {
  }
  
  const LoadStats::stats_tuple &tuple_;
  std::ostream &os_;
    
  template< typename U >
  void operator () (U)
  {
    os_ << tuple_.get< U::value > () << " ";
  }
};


std::ostream &
operator << (std::ostream &os, const LoadStats::stats_tuple &tuple)
{
  for_each< range_c<int, 0, length<LoadStats::stats_tuple>::value> >
    (print_tuple (tuple, os));
  
  return os;
}

std::ostream &
operator << (std::ostream &os, const LoadStatsFace &stats)
{
  LoadStats::stats_tuple
    satisfied   = stats.GetSatisfiedRatio (),
    unsatisfied = stats.GetUnsatisfiedRatio ();

  os << "ration satisfied: " << satisfied << "/ unsatisfied: " << unsatisfied;
  return os;
}

} // namespace ndnSIM
} // namespace ndn
} // namespace ns3

