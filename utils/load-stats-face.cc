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

namespace ndnSIM
{

void
LoadStatsFace::Step ()
{
  m_count.Step ();
  m_satisfied.Step ();
  m_unsatisfied.Step ();
}
  
//
LoadStats::stats_tuple
LoadStatsFace::GetSatisfiedRatio () const
{
  return LoadStats::stats_tuple (m_satisfied.GetStats ().get<0> () / std::max (0.01, m_count.GetStats ().get<0> ()),
				 m_satisfied.GetStats ().get<1> () / std::max (0.01, m_count.GetStats ().get<1> ()),
				 m_satisfied.GetStats ().get<2> () / std::max (0.01, m_count.GetStats ().get<2> ()));
}

LoadStats::stats_tuple
LoadStatsFace::GetUnsatisfiedRatio () const
{
  return LoadStats::stats_tuple (m_unsatisfied.GetStats ().get<0> () / std::max (0.01, m_count.GetStats ().get<0> ()),
				 m_unsatisfied.GetStats ().get<1> () / std::max (0.01, m_count.GetStats ().get<1> ()),
				 m_unsatisfied.GetStats ().get<2> () / std::max (0.01, m_count.GetStats ().get<2> ()));
}
  
LoadStatsFace &
LoadStatsFace::operator += (const LoadStatsFace &load)
{
  m_count       += load.m_count;
  m_satisfied   += load.m_satisfied;
  m_unsatisfied += load.m_unsatisfied;

  return *this;
}

bool
LoadStatsFace::IsZero () const
{
  return m_count.IsZero () && m_satisfied.IsZero () && m_unsatisfied.IsZero ();
}

std::ostream &
operator << (std::ostream &os, const LoadStatsFace &stats)
{
  os << stats.m_count << "/" << stats.m_satisfied << "/" << stats.m_unsatisfied;
  return os;
}

}
