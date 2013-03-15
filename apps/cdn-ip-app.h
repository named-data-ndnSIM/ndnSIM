/*
 * cdn-ip-app.h
 *
 *  Created on: Mar 11, 2013
 *      Author: shock
 */

#ifndef CDN_IP_APP_H_
#define CDN_IP_APP_H_
#include "ndn-consumer.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ndn-consumer-cbr.h"
#include "ns3/random-variable.h"

#include "ndn-consumer-zipf-mandelbrot.h"

#include <vector>
using std::vector;


namespace ns3 {
namespace ndn {

class CDNIPApp: public ConsumerZipfMandelbrot
{
public:
  static TypeId GetTypeId ();

  /**
   * \brief Default constructor
   * Sets up randomized Number Generator (RNG)
   * Note: m_seq of its parent class ConsumerCbr here is used to record the interest number
   */
  CDNIPApp ();
  virtual ~CDNIPApp ();


  virtual void SendPacket();

  virtual void
  OnNack (const Ptr<const InterestHeader> &interest, Ptr<Packet> packet);


  /**
   * @brief Timeout event
   * @param sequenceNumber time outed sequence number
   */
  virtual void
  OnTimeout (uint32_t sequenceNumber);



  virtual void
  OnContentObject (const Ptr<const ContentObjectHeader> &contentObject,
                   Ptr<Packet> payload);

  //void   addDataSources(std::string prefix);

  virtual void
  ScheduleNextPacket ();

  void
  AddProducers(NameComponents source);

  void
  SetProducers(std::string prefixes);


  void
  ChangeProducer();

  std::string
  GetProducers() const;


  vector<NameComponents>::size_type pickIndex(NameComponents name);

protected:

private:
  uint32_t m_N;  //number of the contents
  double m_q;  //q in (k+q)^s
  double m_s;  //s in (k+q)^s
  std::vector<double> m_Pcum;  //cumulative probability

  vector<int> sent;
  vector<int> received;

  vector<NameComponents>     m_Producers;        ///< \brief NDN Name of the Interest (use NameComponents)
  uint32_t m_SeqType; // 0 means Cbr, 1 means ZipfMandelbrot
  UniformVariable m_SeqRng; //RNG
};

} /* namespace ndn */
} /* namespace ns3 */
#endif /* CDN_IP_APP_H_ */
