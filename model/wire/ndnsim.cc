/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * GNU 3.0 license, See the LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndnsim.h"

using namespace std;

#include <ns3/header.h>
#include <ns3/packet.h>
#include <ns3/log.h>

#include "ndnsim/wire-ndnsim.h"

NS_LOG_COMPONENT_DEFINE ("ndn.wire.ndnSIM");

NDN_NAMESPACE_BEGIN

namespace wire {
namespace ndnSIM {

NS_OBJECT_ENSURE_REGISTERED (Interest);
NS_OBJECT_ENSURE_REGISTERED (Data);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Interest::Interest ()
  : m_interest (Create<ndn::Interest> ())
{
}

Interest::Interest (Ptr<ndn::Interest> interest)
  : m_interest (interest)
{
}

Ptr<ndn::Interest>
Interest::GetInterest ()
{
  return m_interest;
}


TypeId
Interest::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::Interest::ndnSIM")
    .SetGroupName ("Ndn")
    .SetParent<Header> ()
    .AddConstructor<Interest> ()
    ;
  return tid;
}

TypeId
Interest::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

Ptr<Packet>
Interest::ToWire (Ptr<const ndn::Interest> interest)
{
  Ptr<const Packet> p = interest->GetWire ();
  if (!p)
    {
      Ptr<Packet> packet = Create<Packet> (*interest->GetPayload ());
      Interest wireEncoding (ConstCast<ndn::Interest> (interest));
      packet->AddHeader (wireEncoding);
      interest->SetWire (packet);

      p = packet;
    }
  
  return p->Copy ();
}

Ptr<ndn::Interest>
Interest::FromWire (Ptr<Packet> packet)
{
  Ptr<ndn::Interest> interest = Create<ndn::Interest> ();
  Ptr<Packet> wire = packet->Copy ();

  Interest wireEncoding (interest);
  packet->RemoveHeader (wireEncoding);

  interest->SetPayload (packet);
  interest->SetWire (wire);

  return interest;
}

uint32_t
Interest::GetSerializedSize (void) const
{
  size_t size =
    1/*version*/ + 1 /*type*/ + 2/*length*/ +
    (4/*nonce*/ + 1/*scope*/ + 1/*nack type*/ + 2/*timestamp*/ +
     NdnSim::SerializedSizeName (m_interest->GetName ()) +

     (2 +
      (m_interest->GetExclude () == 0 ? 0 : (1 + NdnSim::SerializedSizeExclude (*m_interest->GetExclude ())))
      )/* selectors */ +
     
     (2 + 0)/* options */);
  
  NS_LOG_INFO ("Serialize size = " << size);
  return size;
}
    
void
Interest::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (0x80); // version
  start.WriteU8 (0x00); // packet type

  start.WriteU16 (GetSerializedSize () - 4);

  start.WriteU32 (m_interest->GetNonce ());
  start.WriteU8 (m_interest->GetScope ());
  start.WriteU8 (m_interest->GetNack ());

  NS_ASSERT_MSG (0 <= m_interest->GetInterestLifetime ().ToInteger (Time::S) && m_interest->GetInterestLifetime ().ToInteger (Time::S) < 65535,
                 "Incorrect InterestLifetime (should not be smaller than 0 and larger than 65535");
  
  // rounding timestamp value to seconds
  start.WriteU16 (static_cast<uint16_t> (m_interest->GetInterestLifetime ().ToInteger (Time::S)));

  NdnSim::SerializeName (start, m_interest->GetName ());

  if (m_interest->GetExclude () == 0)
    {
      start.WriteU16 (0); // no selectors
    }
  else
    {
      start.WriteU16 (1 + NdnSim::SerializedSizeExclude (*m_interest->GetExclude ()));
      start.WriteU8 (0x01);
      NdnSim::SerializeExclude (start, *m_interest->GetExclude ());
    }
  
  start.WriteU16 (0); // no options
}

uint32_t
Interest::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  
  if (i.ReadU8 () != 0x80)
    throw new InterestException ();

  if (i.ReadU8 () != 0x00)
    throw new InterestException ();

  i.ReadU16 (); // length, don't need it right now
  
  m_interest->SetNonce (i.ReadU32 ());
  m_interest->SetScope (i.ReadU8 ());
  m_interest->SetNack (i.ReadU8 ());

  m_interest->SetInterestLifetime (Seconds (i.ReadU16 ()));

  m_interest->SetName (NdnSim::DeserializeName (i));
  
  uint32_t selectorsLen = i.ReadU16 ();
  if (selectorsLen > 0)
    {
      if (i.ReadU8 () != 0x01) // exclude filter only
        throw InterestException ();

      m_interest->SetExclude (NdnSim::DeserializeExclude (i));
    }
  i.ReadU16 ();

  NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

  return i.GetDistanceFrom (start);
}

void
Interest::Print (std::ostream &os) const
{
  m_interest->Print (os);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


TypeId
Data::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::Data::ndnSIM")
    .SetGroupName ("Ndn")
    .SetParent<Header> ()
    .AddConstructor<Data> ()
    ;
  return tid;
}

TypeId
Data::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  

Data::Data ()
  : m_data (Create<ndn::Data> ())
{
}

Data::Data (Ptr<ndn::Data> data)
  : m_data (data)
{
}

Ptr<ndn::Data>
Data::GetData ()
{
  return m_data;
}

Ptr<Packet>
Data::ToWire (Ptr<const ndn::Data> data)
{
  Ptr<const Packet> p = data->GetWire ();
  if (!p)
    {
      Ptr<Packet> packet = Create<Packet> (*data->GetPayload ());
      Data wireEncoding (ConstCast<ndn::Data> (data));
      packet->AddHeader (wireEncoding);
      data->SetWire (packet);

      p = packet;
    }
  
  return p->Copy ();
}

Ptr<ndn::Data>
Data::FromWire (Ptr<Packet> packet)
{
  Ptr<ndn::Data> data = Create<ndn::Data> ();
  Ptr<Packet> wire = packet->Copy ();

  Data wireEncoding (data);
  packet->RemoveHeader (wireEncoding);

  data->SetPayload (packet);
  data->SetWire (wire);

  return data;
}

uint32_t
Data::GetSerializedSize () const
{
  uint32_t size = 1 + 1 + 2 +
    ((2 + 2) +
     NdnSim::SerializedSizeName (m_data->GetName ()) +
     (2 + 2 + 4 + 2 + 2 + (2 + 0)));
  if (m_data->GetSignature () != 0)
    size += 4;
  
  NS_LOG_INFO ("Serialize size = " << size);
  return size;
}

void
Data::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (0x80); // version
  start.WriteU8 (0x01); // packet type
  start.WriteU16 (GetSerializedSize () - 4); // length
  
  if (m_data->GetSignature () != 0)
    {
      start.WriteU16 (6); // signature length
      start.WriteU16 (0xFF00); // "fake" simulator signature
      start.WriteU32 (m_data->GetSignature ());
    }
  else
    {
      start.WriteU16 (2); // signature length
      start.WriteU16 (0); // empty signature
    }

  // name
  NdnSim::SerializeName (start, m_data->GetName ());

  // content
  // for now assume that contentdata length is zero
  start.WriteU16 (2 + 4 + 2 + 2 + (2 + 0));
  start.WriteU16 (4 + 2 + 2 + (2 + 0));
  start.WriteU32 (static_cast<uint32_t> (m_data->GetTimestamp ().ToInteger (Time::S)));
  start.WriteU16 (static_cast<uint16_t> (m_data->GetFreshness ().ToInteger (Time::S)));
  start.WriteU16 (0); // reserved 
  start.WriteU16 (0); // Length (ContentInfoOptions)

  // that's it folks
}

uint32_t
Data::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  if (i.ReadU8 () != 0x80)
    throw new DataException ();

  if (i.ReadU8 () != 0x01)
    throw new DataException ();

  i.ReadU16 (); // length

  uint32_t signatureLength = i.ReadU16 ();
  if (signatureLength == 6)
    {
      if (i.ReadU16 () != 0xFF00) // signature type
        throw new DataException ();
      m_data->SetSignature (i.ReadU32 ());
    }
  else if (signatureLength == 2)
    {
      if (i.ReadU16 () != 0) // signature type
        throw new DataException ();
      m_data->SetSignature (0);
    }
  else
    throw new DataException ();

  m_data->SetName (NdnSim::DeserializeName (i));

  if (i.ReadU16 () != (2 + 4 + 2 + 2 + (2 + 0))) // content length
    throw new DataException ();

  if (i.ReadU16 () != (4 + 2 + 2 + (2 + 0))) // Length (content Info)
    throw new DataException ();

  m_data->SetTimestamp (Seconds (i.ReadU32 ()));
  m_data->SetFreshness (Seconds (i.ReadU16 ()));

  if (i.ReadU16 () != 0) // Reserved
    throw new DataException ();
  if (i.ReadU16 () != 0) // Length (ContentInfoOptions)
    throw new DataException ();

  NS_ASSERT_MSG (i.GetDistanceFrom (start) == GetSerializedSize (),
                 "Something wrong with Data::Deserialize");
  
  return i.GetDistanceFrom (start);
}

void
Data::Print (std::ostream &os) const
{
  m_data->Print (os);
}

} // ndnSIM
} // wire

NDN_NAMESPACE_END
