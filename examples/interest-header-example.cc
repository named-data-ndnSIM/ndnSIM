#include "ns3/test.h"
#include "ns3/annotated-topology-reader.h"
#include "ns3/ccnx-interest-header.h"
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

NS_LOG_COMPONENT_DEFINE ("InterestHeaderExample");

int
main (int argc, char *argv[])
{
	LogComponentEnable ("InterestHeaderExample", LOG_ALL);
	LogComponentEnable ("Packet", LOG_ALL);
	
    NS_LOG_INFO ("Test started");

	Packet::EnablePrinting ();
	Packet::EnableChecking (); 
	Packet packet (0);

    CcnxInterestHeader interestHeader;
	
    Ptr<CcnxNameComponents> testname = Create<CcnxNameComponents> ();
    (*testname) ("first") ("second");
    interestHeader.SetName(testname);
	
    uint32_t minSuffixComponents = 20;
    interestHeader.SetMinSuffixComponents(minSuffixComponents);
	
    uint32_t maxSuffixComponents = 40;
    interestHeader.SetMaxSuffixComponents(maxSuffixComponents);
	
    Time lifetime = Seconds(661777) + MicroSeconds(1234);
    interestHeader.SetInterestLifetime(lifetime);

    bool child = true;
    interestHeader.SetChildSelector(child);

    Ptr<CcnxNameComponents> exclude = Create<CcnxNameComponents> ();
    (*exclude) ("exclude1") ("exclude2");
    interestHeader.SetExclude(exclude);

	UniformVariable random(1, std::numeric_limits<uint32_t>::max ());
    uint32_t randomNonce = static_cast<uint32_t> (random.GetValue());
    interestHeader.SetNonce(randomNonce);
    
    interestHeader.SetNack(true);
    interestHeader.SetCongested(true);
	NS_LOG_INFO ("Source: \n" << interestHeader);
    
	packet.AddHeader (interestHeader);
	NS_LOG_INFO ("Deserialized packet: " << packet);

	NS_LOG_INFO ("Removing and deserializing individual headers");
	
    CcnxInterestHeader target;
	packet.RemoveHeader (target);

	NS_LOG_INFO ("Target: \n" << target);

	return 0;
}
