#include "ns3/test.h"
#include "ns3/annotated-topology-reader.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/uinteger.h"
#include "ns3/random-variable.h"
#include <limits> 
#include "ns3/ccnx-header-helper.h"
#include "ns3/header.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/nstime.h"
#include "ns3/buffer.h"
#include "ns3/log.h"

using namespace ns3;
#include <fstream>

NS_LOG_COMPONENT_DEFINE ("ContentObjectHeaderExample");

int
main (int argc, char *argv[])
{
  LogComponentEnable ("ContentObjectHeaderExample", LOG_ALL);
  LogComponentEnable ("Packet", LOG_ALL);
	
  NS_LOG_INFO ("Test started");

  Packet::EnablePrinting ();
  Packet::EnableChecking (); 
  Packet packet (10);
	
  CcnxContentObjectHeader header;
  CcnxContentObjectTail   trailer;
	
  Ptr<CcnxNameComponents> testname = Create<CcnxNameComponents> ();
  (*testname) ("1");
  header.SetName(testname);

  NS_LOG_INFO ("Source: \n" << header << trailer);

  packet.AddHeader (header);
  packet.AddTrailer (trailer);

  // NS_LOG_INFO ("Deserialized packet: \n" << packet);

  NS_LOG_INFO ("Removing and deserializing individual headers");
	
  CcnxContentObjectHeader dst_header;
  CcnxContentObjectTail   dst_trailer;

  packet.RemoveHeader (dst_header);
  packet.RemoveTrailer (dst_trailer);
	
  NS_LOG_INFO ("Target: \n" << dst_header << dst_trailer);

  return 0;
}
