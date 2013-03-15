/*
 * cdn-ip-app.cc
 *
 *  Created on: Mar 11, 2013
 *      Author: shock
 */

#include "cdn-ip-app.h"

#include "ns3/ndn-app-face.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"

#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"
#include "ndn-consumer-zipf-mandelbrot.h"
#include <sstream>
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("ndn.CDNIPApp");

using namespace std;

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (CDNIPApp);

TypeId
CDNIPApp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::CDNIPApp")
    .SetGroupName ("Ndn")
    .SetParent<ConsumerZipfMandelbrot> ()
    .AddConstructor<CDNIPApp> ()

    .AddAttribute ("SeqType", "Type of Interest Seq. 0=Cbr, 1=ZipfMandelbrot",
                       StringValue ("0"),
                       MakeDoubleAccessor (&CDNIPApp::m_SeqType),
                       MakeUintegerChecker<uint32_t> ())
   .AddAttribute ("Producers", "Data Producers",
						  StringValue ("0"),
						  //MakeDoubleAccessor (&CDNIPApp::m_SeqType),
						  //MakeUintegerChecker<uint32_t> ())
						  //MakeTimeAccessor (&Consumer::GetRetxTimer, &Consumer::SetRetxTimer),
						  //MakeNameComponentsAccessor (&CDNIPApp::m_interestName),

						  MakeStringAccessor (&CDNIPApp::GetProducers, &CDNIPApp::SetProducers),
						  MakeStringChecker ())
//						  MakeObjectVectorAccessor (&CDNIPApp::m_Producers),
//						  MakeObjectVectorChecker ())
						  //MakeNameComponentsAccessor (&CDNIPApp::GetDataSources, &CDNIPApp::SetDataSources),
						  //MakeNameComponentsChecker ())

    ;

  return tid;
}


CDNIPApp::CDNIPApp()
	: m_SeqType (0)
{
//	vector<NameComponents>::size_type ix = 0;
//	for (; ix !=m_Producers.size(); ++ix) {
//		if (m_Producers[ix] == m_interestName) {
//			break;
//		}
//	}
//	if (ix == m_Producers.size()) {
//		m_Producers.push_back(m_interestName);
//	}

  // SetNumberOfContents is called by NS-3 object system during the initialization
}


std::string
CDNIPApp::GetProducers() const
{

	stringstream str("");

	for (vector<NameComponents>::size_type ix=0; ix !=m_Producers.size(); ++ix){
		NameComponents name = m_Producers[ix];
		name.Print(str);
	}
    NS_LOG_LOGIC("Producers are="<<str.str());
	return str.str();
}

void
CDNIPApp::AddProducers(NameComponents producer){
	//NameComponents source = NameComponents(prefix);

		vector<NameComponents>::size_type ix = 0;
		for (; ix <m_Producers.size(); ++ix) {
			if (m_Producers[ix] == producer) {
				break;
			}
		}
		if (ix == m_Producers.size()) {
			m_Producers.push_back(producer);
			m_interestName = producer;
			sent.push_back(0);
			received.push_back(0);
            NS_LOG_LOGIC("add Data provider "<<producer);
		}

}

void
CDNIPApp::SetProducers(std::string prefixes){
	//prefixes.
	std::string s = prefixes;
	char seperator = ';';

	std::string::size_type prev_pos = 0, pos = 0;
	NameComponents name;
	while((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring( s.substr(prev_pos, pos-prev_pos) );

		 name = NameComponents(substring);

		AddProducers(name);
		prev_pos = ++pos;
	}

	name = NameComponents(s.substr(prev_pos, pos-prev_pos)); // Last word
	AddProducers(name);
}




CDNIPApp::~CDNIPApp()
{
}


void
CDNIPApp::OnNack (const Ptr<const InterestHeader> &interest, Ptr<Packet> packet){
	ChangeProducer();
	Consumer::OnNack(interest, packet);
}

void
CDNIPApp::OnTimeout (uint32_t sequenceNumber) {
	//ConsumerZipfMandelbrot::OnNack();
	//m_InterestName = m_Producers.
	ChangeProducer();
	Consumer::OnTimeout(sequenceNumber);
}


void
CDNIPApp::OnContentObject (const Ptr<const ContentObjectHeader> &contentObject,
                   Ptr<Packet> payload) {
	NameComponents name = contentObject->GetName();
	Name::const_iterator iter = name.begin();
	Name::const_iterator next = iter;
	next ++;
	NameComponents source;
	while (next != name.end()) {
		source.Add(*iter);

		iter ++;
		next ++;
	}
	NS_LOG_LOGIC("ContentName="<<contentObject->GetName()<<". source="<<source);
	vector<NameComponents>::size_type ix = CDNIPApp::pickIndex(source);
	received[ix] += 1;
	Consumer::OnContentObject (contentObject, payload);
}

vector<NameComponents>::size_type
CDNIPApp::ChangeProducer(){
	vector<NameComponents>::size_type ix = 0;
	vector<NameComponents>::size_type cur = 0;
//	if (sent.empty() || received.empty()) {
//		for (vector<NameComponents>::size_type i=0; i<m_Producers.size(); i++) {
//			sent.push_back(0);
//			received.push_back(0);
//		}
//	}
	float getrate = -1;
	ix = 0;
	for (; ix !=m_Producers.size(); ++ix) {
		if (sent[ix] == 0){
			m_interestName = m_Producers[ix];
			cur = ix;
			NS_LOG_LOGIC("sent["<<ix<<"]==0");
			break;
		} else {
			float t = received[ix]/float(sent[ix]);

			if (t > getrate) {
				getrate = t;
				cur = ix;
			}
		}
	}

	if (m_interestName == m_Producers[cur]) {
		NS_LOG_INFO("= Hold Data Producer. datasource.size= "<<m_Producers.size());
	}else {
		NS_LOG_INFO("! Change Data Producer to "<<cur);
		m_interestName = m_Producers[cur];
	}

	return cur;
}

vector<NameComponents>::size_type
CDNIPApp::pickIndex(NameComponents name) {
	vector<NameComponents>::size_type ix = 0;
		for (; ix !=m_Producers.size(); ++ix) {
			if (m_Producers[ix] == name) break;
			else {
				NS_LOG_LOGIC("DataSource="<<m_Producers[ix]);
			}
		}

		if (ix == m_Producers.size()) {
			ix = ChangeProducer();
			NS_LOG_ERROR("index for producer: "<<name<<" can not find in Providers. change it to "<<ix);
			return ix;

		} else{
			NS_LOG_LOGIC("index for producer: "<<name<<" is "<<ix);
			return ix;
		}

}

void
CDNIPApp::SendPacket() {

	  if (!m_active) return;

	  NS_LOG_FUNCTION_NOARGS ();


	  // add by Shock
	  vector<NameComponents>::size_type t =   CDNIPApp::pickIndex(m_interestName); //if not in, will change to in
	  sent[t] += 1;
	  NS_LOG_LOGIC("additon after: sent["<<t<<"]="<<sent[t]);


	  uint32_t seq=std::numeric_limits<uint32_t>::max (); //invalid

	  // std::cout << Simulator::Now ().ToDouble (Time::S) << "s max -> " << m_seqMax << "\n";

	  while (m_retxSeqs.size ())
	    {
	      seq = *m_retxSeqs.begin ();
	      m_retxSeqs.erase (m_retxSeqs.begin ());

	      // NS_ASSERT (m_seqLifetimes.find (seq) != m_seqLifetimes.end ());
	      // if (m_seqLifetimes.find (seq)->time <= Simulator::Now ())
	      //   {

	      //     NS_LOG_LOGIC ("Expire " << seq);
	      //     m_seqLifetimes.erase (seq); // lifetime expired. Trying to find another unexpired sequence number
	      //     continue;
	      //   }
	      NS_LOG_LOGIC("=interest seq "<<seq<<" from m_retxSeqs");
	      break;
	    }

	  if (seq == std::numeric_limits<uint32_t>::max ()) //no retransmission
	    {
	      if (m_seqMax != std::numeric_limits<uint32_t>::max ())
	        {
	          if (m_seq >= m_seqMax)
	            {
	              return; // we are totally done
	            }
	        }

	      if (m_SeqType == 1) {
			  seq = ConsumerZipfMandelbrot::GetNextSeq();
			  m_seq ++;
	      } else {
	    	  seq = m_seq ++ ;
	      }

	    }

	  // std::cout << Simulator::Now ().ToDouble (Time::S) << "s -> " << seq << "\n";




	  Ptr<NameComponents> nameWithSequence = Create<NameComponents> (m_interestName);
	  (*nameWithSequence) (seq);
	  //

	  InterestHeader interestHeader;
	  interestHeader.SetNonce (m_rand.GetValue ());
	  interestHeader.SetName  (nameWithSequence);

	  // NS_LOG_INFO ("Requesting Interest: \n" << interestHeader);
	  NS_LOG_INFO ("> Interest for " << seq<<", Total: "<<m_seq<<", face: "<<m_face->GetId());

	  Ptr<Packet> packet = Create<Packet> ();

	  //NS_LOG_LOGIC ("= Interest for " << seq<<", Total: "<<m_seq<<", face: "<<m_face->GetId());
	  packet->AddHeader (interestHeader);






	  //NS_LOG_LOGIC ("Interest packet size: " << packet->GetSize ());

	  NS_LOG_LOGIC ("Trying to add " << seq << " with " << Simulator::Now () << ". already " << m_seqTimeouts.size () << " items");

	  m_seqTimeouts.insert (SeqTimeout (seq, Simulator::Now ()));
	  m_seqFullDelay.insert (SeqTimeout (seq, Simulator::Now ()));

	  m_seqLastDelay.erase (seq);
	  m_seqLastDelay.insert (SeqTimeout (seq, Simulator::Now ()));

	  m_seqRetxCounts[seq] ++;

	  m_transmittedInterests (&interestHeader, this, m_face);

	  m_rtt->SentSeq (SequenceNumber32 (seq), 1);

	  FwHopCountTag hopCountTag;
	  packet->AddPacketTag (hopCountTag);

	  m_protocolHandler (packet);

	  CDNIPApp::ScheduleNextPacket ();




//  if (!m_active) return;
//  NS_LOG_FUNCTION_NOARGS ();
//
//
//  // add by Shock
//  vector<NameComponents>::size_type t =   CDNIPApp::pickIndex(m_interestName);
//  sent[t] += 1;
//
//  if (m_SeqType == 0) {
//	  ConsumerCbr::SendPacket();
//  }
//  else if (m_SeqType == 1) {
//	  ConsumerZipfMandelbrot::SendPacket();
//  }


}


void
CDNIPApp::ScheduleNextPacket ()
{
  // double mean = 8.0 * m_payloadSize / m_desiredRate.GetBitRate ();
  // std::cout << "next: " << Simulator::Now().ToDouble(Time::S) + mean << "s\n";

  if (m_firstTime)
    {
      m_sendEvent = Simulator::Schedule (Seconds (0.0),
                                         &CDNIPApp::SendPacket, this);
      m_firstTime = false;
    }
  else if (!m_sendEvent.IsRunning ())
    m_sendEvent = Simulator::Schedule (
                                       (m_random == 0) ?
                                         Seconds(1.0 / m_frequency)
                                       :
                                         Seconds(m_random->GetValue ()),
                                       &CDNIPApp::SendPacket, this);

}

} /* namespace ndn */
} /* namespace ns3 */
