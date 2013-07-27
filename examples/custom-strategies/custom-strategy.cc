/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
// custom-strategy.cc

#include "custom-strategy.h"
#include "ns3/ndn-fib.h"
#include "ns3/ndn-fib-entry.h"
#include "ns3/ndn-pit-entry.h"
#include "ns3/ndn-interest.h"

namespace ns3 {
namespace ndn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED(CustomStrategy);

LogComponent CustomStrategy::g_log = LogComponent (CustomStrategy::GetLogName ().c_str ());

std::string
CustomStrategy::GetLogName ()
{
  return "ndn.fw.CustomStrategy";
}

TypeId
CustomStrategy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::CustomStrategy")
    .SetGroupName ("Ndn")
    .SetParent <BaseStrategy> ()
    .AddConstructor <CustomStrategy> ()

    // .AddAttribute ("Attribute", "Attribute spec",
    //                         StringValue ("DefaultValue"),
    //                         MakeStringAccessor (&BaseStrategy::m_variable),
    //                         MakeStringChecker ())
    ;
  return tid;
}

CustomStrategy::CustomStrategy ()
  : m_counter (0)
{
}

bool
CustomStrategy::DoPropagateInterest (Ptr<Face> inFace,
                                     Ptr<const Interest> interest,
                                     Ptr<pit::Entry> pitEntry)
{
  typedef fib::FaceMetricContainer::type::index<fib::i_metric>::type FacesByMetric;
  FacesByMetric &faces = pitEntry->GetFibEntry ()->m_faces.get<fib::i_metric> ();
  FacesByMetric::iterator faceIterator = faces.begin ();

  int propagatedCount = 0;

  // forward to best-metric face
  if (faceIterator != faces.end ())
    {
      if (TrySendOutInterest (inFace, faceIterator->GetFace (), interest, pitEntry))
        propagatedCount ++;

      faceIterator ++;
    }

  // forward to second-best-metric face
  if (faceIterator != faces.end ())
    {
      if (TrySendOutInterest (inFace, faceIterator->GetFace (), interest, pitEntry))
        propagatedCount ++;

      faceIterator ++;
    }
  return propagatedCount > 0;
}

void
CustomStrategy::DidSendOutInterest (Ptr<Face> inFace, Ptr<Face> outFace,
                                    Ptr<const Interest> interest,
                                    Ptr<pit::Entry> pitEntry)
{
  m_counter ++;
}

void
CustomStrategy::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      m_counter --;
    }

  BaseStrategy::WillEraseTimedOutPendingInterest (pitEntry);
}


void
CustomStrategy::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                            Ptr<pit::Entry> pitEntry)
{
  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      m_counter --;
    }

  BaseStrategy::WillSatisfyPendingInterest (inFace, pitEntry);
}


} // namespace fw
} // namespace ndn
} // namespace ns3
