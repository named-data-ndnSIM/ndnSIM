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

# ndn-simple.py

from ns.core import *
from ns.network import *
from ns.point_to_point import *
from ns.ndnSIM import *

#
# This scenario simulates a very simple network topology:
#
#
#      +----------+     1Mbps      +--------+     1Mbps      +----------+
#      | consumer | <------------> | router | <------------> | producer |
#      +----------+         10ms   +--------+          10ms  +----------+
#
#
# Consumer requests data from producer with frequency 10 interests per second
# (interests contain constantly increasing sequence number).
#
# For every received interest, producer replies with a data packet, containing
# 1024 bytes of virtual payload.
#
# To run scenario and see what is happening, use the following command:
#
#     NS_LOG=ndn.Consumer:ndn.Producer ./waf --pyrun=src/ndnSIM/examples/ndn-simple.py
#

# Set default parameters for PointToPoint links and channels
Config.SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"))
Config.SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"))
Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20))

# Read optional command-line parameters (e.g., enable visualizer with ./waf --pyrun=<> --visualize
import sys; cmd = CommandLine(); cmd.Parse(sys.argv);

# Creating nodes
nodes = NodeContainer()
nodes.Create(3)

# Connecting nodes using two links
p2p = PointToPointHelper()
p2p.Install(nodes.Get(0), nodes.Get(1))
p2p.Install(nodes.Get(1), nodes.Get(2))

# Install NDN stack on all nodes
ndnHelper = ndn.StackHelper()
ndnHelper.SetDefaultRoutes(True)
ndnHelper.InstallAll()

# Choosing forwarding strategy
ndn.StrategyChoiceHelper.InstallAll("/prefix", "/localhost/nfd/strategy/multicast")

# Installing applications

# Consumer
consumerHelper = ndn.AppHelper("ns3::ndn::ConsumerCbr")
# Consumer will request /prefix/0, /prefix/1, ...
consumerHelper.SetPrefix("/prefix")
consumerHelper.SetAttribute("Frequency", StringValue("10")) # 10 interests a second
consumerHelper.Install(nodes.Get(0))                        # first node

# Producer
producerHelper = ndn.AppHelper("ns3::ndn::Producer")
# Producer will reply to all requests starting with /prefix
producerHelper.SetPrefix("/prefix")
producerHelper.SetAttribute("PayloadSize", StringValue("1024"))
producerHelper.Install(nodes.Get(2)) # last node

Simulator.Stop(Seconds(20.0))

Simulator.Run()
Simulator.Destroy()
