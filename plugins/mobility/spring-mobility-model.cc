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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */
#include "spring-mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/node-list.h"
#include "ns3/node.h"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("SpringMobilityModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SpringMobilityModel);

double SpringMobilityModel::m_totalKineticEnergy = 0.0;
bool SpringMobilityModel::m_stable = false;
EventId SpringMobilityModel::m_updateEvent;
double SpringMobilityModel::m_epsilon = 100.0;

const double COLOUMB_K = 200; 

TypeId SpringMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SpringMobilityModel")
    .SetParent<MobilityModel> ()
    .AddConstructor<SpringMobilityModel> ()
    // .AddAttribute ("Epsilon", "Bound for kinetic energy when system is considered stable",
    //                DoubleValue (100.0),
    //                MakeDoubleAccessor (&SpringMobilityModel::m_epsilon),
    //                MakeDoubleChecker<double> ())
    .AddAttribute ("NodeMass", "Node mass",
                   DoubleValue (10),
                   MakeDoubleAccessor (&SpringMobilityModel::m_nodeMass),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("NodeCharge", "Node charge",
                   DoubleValue (2),
                   MakeDoubleAccessor (&SpringMobilityModel::m_nodeCharge),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SpringNormalLength", "Normal length of spring length",
                   DoubleValue (10),
                   MakeDoubleAccessor (&SpringMobilityModel::m_springNormalLength),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SpringConstant", "Spring constant",
                   DoubleValue (0.2),
                   MakeDoubleAccessor (&SpringMobilityModel::m_springConstant),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("DampingFactor", "Dumping factor",
                   DoubleValue (0.8),
                   MakeDoubleAccessor (&SpringMobilityModel::m_dampingFactor),
                   MakeDoubleChecker<double> ())
    ;
  
  return tid;
}

SpringMobilityModel::SpringMobilityModel ()
  : m_position (0,0,0)
  , m_velocity (0,0,0)
{
}

SpringMobilityModel::~SpringMobilityModel ()
{
}

void
SpringMobilityModel::AddSpring (Ptr<MobilityModel> node)
{
  m_springs.push_back (node);
}

void
SpringMobilityModel::DoStart ()
{
  if (!m_updateEvent.IsRunning ())
    m_updateEvent = Simulator::Schedule (Seconds(0.05), SpringMobilityModel::UpdateAll);
}

void
SpringMobilityModel::UpdateAll ()
{
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      Ptr<SpringMobilityModel> model = (*node)->GetObject<SpringMobilityModel> ();
      if (model != 0)
        model->Update ();
    }

  if (m_totalKineticEnergy < m_epsilon)
    {
      m_stable = true;
      NS_LOG_INFO ("Stabilized with " << m_totalKineticEnergy);
    }
  else
    m_updateEvent = Simulator::Schedule (Seconds(0.05), SpringMobilityModel::UpdateAll);  
}

void
SpringMobilityModel::Update () const
{
  NS_LOG_FUNCTION (this << m_stable << m_position << m_velocity);
  if (m_stable) return;
  Time now = Simulator::Now ();

  if (now <= m_lastTime)
    {
      m_lastTime = now;
      return;
    }

  double time_step_s = (now - m_lastTime).ToDouble (Time::S);
  m_lastTime = now;

  Vector force (0.0, 0.0, 0.0);

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      if ((*node)->GetId () == GetObject<Node> ()->GetId ()) continue;
      Ptr<SpringMobilityModel> model = (*node)->GetObject<SpringMobilityModel> ();
      if (model == 0) continue;
      if (model == this) continue;

      double distance = GetDistanceFrom (model);
      if (distance < 0.1) continue;

      Vector direction = (GetPosition () - model->GetPosition ()) / distance; // direction vector of size 1, force trying to take nodes apart

      force += direction * COLOUMB_K * m_nodeCharge * m_nodeCharge / distance / distance;
    }

  BOOST_FOREACH (Ptr<MobilityModel> model, m_springs)
    {
      double distance = GetDistanceFrom (model);
      Vector direction = (model->GetPosition () - GetPosition ()) / distance; // direction vector of size 1, force trying to take nodes closer, if they are more than distance apart

      force += direction * (- m_springNormalLength + distance) / m_springConstant;
    }

  // NS_LOG_DEBUG ("force: " << force);

  // subtract previous value of kinetic energy for the node
  double velocityValue = CalculateDistance (m_velocity, Vector(0,0,0)); 
  m_totalKineticEnergy -= m_nodeMass * velocityValue * velocityValue;

  // Correct velocity and position
  m_velocity = (m_velocity + force * time_step_s) * m_dampingFactor;
  m_position += m_velocity * time_step_s;

  // Add new value for the kinetic energy
  velocityValue = CalculateDistance (m_velocity, Vector(0,0,0)); 
  m_totalKineticEnergy += m_nodeMass * velocityValue * velocityValue;

  NotifyCourseChange ();
}

Vector
SpringMobilityModel::DoGetPosition (void) const
{
  // NS_LOG_FUNCTION (this << m_position);
  return m_position;
}
void 
SpringMobilityModel::DoSetPosition (const Vector &position)
{
  // NS_LOG_FUNCTION (this << position);
  m_position = position;

  NotifyCourseChange ();
  m_stable = false;


  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      Ptr<SpringMobilityModel> model = (*node)->GetObject<SpringMobilityModel> ();
      if (model != 0)
        model->m_lastTime = Simulator::Now ();
    }
  
  if (!m_updateEvent.IsRunning ())
    m_updateEvent = Simulator::Schedule (Seconds(0.05), SpringMobilityModel::UpdateAll);
}
Vector
SpringMobilityModel::DoGetVelocity (void) const
{
  return m_velocity;
}

} // namespace ns3
