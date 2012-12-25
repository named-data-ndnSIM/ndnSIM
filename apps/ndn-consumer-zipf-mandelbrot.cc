/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Tsinghua University, P.R.China
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Xiaoke Jiang <shock.jiang@gmail.com>
 */

#include "ndn-consumer-zipf-mandelbrot.h"

#include "ns3/ndn-app-face.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"
#include <math.h>
//#include <random-variable.h>


NS_LOG_COMPONENT_DEFINE ("ndn.ConsumerZipfMandelbrot");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (ConsumerZipfMandelbrot);

TypeId
ConsumerZipfMandelbrot::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::ConsumerZipfMandelbrot")
    .SetGroupName ("Ndn")
    .SetParent<ConsumerCbr> ()
    .AddConstructor<ConsumerZipfMandelbrot> ()
    .AddAttribute ("N", "Number of the Contents in total",
                      StringValue ("100"),
                      MakeUintegerAccessor (&ConsumerZipfMandelbrot::m_N),
                      MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("q", "parameter of improve rank",
                      StringValue ("0.7"),
                      MakeDoubleAccessor (&ConsumerZipfMandelbrot::m_q),
                      MakeDoubleChecker<double>())
	  .AddAttribute ("s", "parameter of power",
							StringValue ("0.7"),
							MakeDoubleAccessor (&ConsumerZipfMandelbrot::m_s),
							MakeDoubleChecker<double>())
    ;

  return tid;
}


ConsumerZipfMandelbrot::ConsumerZipfMandelbrot()
	: m_N(100)
	, m_q (0.7)
	, m_s (0.7)
{
	m_Pcum = new double[m_N+1];
	m_Pcum[0] = 0.0;
	for (uint32_t i=1; i<=m_N; i++) {
		m_Pcum[i] = m_Pcum[i-1] + 1.0/pow(i+m_q, m_s);
	}
	for (uint32_t i=1; i<=m_N; i++) {
		m_Pcum[i] = m_Pcum[i] / m_Pcum[m_N];
		NS_LOG_LOGIC("cum Probability ["<<i<<"]="<<m_Pcum[i]);
	}
	//Ptr<UniformRandomVariable> m_SeqRng = CreateObject<UniformRandomVariable> ();
	m_SeqRng = new UniformVariable(0.0, 1.0); //[1, m_N+1)
	//m_SeqRng = new UniformVariable ();
}

ConsumerZipfMandelbrot::~ConsumerZipfMandelbrot() {
	if (m_Pcum) {
		delete m_Pcum;
		m_Pcum = NULL;
	}
//	if (m_SeqRng) {
//		delete m_SeqRng;
//		m_SeqRng = NULL;
//	}
}

void
ConsumerZipfMandelbrot::SendPacket() {
	  if (!m_active) return;

	  NS_LOG_FUNCTION_NOARGS ();

	  uint32_t seq=std::numeric_limits<uint32_t>::max (); //invalid

	  // std::cout << Simulator::Now ().ToDouble (Time::S) << "s max -> " << m_seqMax << "\n";

	  while (m_retxSeqs.size ())
	    {
	      seq = *m_retxSeqs.begin ();
	      m_retxSeqs.erase (m_retxSeqs.begin ());

	      // NS_ASSERT (m_seqLifetimes.find (seq) != m_seqLifetimes.end ());
	      // if (m_seqLifetimes.find (seq)->time <= Simulator::Now ())
	      //   {

	      //     NS_LOG_DEBUG ("Expire " << seq);
	      //     m_seqLifetimes.erase (seq); // lifetime expired. Trying to find another unexpired sequence number
	      //     continue;
	      //   }
	      NS_LOG_DEBUG("=interest seq "<<seq<<" from m_retxSeqs");
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

	      seq = ConsumerZipfMandelbrot::GetNextSeq();
	      m_seq ++;
	    }

	  // std::cout << Simulator::Now ().ToDouble (Time::S) << "s -> " << seq << "\n";

	  //
	  Ptr<NameComponents> nameWithSequence = Create<NameComponents> (m_interestName);
	  (*nameWithSequence) (seq);
	  //

	  InterestHeader interestHeader;
	  interestHeader.SetNonce               (m_rand.GetValue ());
	  interestHeader.SetName                (nameWithSequence);

	  // NS_LOG_INFO ("Requesting Interest: \n" << interestHeader);
	  NS_LOG_INFO ("> Interest for " << seq<<", Total: "<<m_seq<<", face: "<<m_face->GetId());

	  Ptr<Packet> packet = Create<Packet> ();

	  //NS_LOG_DEBUG ("= Interest for " << seq<<", Total: "<<m_seq<<", face: "<<m_face->GetId());
	  packet->AddHeader (interestHeader);
	  //NS_LOG_DEBUG ("Interest packet size: " << packet->GetSize ());

	  NS_LOG_DEBUG ("Trying to add " << seq << " with " << Simulator::Now () << ". already " << m_seqTimeouts.size () << " items");

	  m_seqTimeouts.insert (SeqTimeout (seq, Simulator::Now ()));
	  m_seqLifetimes.insert (SeqTimeout (seq, Simulator::Now () + m_interestLifeTime)); // only one insert will work. if entry exists, nothing will happen... nothing should happen
	  m_transmittedInterests (&interestHeader, this, m_face);

	  m_rtt->SentSeq (SequenceNumber32 (seq), 1);

	  m_protocolHandler (packet);

	 ConsumerZipfMandelbrot::ScheduleNextPacket ();
}


uint32_t
ConsumerZipfMandelbrot::GetNextSeq(){
	uint32_t content_index = 1; //[1, m_N]
	double p_sum = 0;

	double p_random = m_SeqRng->GetValue();
	while (p_random == 0){
		p_random = m_SeqRng->GetValue();
	}
	//if (p_random == 0)
	NS_LOG_LOGIC("p_random="<<p_random);
	for (uint32_t i=1; i<=m_N; i++) {
		p_sum = m_Pcum[i];   //m_Pcum[i] = m_Pcum[i-1] + p[i], p[0] = 0;   e.g.: p_cum[1] = p[1], p_cum[2] = p[1] + p[2]
		if (p_random <= p_sum) {
			content_index = i;
			break;
		} //if
	} //for
	//content_index = 1;
	NS_LOG_DEBUG("RandomNumber="<<content_index);
	return content_index;
}

void
ConsumerZipfMandelbrot::ScheduleNextPacket() {

	  if (m_firstTime)
	    {
	      m_sendEvent = Simulator::Schedule (Seconds (0.0),
	                                         &ConsumerZipfMandelbrot::SendPacket, this);
	      m_firstTime = false;
	    }
	  else if (!m_sendEvent.IsRunning ())
	    m_sendEvent = Simulator::Schedule (
	                                       (m_random == 0) ?
	                                         Seconds(1.0 / m_frequency)
	                                       :
	                                         Seconds(m_random->GetValue ()),
	                                       &ConsumerZipfMandelbrot::SendPacket, this);
}

} /* namespace ndn */
} /* namespace ns3 */
