/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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

#ifndef CCNX_STACK_HELPER_H
#define CCNX_STACK_HELPER_H

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/object-factory.h"

#include "ccnx-trace-helper.h"

namespace ns3 {

class Node;
class CcnxForwardingHelper;
class CcnxFaceContainer;

/**
 * \ingroup ccnx
 * \defgroup ccnx-helpers Helpers
 */
/**
 * \ingroup ccnx-helpers
 * \brief Adding CCNx functionality to existing Nodes.
 *
 * This helper enables pcap and ascii tracing of events in the ccnx stack
 * associated with a node.  This is substantially similar to the tracing that
 * happens in device helpers, but the important difference is that, well, there
 * is no device.  This means that the creation of output file names will change,
 * and also the user-visible methods will not reference devices and therefore
 * the number of trace enable methods is reduced.
 *
 * Normally we eschew multiple inheritance, however, the classes
 * PcapUserHelperForCcnx and AsciiTraceUserHelperForCcnx are treated as
 * "mixins".  A mixin is a self-contained class that encapsulates a general
 * attribute or a set of functionality that may be of interest to many other
 * classes.
 */
class CcnxStackHelper : public PcapHelperForCcnx, public AsciiTraceHelperForCcnx
{
public:
  /**
   * \brief Create a new CcnxStackHelper which <empty> forwarding by default.
   *
   * \todo set non-empty default forwarding
   */
  CcnxStackHelper ();

  /**
   * \brief Destroy the CcnxStackHelper
   */
  virtual ~CcnxStackHelper ();
  CcnxStackHelper (const CcnxStackHelper &);
  CcnxStackHelper &operator = (const CcnxStackHelper &o);

  /**
   * \brief Return helper internal state to that of a newly constructed one
   */
  void Reset ();

  /**
   * Set forwarding strategy helper
   *
   * \param forwarding a new forwarding helper
   *
   * Set the forwarding helper to use during Install. The forwarding helper is
   * really an object factory which is used to create an object of type
   * ns3::CcnxFrProtocol per node. This forwarding object is then associated to
   * a single ns3::Ccnx object through its ns3::Ccnx::SetforwardingProtocol.
   */
  void
  SetForwardingHelper (const CcnxForwardingHelper &forwarding);

  /**
   * \brief Install CCNx stack on the node
   *
   * This method will assert if called on a node that already has Ccnx object
   * installed on it
   * 
   * \param nodeName The name of the node on which to install the stack.
   *
   * \returns list of installed faces in the form of a smart pointer
   * to CcnxFaceContainer object
   */
  Ptr<CcnxFaceContainer>
  Install (std::string nodeName) const;

  /**
   * \brief Install CCNx stack on the node
   *
   * This method will assert if called on a node that already has Ccnx object
   * installed on it
   * 
   * \param node The node on which to install the stack.
   *
   * \returns list of installed faces in the form of a smart pointer
   * to CcnxFaceContainer object
   */
  Ptr<CcnxFaceContainer>
  Install (Ptr<Node> node) const;

  /**
   * \brief Install CCNx stack on each node in the input container
   *
   * The program will assert if this method is called on a container with a node
   * that already has an ccnx object aggregated to it.
   * 
   * \param c NodeContainer that holds the set of nodes on which to install the
   * new stacks.
   *
   * \returns list of installed faces in the form of a smart pointer
   * to CcnxFaceContainer object
   */
  Ptr<CcnxFaceContainer>
  Install (NodeContainer c) const;

  /**
   * \brief Install CCNx stack on all nodes in the simulation
   *
   * \returns list of installed faces in the form of a smart pointer
   * to CcnxFaceContainer object
   */
  Ptr<CcnxFaceContainer>
  InstallAll () const;

  /**
   * \brief Enable/disable ccnx stack install.
   *
   * \param enable enable state
   */
  void
  SetCcnxStackInstall (bool enable);

private:
  /**
   * @brief Enable pcap output the indicated Ccnx and interface pair.
   * @internal
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param ccnx Ptr to the Ccnx interface on which you want to enable tracing.
   * @param interface Interface ID on the Ccnx on which you want to enable tracing.
   */
  virtual void EnablePcapCcnxInternal (std::string prefix, 
                                       Ptr<Ccnx> ccnx, 
                                       uint32_t interface,
                                       bool explicitFilename);

  /**
   * @brief Enable ascii trace output on the indicated Ccnx and interface pair.
   * @internal
   *
   * @param stream An OutputStreamWrapper representing an existing file to use
   *               when writing trace data.
   * @param prefix Filename prefix to use for ascii trace files.
   * @param ccnx Ptr to the Ccnx interface on which you want to enable tracing.
   * @param interface Interface ID on the Ccnx on which you want to enable tracing.
   */
  virtual void EnableAsciiCcnxInternal (Ptr<OutputStreamWrapper> stream, 
                                        std::string prefix, 
                                        Ptr<Ccnx> ccnx, 
                                        uint32_t interface,
                                        bool explicitFilename);

  void Initialize (void);
  ObjectFactory m_tcpFactory;
  const CcnxForwardingHelper *m_forwarding;

  /**
   * \internal
   */
  static void CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId);

  /**
   * \internal
   */
  static void Cleanup (void);

  /**
   * \internal
   */
  bool PcapHooked (Ptr<Ccnx> ccnx);

  /**
   * \internal
   */
  bool AsciiHooked (Ptr<Ccnx> ccnx);

  /**
   * \brief Ccnx install state (enabled/disabled) ?
   */
  bool m_ccnxEnabled;
};

} // namespace ns3

#endif /* CCNX_STACK_HELPER_H */
