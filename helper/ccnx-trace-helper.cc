/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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

#include <stdint.h>
#include <string>
#include <fstream>

#include "ns3/abort.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/names.h"
#include "ns3/net-device.h"
#include "ns3/pcap-file-wrapper.h"

#include "ccnx-trace-helper.h"

NS_LOG_COMPONENT_DEFINE ("CcnxTraceHelper");

namespace ns3 {

void 
PcapHelperForCcnx::EnablePcapCcnx (std::string prefix, Ptr<Ccnx> ccnx, uint32_t interface, bool explicitFilename)
{
  EnablePcapCcnxInternal (prefix, ccnx, interface, explicitFilename);
}

void 
PcapHelperForCcnx::EnablePcapCcnx (std::string prefix, std::string ccnxName, uint32_t interface, bool explicitFilename)
{
  Ptr<Ccnx> ccnx = Names::Find<Ccnx> (ccnxName);
  EnablePcapCcnx (prefix, ccnx, interface, explicitFilename);
}

  /// \todo This call is broken
void 
PcapHelperForCcnx::EnablePcapCcnx (std::string prefix, CcnxFaceContainer c)
{
  for (CcnxFaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      // EnablePcapCcnx (prefix, (*i)->GetCcnx (), 0,false);
    }
}

void
PcapHelperForCcnx::EnablePcapCcnx (std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ccnx> ccnx = node->GetObject<Ccnx> ();
      if (ccnx)
        {
          for (uint32_t j = 0; j < ccnx->GetNFaces (); ++j)
            {
              EnablePcapCcnx (prefix, ccnx, j, false);
            }
        }
    }
}

void
PcapHelperForCcnx::EnablePcapCcnxAll (std::string prefix)
{
  EnablePcapCcnx (prefix, NodeContainer::GetGlobal ());
}

void 
PcapHelperForCcnx::EnablePcapCcnx (std::string prefix, uint32_t nodeid, uint32_t interface, bool explicitFilename)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }

      Ptr<Ccnx> ccnx = node->GetObject<Ccnx> ();
      if (ccnx)
        {
          EnablePcapCcnx (prefix, ccnx, interface, explicitFilename);
        }
      return;
    }
}

//
// Public API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnx (std::string prefix, Ptr<Ccnx> ccnx, uint32_t interface, bool explicitFilename)
{
  EnableAsciiCcnxInternal (Ptr<OutputStreamWrapper> (), prefix, ccnx, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, Ptr<Ccnx> ccnx, uint32_t interface)
{
  EnableAsciiCcnxInternal (stream, std::string (), ccnx, interface, false);
}

//
// Public API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnx (
  std::string prefix, 
  std::string ccnxName, 
  uint32_t interface,
  bool explicitFilename)
{
  EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> (), prefix, ccnxName, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, std::string ccnxName, uint32_t interface)
{
  EnableAsciiCcnxImpl (stream, std::string (), ccnxName, interface, false);
}

//
// Private API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnxImpl (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  std::string ccnxName, 
  uint32_t interface,
  bool explicitFilename)
{
  Ptr<Ccnx> ccnx = Names::Find<Ccnx> (ccnxName);
  EnableAsciiCcnxInternal (stream, prefix, ccnx, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnx (std::string prefix, CcnxFaceContainer c)
{
  EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> (), prefix, c);
}

//
// Public API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, CcnxFaceContainer c)
{
  EnableAsciiCcnxImpl (stream, std::string (), c);
}

//
// Private API
// \todo This method is broken
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> stream, std::string prefix, CcnxFaceContainer c)
{
  for (CcnxFaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      // EnableAsciiCcnxInternal (stream, prefix, pair.first, pair.second, false);
    }
}

//
// Public API
//
void
AsciiTraceHelperForCcnx::EnableAsciiCcnx (std::string prefix, NodeContainer n)
{
  EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> (), prefix, n);
}

//
// Public API
//
void
AsciiTraceHelperForCcnx::EnableAsciiCcnx (Ptr<OutputStreamWrapper> stream, NodeContainer n)
{
  EnableAsciiCcnxImpl (stream, std::string (), n);
}

//
// Private API
//
void
AsciiTraceHelperForCcnx::EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> stream, std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ccnx> ccnx = node->GetObject<Ccnx> ();
      if (ccnx)
        {
          for (uint32_t j = 0; j < ccnx->GetNFaces (); ++j)
            {
              EnableAsciiCcnxInternal (stream, prefix, ccnx, j, false);
            }
        }
    }
}

//
// Public API
//
void
AsciiTraceHelperForCcnx::EnableAsciiCcnxAll (std::string prefix)
{
  EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> (), prefix, NodeContainer::GetGlobal ());
}

//
// Public API
//
void
AsciiTraceHelperForCcnx::EnableAsciiCcnxAll (Ptr<OutputStreamWrapper> stream)
{
  EnableAsciiCcnxImpl (stream, std::string (), NodeContainer::GetGlobal ());
}

//
// Public API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnx (
  Ptr<OutputStreamWrapper> stream, 
  uint32_t nodeid, 
  uint32_t interface,
  bool explicitFilename)
{
  EnableAsciiCcnxImpl (stream, std::string (), nodeid, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnx (std::string prefix, uint32_t nodeid, uint32_t interface, bool explicitFilename)
{
  EnableAsciiCcnxImpl (Ptr<OutputStreamWrapper> (), prefix, nodeid, interface, explicitFilename);
}

//
// Private API
//
void 
AsciiTraceHelperForCcnx::EnableAsciiCcnxImpl (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t interface,
  bool explicitFilename)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }

      Ptr<Ccnx> ccnx = node->GetObject<Ccnx> ();
      if (ccnx)
        {
          EnableAsciiCcnxInternal (stream, prefix, ccnx, interface, explicitFilename);
        }

      return;
    }
}


} // namespace ns3

