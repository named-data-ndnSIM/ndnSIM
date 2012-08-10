
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

using namespace ns3;

class PcapWriter
{
public:
  PcapWriter (const std::string &file)
  {
    PcapHelper helper;
    m_pcap = helper.CreateFile (file, std::ios::out, PcapHelper::DLT_PPP);
  }
  
  void
  TracePacket (Ptr<const Packet> packet)
  {
    static PppHeader pppHeader;
    pppHeader.SetProtocol (0x0077);
    
    m_pcap->Write (Simulator::Now (), pppHeader, packet);
  }

private:
  Ptr<PcapFileWrapper> m_pcap;
};


int
main (int argc, char *argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));

  Config::SetDefault ("ns3::ndn::Producer::SignatureBits", StringValue ("1"));
  
  // Creating nodes
  NodeContainer nodes;
  nodes.Create (3);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install (nodes.Get (0), nodes.Get (1));
  p2p.Install (nodes.Get (1), nodes.Get (2));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes (true);
  ndnHelper.InstallAll ();

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
  // Consumer will request /prefix/0, /prefix/1, ...
  consumerHelper.SetPrefix ("/prefix");
  consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second
  consumerHelper.Install (nodes.Get (0)); // first node

  // Producer
  ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix ("/prefix");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  producerHelper.Install (nodes.Get (2)); // last node

  PcapWriter trace ("ndn-simple-trace.pcap");
  Config::ConnectWithoutContext ("/NodeList/*/$ns3::ndn::L3Protocol/FaceList/*/NdnTx",
				 MakeCallback (&PcapWriter::TracePacket, &trace));
  
  Simulator::Stop (Seconds (20.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

