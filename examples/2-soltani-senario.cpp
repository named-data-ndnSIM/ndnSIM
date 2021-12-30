

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {


  int
    main(int argc, char* argv[])
  {
    CommandLine cmd;
    cmd.Parse(argc, argv);

    AnnotatedTopologyReader topologyReader("", 25);
    topologyReader.SetFileName("src/ndnSIM/examples/topologies/n50.txt");
    topologyReader.Read();

    // Install NDN stack on all nodes
    ndn::StackHelper ndnHelper;
    ndnHelper.InstallAll();

    // Set BestRoute strategy
    ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

    // Installing global routing interface on all nodes
    ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
    ndnGlobalRoutingHelper.InstallAll();

    // Getting containers for the consumer/producer
    Ptr<Node> producer = Names::Find<Node>("s1");
    Ptr<Node> producer = Names::Find<Node>("s2");
    Ptr<Node> producer = Names::Find<Node>("s3");
    Ptr<Node> producer = Names::Find<Node>("s4");
    NodeContainer consumerNodes;
    consumerNodes.Add(Names::Find<Node>("c1"));
    consumerNodes.Add(Names::Find<Node>("c2"));
    consumerNodes.Add(Names::Find<Node>("c3"));
    consumerNodes.Add(Names::Find<Node>("c4"));
    consumerNodes.Add(Names::Find<Node>("c5"));
    consumerNodes.Add(Names::Find<Node>("c6"));
    consumerNodes.Add(Names::Find<Node>("c7"));
    consumerNodes.Add(Names::Find<Node>("c8"));
    consumerNodes.Add(Names::Find<Node>("c9"));
    consumerNodes.Add(Names::Find<Node>("c10"));
    consumerNodes.Add(Names::Find<Node>("c11"));
    consumerNodes.Add(Names::Find<Node>("c12"));

    // Install NDN applications
    std::string prefix = "/prefix";

    ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
    consumerHelper.SetPrefix(prefix);
    consumerHelper.SetAttribute("Frequency", StringValue("100")); // 100 interests a second
    consumerHelper.Install(consumerNodes);

    ndn::AppHelper producerHelper("ns3::ndn::Producer");
    producerHelper.SetPrefix(prefix);
    producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper.Install(producer);

    // Add /prefix origins to ndn::GlobalRouter
    ndnGlobalRoutingHelper.AddOrigins(prefix, producer);

    // Calculate and install FIBs
    ndn::GlobalRoutingHelper::CalculateRoutes();

    Simulator::Stop(Seconds(20.0));

    // tracer --------- it for rate trace and drop trace
    ndn::L3RateTracer::InstallAll("output/rate-trace.txt", Seconds(0.5));
    L2RateTracer::InstallAll("output/drop-trace.txt", Seconds(0.5));
    ndn::AppDelayTracer::InstallAll("output/app-delays-trace.txt");

    Simulator::Run();
    Simulator::Destroy();

    return 0;
  }

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
