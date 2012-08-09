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

#ifndef LOAD_STATS_FACE_H
#define LOAD_STATS_FACE_H

#include "load-stats.h"

namespace ns3 {
namespace ndn {
namespace ndnSIM {

class LoadStatsFace
{
public:
  void
  Step ();

  inline LoadStats&
  count ();

  inline const LoadStats&
  count () const;

  inline LoadStats&
  satisfied ();

  inline const LoadStats&
  satisfied () const;
  
  inline LoadStats&
  unsatisfied ();

  inline const LoadStats&
  unsatisfied () const;

  inline LoadStats&
  tx ();

  inline const LoadStats&
  tx () const;

  inline LoadStats&
  rx ();

  inline const LoadStats&
  rx () const;
  
  //
  LoadStats::stats_tuple
  GetSatisfiedRatio () const;

  LoadStats::stats_tuple
  GetUnsatisfiedRatio () const;

  LoadStatsFace &
  operator += (const LoadStatsFace &load);

  bool
  IsZero () const;
  
private:
  LoadStats m_count;
  LoadStats m_satisfied;
  LoadStats m_unsatisfied;
  LoadStats m_rx;
  LoadStats m_tx;

  friend std::ostream &
  operator << (std::ostream &os, const LoadStatsFace &stats);
};

inline LoadStats&
LoadStatsFace::count ()
{ return m_count; }

inline const LoadStats&
LoadStatsFace::count () const
{ return m_count; }

inline LoadStats&
LoadStatsFace::satisfied ()
{ return m_satisfied; }

inline const LoadStats&
LoadStatsFace::satisfied () const
{ return m_satisfied; }

inline LoadStats&
LoadStatsFace::unsatisfied ()
{ return m_unsatisfied; }

inline const LoadStats&
LoadStatsFace::unsatisfied () const
{ return m_unsatisfied; }

inline LoadStats&
LoadStatsFace::tx ()
{ return m_tx; }

inline const LoadStats&
LoadStatsFace::tx () const
{ return m_tx; }

inline LoadStats&
LoadStatsFace::rx ()
{ return m_rx; }

inline const LoadStats&
LoadStatsFace::rx () const
{ return m_rx; }


std::ostream &
operator << (std::ostream &os, const LoadStatsFace &stats);

} // namespace ndnSIM
} // namespace ndn
} // namespace ns3

#endif // LOAD_STATS_FACE_H
