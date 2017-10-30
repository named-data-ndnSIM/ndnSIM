## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
#
# Copyright (c) 2011-2015  Regents of the University of California.
#
# This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
# contributors.
#
# ndnSIM is free software: you can redistribute it and/or modify it under the terms
# of the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
#

# ndn-grid.py

from ns.core import *
from ns.network import *
from ns.point_to_point import *
from ns.point_to_point_layout import *
from ns.ndnSIM import *

#
# This scenario simulates a grid topology (using PointToPointGrid module)
#
# (consumer) -- ( ) ----- ( )
#     |          |         |
#    ( ) ------ ( ) ----- ( )
#     |          |         |
#    ( ) ------ ( ) -- (producer)
#
# All links are 1Mbps with propagation 10ms delay.
#
# FIB is populated using NdnGlobalRoutingHelper.
#
# Consumer requests data from producer with frequency 100 interests per second
# (interests contain constantly increasing sequence number).
#
# For every received interest, producer replies with a data packet, containing
# 1024 bytes of virtual payload.
#
# To run scenario and see what is happening, use the following command:
#
#     NS_LOG=ndn.Consumer:ndn.Producer ./waf --pyrun=src/ndnSIM/examples/ndn-grid.py
#

Config.SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"))
Config.SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"))
Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20))

import sys; cmd = CommandLine(); cmd.Parse(sys.argv);

p2p = PointToPointHelper ()
grid = PointToPointGridHelper (3,3,p2p)
grid.BoundingBox(100,100,200,200)

#######################################################

ndnHelper = ndn.StackHelper()
ndnHelper.InstallAll();


# Getting containers for the consumer/producer
producer = grid.GetNode(2, 2)
consumerNodes = NodeContainer()
consumerNodes.Add(grid.GetNode(0,0))


cHelper = ndn.AppHelper("ns3::ndn::ConsumerCbr")
cHelper.SetPrefix("/prefix")
cHelper.SetAttribute("Frequency", StringValue("10"))
out = cHelper.Install(consumerNodes)

pHelper = ndn.AppHelper("ns3::ndn::Producer")
pHelper.SetPrefix("/prefix")
pHelper.SetAttribute("PayloadSize", StringValue("1024"));
pHelper.Install(producer)

ndnGlobalRoutingHelper = ndn.GlobalRoutingHelper()
ndnGlobalRoutingHelper.InstallAll()

# Add /prefix origins to ndn::GlobalRouter
ndnGlobalRoutingHelper.AddOrigin("/prefix", producer)

# Calculate and install FIBs
ndnGlobalRoutingHelper.CalculateRoutes()


# #######################################################

Simulator.Stop(Seconds(20.0))
Simulator.Run()

# # To access FIB, PIT, CS, uncomment the following lines

# l3Protocol = ndn.L3Protocol.getL3Protocol(grid.GetNode(0,0))
# forwarder = l3Protocol.getForwarder()

# fib = forwarder.getFib()
# print "Contents of FIB (%d):" % fib.size()
# for i in fib:
#     print " - %s:" % i.getPrefix()
#     for nh in i.getNextHops():
#         print "    - %s%d (cost: %d)" % (nh.getFace(), nh.getFace().getId(), nh.getCost())

# pit = forwarder.getPit()
# print "Contents of PIT (%d):" % pit.size()
# for i in pit:
#     print " - %s" % i.getName()

# cs = forwarder.getCs()
# print "Contents of CS (%d):" % cs.size()
# for i in cs:
#     print " - %s" % i.getName()

Simulator.Destroy()

# # or run using the visualizer
# import visualizer
# visualizer.start()
