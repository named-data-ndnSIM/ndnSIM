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

NS_LOG_COMPONENT_DEFINE ("InterestHeaderSerializationTest");

class InterestHeaderSerializationTest : public TestCase
{
public:
    //static const uint32_t N_RUNS = 5;
    //static const uint32_t N_BINS = 50;
    //static const uint32_t N_MEASUREMENTS = 1000000;
    
    InterestHeaderSerializationTest ();
    virtual ~InterestHeaderSerializationTest ();
    
    //double ChiSquaredTest (UniformVariable &u);
    
private:
    virtual void DoRun (void);
};

InterestHeaderSerializationTest::InterestHeaderSerializationTest ()
: TestCase ("Interest Header Serialization Test")
{
}

InterestHeaderSerializationTest::~InterestHeaderSerializationTest ()
{
}

void
InterestHeaderSerializationTest::DoRun(void)
{
    //ReportStart();
    //SetVerbose(true);
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
    //uint32_t size = 5;
    NS_TEST_ASSERT_MSG_EQ (false, true, "GetSize = " << size);
    Buffer buf(size);
    Buffer::Iterator iter = buf.Begin ();
    //interestHeader.
    //interestHeader.Serialize(iter);

    iter = buf.Begin ();
    CcnxInterestHeader target;
    //target.Deserialize(iter);
    
    /*if(target.GetNonce() == randomNonce)
    {
        
        ReportCaseFailure();
    }*/
    NS_TEST_ASSERT_MSG_EQ (target.GetNonce(), randomNonce, "Interest Header nonce deserialization failed");
    
    //NS_TEST_ASSERT_MSG_EQ (target.GetName(), testname, "Interest Header name deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.GetInterestLifetime(), lifetime, "Interest Header lifetime deserialization failed");
    
    //NS_TEST_ASSERT_MSG_EQ (target.GetChildSelector(), child, "Interest Header childselector deserialization failed");
    
    //NS_TEST_ASSERT_MSG_EQ (target.GetExclude(), exclude, "Interest Header exclude deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.GetMaxSuffixComponents(), (int)maxSuffixComponents, "Interest Header maxSuffixComponents deserialization failed");
    
    NS_TEST_ASSERT_MSG_EQ (target.GetMinSuffixComponents(), (int)minSuffixComponents, "Interest Header minSuffixComponents deserialization failed");
}

class InterestHeaderSerializationTestSuite : public TestSuite
{
public:
    InterestHeaderSerializationTestSuite ();
};

InterestHeaderSerializationTestSuite::InterestHeaderSerializationTestSuite ()
: TestSuite ("interest-header-serialization-test-suite", UNIT)
{
    AddTestCase (new InterestHeaderSerializationTest);
}

static InterestHeaderSerializationTestSuite suite;
