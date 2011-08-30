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

NS_LOG_COMPONENT_DEFINE ("InterestHeaderExample");

int
main (int argc, char *argv[])
{
    NS_LOG_INFO ("Test started");
    uint32_t randomNonce = UniformVariable().GetInteger(1, std::numeric_limits<uint32_t>::max ());
    Ptr<CcnxNameComponents> testname = Create<CcnxNameComponents> ();
    (*testname) ("test") ("test2");
    
    Ptr<CcnxNameComponents> exclude = Create<CcnxNameComponents> ();
    (*testname) ("exclude") ("exclude2");
    
    Time lifetime = Seconds(4.0);
    bool child = true;
    
    uint32_t maxSuffixComponents = 40;
    uint32_t minSuffixComponents = 20;
    
    CcnxInterestHeader interestHeader;
    interestHeader.SetNonce(randomNonce);
    interestHeader.SetName(testname);
    interestHeader.SetInterestLifetime(lifetime);
    interestHeader.SetChildSelector(child);
    interestHeader.SetExclude(exclude);
    interestHeader.SetMaxSuffixComponents(maxSuffixComponents);
    interestHeader.SetMinSuffixComponents(minSuffixComponents);
    
    uint32_t size = interestHeader.GetSerializedSize();
    NS_LOG_INFO ("Size = " << size);
    //uint32_t size = 5;
    //NS_TEST_ASSERT_MSG_EQ (false, true, "GetSize = " << size);
    
    Buffer buf(size);
    Buffer::Iterator iter = buf.Begin ();
    //interestHeader.
    //interestHeader.Serialize(iter);
    
    iter = buf.Begin ();
    CcnxInterestHeader target;
    
}