/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2018  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-lte-ue-net-device-transport.hpp"

#include "../helper/ndn-stack-helper.hpp"
#include "ndn-block-header.hpp"
#include "../utils/ndn-ns3-packet-tag.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

#include "ns3/queue.h"
#include "ns3/udp-socket-factory.h"

NS_LOG_COMPONENT_DEFINE("ndn.LteUeNetDeviceTransport");

namespace ns3 {
namespace ndn {

LteUeNetDeviceTransport::LteUeNetDeviceTransport(Ptr<Node> node,
                                                 const Ptr<NetDevice>& netDevice,
                                                 const std::string& localUri,
                                                 const std::string& remoteUri,
                                                 ::ndn::nfd::FaceScope scope,
                                                 ::ndn::nfd::FacePersistency persistency,
                                                 ::ndn::nfd::LinkType linkType)
  : m_netDevice(netDevice)
  , m_node(node)
{
  this->setLocalUri(FaceUri(localUri));
  this->setRemoteUri(FaceUri(remoteUri));
  this->setScope(scope);
  this->setPersistency(persistency);
  this->setLinkType(linkType);
  this->setMtu(m_netDevice->GetMtu()); // Use the MTU of the netDevice

  // // Get send queue capacity for congestion marking
  // PointerValue txQueueAttribute;
  // if (m_netDevice->GetAttributeFailSafe("TxQueue", txQueueAttribute)) {
  //   Ptr<ns3::QueueBase> txQueue = txQueueAttribute.Get<ns3::QueueBase>();
  //   // must be put into bytes mode queue

  //   auto size = txQueue->GetMaxSize();
  //   if (size.GetUnit() == BYTES) {
  //     this->setSendQueueCapacity(size.GetValue());
  //   }
  //   else {
  //     // don't know the exact size in bytes, guessing based on "standard" packet size
  //     this->setSendQueueCapacity(size.GetValue() * 1500);
  //   }
  // }

  NS_LOG_FUNCTION(this << "Creating an ndnSIM transport instance for netDevice with URI"
                  << this->getLocalUri());

  NS_ASSERT_MSG(m_netDevice != 0, "NetDeviceFace needs to be assigned a valid NetDevice");

  m_socket = Socket::CreateSocket(m_node, UdpSocketFactory::GetTypeId());
  m_socket->Bind(InetSocketAddress(Ipv4Address::GetAny (), 6363));
  m_socket->Connect(InetSocketAddress(Ipv4Address("225.63.63.1"), 6363));
  m_socket->Listen ();
  m_socket->SetAllowBroadcast(true);

  m_socket->SetRecvCallback(MakeCallback(&LteUeNetDeviceTransport::receiveFromSocket, this));
}

LteUeNetDeviceTransport::~LteUeNetDeviceTransport()
{
  NS_LOG_FUNCTION_NOARGS();
}

ssize_t
LteUeNetDeviceTransport::getSendQueueLength()
{
  // PointerValue txQueueAttribute;
  // if (m_netDevice->GetAttributeFailSafe("TxQueue", txQueueAttribute)) {
  //   Ptr<ns3::QueueBase> txQueue = txQueueAttribute.Get<ns3::QueueBase>();
  //   return txQueue->GetNBytes();
  // }
  // else {
  return nfd::face::QUEUE_UNSUPPORTED;
  // }
}

void
LteUeNetDeviceTransport::doClose()
{
  NS_LOG_FUNCTION(this << "Closing transport for netDevice with URI"
                  << this->getLocalUri());

  // set the state of the transport to "CLOSED"
  this->setState(nfd::face::TransportState::CLOSED);
}

void
LteUeNetDeviceTransport::doSend(Packet&& packet)
{
  NS_LOG_FUNCTION(this << "Sending packet from netDevice with URI"
                  << this->getLocalUri());

  // convert NFD packet to NS3 packet
  BlockHeader header(packet);

  Ptr<ns3::Packet> ns3Packet = Create<ns3::Packet>();
  ns3Packet->AddHeader(header);

  // send the NS3 packet
  m_socket->Send(ns3Packet);
}

// callback
void
LteUeNetDeviceTransport::receiveFromSocket(Ptr<Socket> socket)
{
  Ptr<ns3::Packet> p;
  while ((p = socket->Recv())) {
    // Convert NS3 packet to NFD packet
    Ptr<ns3::Packet> packet = p->Copy();

    BlockHeader header;
    packet->RemoveHeader(header);

    auto nfdPacket = Packet(std::move(header.getBlock()));

    this->receive(std::move(nfdPacket));
  }
}

Ptr<NetDevice>
LteUeNetDeviceTransport::GetNetDevice() const
{
  return m_netDevice;
}

} // namespace ndn
} // namespace ns3
