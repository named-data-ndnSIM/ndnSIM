/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Lucas Wang <lucas@cs.ucla.edu>
 */
#ifndef HIGHWAY_POSITION_ALLOCATOR_H
#define HIGHWAY_POSITION_ALLOCATOR_H

#include "ns3/position-allocator.h"

namespace ns3 {
  
/**
 * This position allocator is used to generate positions for cars 
 * on a highway.
 */

class HighwayPositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);
  HighwayPositionAllocator ();
  virtual Vector GetNext (void) const;
  void SetStartPosition(Vector start);
  Vector GetStartPosition(void) const;
  void SetDirection(double direction); // the angle in terms of radian degrees
  double GetDirection(void) const ;
  void SetLength(double length);
  double GetLength(void) const ;

  virtual int64_t AssignStreams (int64_t stream);

private:
  mutable Vector m_previous_position; // previously generated vehicle position
  Vector m_start; // highway starting point
  double m_direction; //highway direction
  double m_length; // highway length

  mutable UniformRandomVariable m_random_gap_var;
};
}

#endif
