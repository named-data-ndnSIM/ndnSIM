/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Washington
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
 */

#ifndef CCNX_TRACE_HELPER_H
#define CCNX_TRACE_HELPER_H

#include "ns3/assert.h"
#include "ns3/ccnx-face-container.h"
#include "ns3/ccnx.h"
#include "ns3/trace-helper.h"

namespace ns3 {

/**
 * @brief Base class providing common user-level pcap operations for helpers
 * representing Ccnx protocols .
 */
class PcapHelperForCcnx
{
public:
  /**
   * @brief Construct a PcapHelperForCcnx.
   */
  PcapHelperForCcnx () {}

  /**
   * @brief Destroy a PcapHelperForCcnx.
   */
  virtual ~PcapHelperForCcnx () {}

  /**
   * @brief Enable pcap output the indicated Ccnx and interface pair.
   * @internal
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param Ccnx Ptr<Ccnx> on which you want to enable tracing.
   * @param interface Interface on Ccnx on which you want to enable tracing.
   * @param explicitFilename Treat the prefix as an explicit filename if true
   */
  virtual void EnablePcapCcnxInternal (std::string prefix, 
                                       Ptr<Ccnx> ccnx, 
                                       uint32_t interface,
                                       bool explicitFilename) = 0;

  /**
   * @brief Enable pcap output the indicated Ccnx and interface pair.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param ccnx Ptr<Ccnx> on which you want to enable tracing.
   * @param interface Interface on ccnx on which you want to enable tracing.
   * @param explicitFilename Treat the prefix as an explicit filename if true.
   */
  void EnablePcapCcnx (std::string prefix, Ptr<Ccnx> ccnx, uint32_t interface, bool explicitFilename = false);

  /**
   * @brief Enable pcap output the indicated ccnx and interface pair using a
   * Ptr<Ccnx> previously named using the ns-3 object name service.
   *
   * @param prefix filename prefix to use for pcap files.
   * @param ccnxName Name of the Ptr<Ccnx> on which you want to enable tracing.
   * @param interface Interface on ccnx on which you want to enable tracing.
   * @param explicitFilename Treat the prefix as an explicit filename if true.
   */
  void EnablePcapCcnx (std::string prefix, std::string ccnxName, uint32_t interface, bool explicitFilename = false);

  /**
   * @brief Enable pcap output on each Ccnx and interface pair in the container.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param c CcnxFaceContainer of Ccnx and interface pairs
   */
  void EnablePcapCcnx (std::string prefix, CcnxFaceContainer c);

  /**
   * @brief Enable pcap output on all Ccnx and interface pairs existing in the
   * nodes provided in the container.
   *
   * \param prefix Filename prefix to use for pcap files.
   * \param n container of nodes.
   */
  void EnablePcapCcnx (std::string prefix, NodeContainer n);

  /**
   * @brief Enable pcap output on the Ccnx and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ccnx aggregated to a node, the node-id unambiguously 
   * determines the Ccnx.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param nodeid The node identifier/number of the node on which to enable tracing.
   * @param interface Interface on ccnx on which you want to enable tracing.
   * @param explicitFilename Treat the prefix as an explicit filename if true
   */
  void EnablePcapCcnx (std::string prefix, uint32_t nodeid, uint32_t interface, bool explicitFilename);

  /**
   * @brief Enable pcap output on all Ccnx and interface pairs existing in the 
   * set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for pcap files.
   */
  void EnablePcapCcnxAll (std::string prefix);

};

/**
 * @brief Base class providing common user-level ascii trace operations for 
 * helpers representing Ccnx protocols .
 */
class AsciiTraceHelperForCcnx
{
public:
  /**
   * @brief Construct an AsciiTraceHelperForCcnx.
   */
  AsciiTraceHelperForCcnx () {}

  /**
   * @brief Destroy an AsciiTraceHelperForCcnx
   */
  virtual ~AsciiTraceHelperForCcnx () {}

  /**
   * @brief Enable ascii trace output on the indicated Ccnx and interface pair.
   * @internal
   *
   * The implementation is expected to use a provided Ptr<OutputStreamWrapper>
   * if it is non-null.  If the OutputStreamWrapper is null, the implementation
   * is expected to use a provided prefix to construct a new file name for
   * each net device using the rules described in the class overview.
   *
   * If the prefix is provided, there will be one file per Ccnx and interface pair
   * created.  In this case, adding a trace context to the file would be pointless,
   * so the helper implementation is expected to TraceConnectWithoutContext.
   *
   * If the output stream object is provided, there may be many different Ccnx 
   * and interface pairs writing to a single file.  In this case, the trace 
   * context could be important, so the helper implementation is expected to 
   * TraceConnect.
   *
   * @param stream An OutputStreamWrapper representing an existing file to use
   *               when writing trace data.
   * @param prefix Filename prefix to use for ascii trace files.
   * @param ccnx Ptr<Ccnx> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   * @param explicitFilename Treat the prefix as an explicit filename if true.
   */
  virtual void EnableAsciiCcnxInternal (Ptr<OutputStreamWrapper> stream, 
                                        std::string prefix, 
                                        Ptr<Ccnx> ccnx, 
                                        uint32_t interface,
                                        bool explicitFilename) = 0;

  /**
   * @brief Enable ascii trace output on the indicated ccnx and interface pair.
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param ccnx Ptr<Ccnx> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   * @param explicitFilename Treat the prefix as an explicit filename if true.
   */
  void EnableAsciiCcnx (std::string prefix, Ptr<Ccnx> ccnx, uint32_t interface, bool explicitFilename = false);

  /**
   * @brief Enable ascii trace output on the indicated Ccnx and interface pair.
   *
   * @param stream An OutputStreamWrapper representing an existing file to use
   *               when writing trace data.
   * @param ccnx Ptr<Ccnx> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, Ptr<Ccnx> ccnx, uint32_t interface);

  /**
   * @brief Enable ascii trace output the indicated ccnx and interface pair
   * using an ccnx previously named using the ns-3 object name service.
   *
   * @param prefix filename prefix to use for ascii files.
   * @param ccnxName The name of the ccnx on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   * @param explicitFilename Treat the prefix as an explicit filename if true.
   */
  void EnableAsciiCcnx (std::string prefix, std::string ccnxName, uint32_t interface, bool explicitFilename = false);

  /**
   * @brief Enable ascii trace output the indicated net device using a device 
   * previously named using the ns-3 object name service.
   *
   * @param stream An OutputStreamWrapper representing an existing file to use
   *               when writing trace data.
   * @param ccnxName The name of the ccnx on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, std::string ccnxName, uint32_t interface);

  /**
   * @brief Enable ascii trace output on each Ccnx and interface pair in the 
   * container
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param c CcnxFaceContainer of Ccnx and interface pairs on which to 
   *          enable tracing.
   */
  void EnableAsciiCcnx (std::string prefix, CcnxFaceContainer c);

  /**
   * @brief Enable ascii trace output on each device in the container which is
   * of the appropriate type.
   *
   * @param stream An OutputStreamWrapper representing an existing file to use
   *               when writing trace data.
   * @param c ccnxInterfaceContainer of ccnx and interface pairs on which to 
   *          enable tracing.
   */
  void EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, CcnxFaceContainer c);

  /**
   * @brief Enable ascii trace output on all ccnx and interface pairs existing
   * in the nodes provided in the container.
   *
   * \param prefix Filename prefix to use for ascii files.
   * \param n container of nodes.
   */
  void EnableAsciiCcnx (std::string prefix, NodeContainer n);

  /**
   * @brief Enable ascii trace output on all ccnx and interface pairs existing
   * in the nodes provided in the container.
   *
   * @param stream An OutputStreamWrapper representing an existing file to use
   *               when writing trace data.
   * \param n container of nodes.
   */
  void EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, NodeContainer n);

  /**
   * @brief Enable ascii trace output on all Ccnx and interface pairs existing
   * in the set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for ascii files.
   */
  void EnableAsciiCcnxAll (std::string prefix);

  /**
   * @brief Enable ascii trace output on each device (which is of the
   * appropriate type) in the set of all nodes created in the simulation.
   *
   * @param stream An OutputStreamWrapper representing an existing file to use
   *               when writing trace data.
   */
  void EnableAsciiCcnxAll (Ptr<OutputStreamWrapper> stream);

  /**
   * @brief Enable pcap output on the Ccnx and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ccnx aggregated to a node, the node-id unambiguously 
   * determines the Ccnx.
   *
   * @param prefix Filename prefix to use when creating ascii trace files
   * @param nodeid The node identifier/number of the node on which to enable
   *               ascii tracing
   * @param deviceid The device identifier/index of the device on which to enable
   *                 ascii tracing
   * @param explicitFilename Treat the prefix as an explicit filename if true
   */
  void EnableAsciiCcnx (std::string prefix, uint32_t nodeid, uint32_t deviceid, bool explicitFilename);

  /**
   * @brief Enable pcap output on the ccnx and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one ccnx aggregated to a node, the node-id unambiguously 
   * determines the ccnx.
   *
   * @param stream An OutputStreamWrapper representing an existing file to use
   *               when writing trace data.
   * @param nodeid The node identifier/number of the node on which to enable
   *               ascii tracing
   * @param interface The interface on which you want to enable tracing.
   * @param explicitFilename Treat the prefix as an explicit filename if true
   */
  void EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, uint32_t nodeid, uint32_t interface, bool explicitFilename);

private:
  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> stream, 
                            std::string prefix, 
                            uint32_t nodeid, 
                            uint32_t interface,
                            bool explicitFilename);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> stream, std::string prefix, NodeContainer n);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> stream, std::string prefix, CcnxFaceContainer c);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> stream, 
                            std::string prefix, 
                            std::string ccnxName, 
                            uint32_t interface,
                            bool explicitFilename);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> stream, 
                            std::string prefix, 
                            Ptr<Ccnx> ccnx, 
                            uint32_t interface,
                            bool explicitFilename);
};

} // namespace ns3

#endif /* CCNX_TRACE_HELPER_H */
