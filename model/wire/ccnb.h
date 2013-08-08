/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * BSD license, See the doc/LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_WIRE_CCNB_H
#define NDN_WIRE_CCNB_H

#include "ns3/ndn-common.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"

NDN_NAMESPACE_BEGIN

namespace wire {

/**
 * @brief Namespace for CCNb wire format operations
 */
namespace ccnb {

/**
  * @brief Routines to serialize/deserialize NDN interest in ccnb format
  *
  * @see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
  **/
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
  
  //////////////////////////////////////////////////////////////////
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
  * @brief Routines to serialize/deserialize NDN Data packet in ccnb format
  *
  * @see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
  **/
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

} // ccnb
} // wire

NDN_NAMESPACE_END

#endif // NDN_WIRE_CCNB_H
