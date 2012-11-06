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


#include "highway-position-allocator.h"
#include "ns3/random-variable.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("HighwayPositionAllocator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (HighwayPositionAllocator);

TypeId HighwayPositionAllocator::GetTypeId (void){
  static TypeId tid = TypeId("ns3::HighwayPositionAllocator").
    SetParent<PositionAllocator> ().
    SetGroupName("Mobility").
    AddConstructor<HighwayPositionAllocator>().
    AddAttribute("Start", "the start position of the highway",
		 VectorValue (Vector(0.0, 0.0, 0.0)),
		 MakeVectorAccessor(&HighwayPositionAllocator::SetStartPosition,
				    &HighwayPositionAllocator::GetStartPosition),
                 MakeVectorChecker ()).
    AddAttribute("Direction", "the direction of the highway",
		 DoubleValue (0.0),
		 MakeDoubleAccessor(&HighwayPositionAllocator::SetDirection,
				    &HighwayPositionAllocator::GetDirection),
                 MakeDoubleChecker<double> ()).
    AddAttribute("Length", "the length of the highway",
		 DoubleValue (0.0),
		 MakeDoubleAccessor(&HighwayPositionAllocator::SetLength,
				    &HighwayPositionAllocator::GetLength),
                 MakeDoubleChecker<double> ());

  return tid;
}

HighwayPositionAllocator::HighwayPositionAllocator ()
  : m_previous_position (Vector (0.0, 0.0, 0.0))
{
}

Vector HighwayPositionAllocator::GetNext (void) const{
  double random_gap = m_random_gap_var.GetValue (1.0, 10.0);
  
  double delta_x = random_gap * cos(m_direction);
  double delta_y = random_gap * sin(m_direction);
  
  
  Vector new_position(m_previous_position.x - delta_x, m_previous_position.y - delta_y, m_previous_position.z);
  m_previous_position.x = new_position.x;
  m_previous_position.y = new_position.y;
  m_previous_position.z = new_position.z;

  return new_position;
}

void HighwayPositionAllocator::SetStartPosition(Vector start){
  m_start = start;
  m_previous_position = m_start; // initialize the m_previous_position to be start of the highway
}

Vector HighwayPositionAllocator::GetStartPosition(void) const {
  return m_start;
}

void HighwayPositionAllocator::SetDirection(double direction){
  m_direction = direction;
}

double HighwayPositionAllocator::GetDirection(void) const {
  return m_direction;
}

void HighwayPositionAllocator::SetLength(double length){
  m_length = length;
}

double HighwayPositionAllocator::GetLength(void) const {
  return m_length;
}


int64_t
HighwayPositionAllocator::AssignStreams (int64_t stream)
{
  m_random_gap_var.SetStream (stream);
  return 1;
}

}
