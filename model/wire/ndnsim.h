/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * GNU 3.0 license, See the LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_WIRE_NDNSIM_H
#define NDN_WIRE_NDNSIM_H

#include "ns3/ndn-common.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"

NDN_NAMESPACE_BEGIN

/**
 * @brief Namespace encapsulating wire operations
 */
namespace wire {

/**
 * @brief Namespace for ndnSIM wire format operations
 */
namespace ndnSIM {

/**
 * @brief Routines to serialize/deserialize Interest packet in ndnSIM format
 *
 * Optimized and simplified formatting of Interest packets 
 *
 *	Interest ::= Nonce 
 *	     	     Scope 
 *		     InterestLifetime 
 *	     	     Name 
 *	     	     Selectors 
 *	     	     Options
 *
 * Minumum size of the Interest packet: 1 + 4 + 2 + 1 + (2 + 0) + (2 + 0) + (2 + 0) = 14
 *
 * Maximum size of the Interest packet: 1 + 4 + 2 + 1 + (2 + 65535) + (2 + 65535) + (2 + 65535) = 196619
 *
 * ::
 *
 *        0                   1                   2                   3
 *        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *        |                          Nonce                                |
 *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *        |     Scope     |   Reserved    |      InterestLifetime         |
 *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *        |            Length             |                               |
 *	   |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *        ~                                                               ~
 *        ~                            Name                               ~
 *        |							           |	
 *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *        |            Length             |                               |
 *        |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *        ~                                                               ~
 *        ~                          Selectors                            ~
 *        |							            |	
 *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *        |            Length             |                               |
 *	   |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *        ~                                                               ~
 *        ~                          Options                              ~
 *        |							           |	
 *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class Interest : public Header
{
public:
  Interest ();
  Interest (Ptr<ndn::Interest> interest);

  Ptr<ndn::Interest>
  GetInterest ();

  static Ptr<Packet>
  ToWire (Ptr<const ndn::Interest> interest);

  static Ptr<ndn::Interest>
  FromWire (Ptr<Packet> packet);
  
  // from Header
  static TypeId GetTypeId (void); 
  virtual TypeId GetInstanceTypeId (void) const; 
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  Ptr<ndn::Interest> m_interest;
};

/**
 * @brief Routines to serialize/deserialize Data packet in ndnSIM format
 *
 * Only few important fields are actually implemented in the simulation
 *
 * @see http://ndnsim.net/new-packet-formats.html
 *
 *	Data ::= Signature
 *                	  Name
 *                   	  Content
 *
 *      0                   1                   2                   3
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Length             |                               |
 *      |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
 *      ~                                                               ~
 *      ~                           Signature                           ~
 *      |							        |	
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Length             |                               |
 *      |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *      ~                                                               ~
 *      ~                             Name                              ~
 *      |							        |	
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Length             |                               |
 *      |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *      ~                                                               ~
 *      ~                           Content                             ~
 *      |							        |	
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
class Data : public Header
{
public:
  Data ();
  Data (Ptr<ndn::Data> data);

  Ptr<ndn::Data>
  GetData ();

  static Ptr<Packet>
  ToWire (Ptr<const ndn::Data> data);

  static Ptr<ndn::Data>
  FromWire (Ptr<Packet> packet);
  
  // from Header
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  Ptr<ndn::Data> m_data;  
};

} // ndnSIM
} // wire

NDN_NAMESPACE_END

#endif // NDN_WIRE_NDNSIM_H
