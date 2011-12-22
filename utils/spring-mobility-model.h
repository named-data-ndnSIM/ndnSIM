/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2007 INRIA
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
 * Author: 
 */
#ifndef SPRING_MOBILITY_MODEL_H
#define SPRING_MOBILITY_MODEL_H

#include "ns3/mobility-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"

namespace ns3 {

/**
 * \ingroup mobility
 *
 * \brief 
 */
class SpringMobilityModel : public MobilityModel 
{
public:
  static TypeId GetTypeId (void);

  /**
   * Create a position located at coordinates (0,0,0) with velocity (0,0,0)
   */
  SpringMobilityModel ();
  virtual ~SpringMobilityModel ();

  /**
   * \brief Attach node by a spring
   * \param node MobilityModel layer of the attaching node
   */
  void
  AddSpring (Ptr<MobilityModel> node);

private:
  // from Object
  virtual void
  DoStart ();

  // from MobilityModel
  virtual Vector 
  DoGetPosition (void) const;

  virtual void 
  DoSetPosition (const Vector &position);

  virtual Vector 
  DoGetVelocity (void) const;

  // Updating positions
  void 
  Update (void) const;

  static void
  UpdateAll ();

private:
  static double m_epsilon;

  double m_nodeMass;
  double m_nodeCharge;
  double m_springNormalLength;
  double m_springConstant;
  double m_dampingFactor;

  static double m_totalKineticEnergy;  
  static bool m_stable;
  static EventId m_updateEvent;
  
  mutable Vector m_position;
  mutable Vector m_velocity;
  mutable Time m_lastTime;

  std::list<Ptr<MobilityModel> > m_springs;
};

} // namespace ns3

#endif // SPRING_MOBILITY_MODEL_H
