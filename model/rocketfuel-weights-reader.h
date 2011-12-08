/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hajime Tazaki
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
 * Author: Hajime Tazaki (tazaki@sfc.wide.ad.jp)
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef ROCKETFUEL_TOPOLOGY_READER_H
#define ROCKETFUEL_TOPOLOGY_READER_H

#include "ns3/nstime.h"
#include "ns3/net-device-container.h"
#include "ns3/topology-reader.h"


#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace ns3 {
    
    
    // ------------------------------------------------------------
    // --------------------------------------------
    /**
     * \brief Topology file reader (Rocketfuel-format type).
     *
     * http://www.cs.washington.edu/research/networking/rocketfuel/
     *
     * May 2nd, 2010: Currently only support "weights" file and "cch" file.
     * http://www.cs.washington.edu/research/networking/rocketfuel/maps/weights-dist.tar.gz
     * http://www.cs.washington.edu/research/networking/rocketfuel/maps/rocketfuel_maps_cch.tar.gz
     */
    class RocketfuelWeightsReader : public TopologyReader
    {
    public:
        static TypeId GetTypeId (void);
        
        RocketfuelWeightsReader ();
        virtual ~RocketfuelWeightsReader ();
        
        /**
         * \brief Main topology reading function.
         *
         * This method opens an input stream and reads the Rocketfuel-format file.
         * Every row represents a topology link (the ids of a couple of nodes),
         * so the input file is read line by line to figure out how many links
         * and nodes are in the topology.
         *
         * \return the container of the nodes created (or empty container if there was an error)
         */
        virtual NodeContainer Read (void);
        
        void ApplySettings(NetDeviceContainer* ndc, NodeContainer* nc);
        
    private:
        RocketfuelWeightsReader (const RocketfuelWeightsReader&);
        RocketfuelWeightsReader& operator= (const RocketfuelWeightsReader&);
        // Parser for the *.cch file available at:
        // http://www.cs.washington.edu/research/networking/rocketfuel/maps/rocketfuel_maps_cch.tar.gz
        NodeContainer GenerateFromMapsFile (int argc, char *argv[]);
        // Parser for the weights.* file available at:
        // http://www.cs.washington.edu/research/networking/rocketfuel/maps/weights-dist.tar.gz
        NodeContainer GenerateFromWeightsFile (int argc, char *argv[]);
        
        enum RF_FileType
        {
            RF_MAPS,
            RF_WEIGHTS,
            RF_UNKNOWN
        };
        enum RF_FileType GetFileType (const char *);
        
        // end class RocketfuelWeightsReader
    };
    
    // end namespace ns3
};


#endif /* ROCKETFUEL_TOPOLOGY_READER_H */


