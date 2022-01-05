#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

  int
    main(int argc, char* argv[])
  {
    CommandLine cmd;
    cmd.Parse(argc, argv);

    AnnotatedTopologyReader topologyReader("", 1);
    topologyReader.SetFileName("src/ndnSIM/examples/topologies/n50.txt");
    topologyReader.Read();

    // Install NDN stack on all nodes
    ndn::StackHelper ndnHelper;
    //set lru for replacement
    ndnHelper.setPolicy("nfd::cs::lru");
    // set 100 packet for cache size in Content Store
    ndnHelper.setCsSize(100);
    ndnHelper.InstallAll();

    // Choosing forwarding strategy
    ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/best-route");

    // Installing global routing interface on all nodes
    ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
    ndnGlobalRoutingHelper.InstallAll();

    // Getting containers for the consumer/producer
    Ptr<Node> consumers[34] = { Names::Find<Node>("c1"), Names::Find<Node>("c2"),
                              Names::Find<Node>("c3"), Names::Find<Node>("c4"),
                              Names::Find<Node>("c5"), Names::Find<Node>("c6"),
                              Names::Find<Node>("c7"), Names::Find<Node>("c8"),
                              Names::Find<Node>("c9"), Names::Find<Node>("c10"),
                              Names::Find<Node>("c11"), Names::Find<Node>("c12"),
                              Names::Find<Node>("c13"), Names::Find<Node>("c14"),
                              Names::Find<Node>("c15"), Names::Find<Node>("c16"),
                              Names::Find<Node>("c17"), Names::Find<Node>("c18"),
                              Names::Find<Node>("c19"), Names::Find<Node>("c20"),
                              Names::Find<Node>("c1"), Names::Find<Node>("c2"),
                              Names::Find<Node>("c1"), Names::Find<Node>("c2"),
                              Names::Find<Node>("c33"), Names::Find<Node>("c34") };
    Ptr<Node> producers[5] = { Names::Find<Node>("s1"), Names::Find<Node>("s2"),
                              Names::Find<Node>("s3"), Names::Find<Node>("s4"),
                              Names::Find<Node>("s5") };

    for (int i = 0; i < 4; i++) {
      std::string prefix = "/data/" + Names::FindName(producers[i]);

      /////////////////////////////////////////////////////////////////////////////////
      // install consumer app on consumer node c_i to request data from producer p_i //
      /////////////////////////////////////////////////////////////////////////////////

      ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
      consumerHelper.SetAttribute("Frequency", StringValue("10")); // 100 interests a second

      consumerHelper.SetPrefix(prefix);
      ApplicationContainer consumer = consumerHelper.Install(consumers[i]);
      consumer.Start(Seconds(i));     // start consumers at 0s, 1s, 2s, 3s
      consumer.Stop(Seconds(19 - i)); // stop consumers at 19s, 18s, 17s, 16s

      ///////////////////////////////////////////////
      // install producer app on producer node p_i //
      ///////////////////////////////////////////////

      ndn::AppHelper producerHelper("ns3::ndn::Producer");
      producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

      // install producer that will satisfy Interests in /dst1 namespace
      producerHelper.SetPrefix(prefix);
      ApplicationContainer producer = producerHelper.Install(producers[i]);
      // when Start/Stop time is not specified, the application is running throughout the simulation
    }

    // for (int i = 0; i < 5; i++) {
    //   ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
    //   consumerHelper.SetAttribute("Frequency", StringValue("10")); // 100 interests a second

    //   // Each consumer will express the same data /root/<seq-no>
    //   consumerHelper.SetPrefix("/root");
    //   ApplicationContainer app = consumerHelper.Install(consumers[i]);
    //   app.Start(Seconds(0.01 * i));
    // }

    // ndn::AppHelper producerHelper("ns3::ndn::Producer");
    // producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

    // // Register /root prefix with global routing controller and
    // // install producer that will satisfy Interests in /root namespace
    // ndnGlobalRoutingHelper.AddOrigins("/root", producers);
    // producerHelper.SetPrefix("/root");
    // producerHelper.Install(producers);

    // Calculate and install FIBs
    ndn::GlobalRoutingHelper::CalculateRoutes();

    Simulator::Stop(Seconds(20.0));

    // tracer --------- it for rate trace and drop trace and delay
    ndn::L3RateTracer::InstallAll("output/rate-trace.txt", Seconds(0.5));
    L2RateTracer::InstallAll("output/drop-trace.txt", Seconds(0.5));
    ndn::AppDelayTracer::InstallAll("output/app-delays-trace.txt");
    // for cache hit rate
    ndn::CsTracer::InstallAll("cs-trace.txt", Seconds(1));

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