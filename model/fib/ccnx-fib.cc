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

#include "ccnx-fib-impl.h"

#include "ns3/ccnx.h"
#include "ns3/ccnx-face.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-name-components.h"

#include "ns3/node.h"
#include "ns3/names.h"

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

namespace ns3 {

using namespace __ccnx_private;

TypeId 
CcnxFib::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::private::CcnxFib") // cheating ns3 object system
    .SetParent<Object> ()
    .SetGroupName ("Ccnx")
  ;
  return tid;
}

std::ostream& operator<< (std::ostream& os, const CcnxFib &fib)
{
  os << "Node " << Names::FindName (fib.GetObject<Node>()) << "\n";
  os << "  Dest prefix      Interfaces(Costs)                  \n";
  os << "+-------------+--------------------------------------+\n";

  fib.Print (os);
  return os;
}

} // namespace ns3
