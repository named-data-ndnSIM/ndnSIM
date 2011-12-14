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

#include "spring-mobility-helper.h"
#include "spring-mobility-model.h"

namespace ns3 {

void
SpringMobilityHelper::InstallSprings (Ptr<Node> node1, Ptr<Node> node2)
{
  Ptr<SpringMobilityModel> model1, model2;
  model1 = node1->GetObject<SpringMobilityModel> ();
  model2 = node2->GetObject<SpringMobilityModel> ();
  
  NS_ASSERT (model1 != 0 && model2 != 0);

  model1->AddSpring (model2);
  model2->AddSpring (model1);
}

void
SpringMobilityHelper::InstallSprings (TopologyReader::ConstLinksIterator first, 
				      TopologyReader::ConstLinksIterator end)
{
  for (; first != end; first++)
    {
      InstallSprings (first->GetFromNode (), 
		      first->GetToNode ());
    }
}

} // namespace ns3

