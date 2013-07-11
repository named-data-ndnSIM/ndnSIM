/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * BSD license, See the doc/LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndnsim.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace ndnSIM {


class Name
{
public:
  Name ()
    : m_name (Create<ndn::Name> ())
  {
  }
  
  Name (Ptr<ndn::Name> name)
    : m_name (name)
  {
  }

  Ptr<Name>
  GetName ()
  {
    return m_name;
  }
  
  size_t
  GetSerializedSize () const
  {
    size_t nameSerializedSize = 2;

    for (std::list<std::string>::const_iterator i = m_name->begin ();
         i != m_name->end ();
         i++)
      {
        nameSerializedSize += 2 + i->size ();
      }
    NS_ASSERT_MSG (nameSerializedSize < 30000, "Name is too long (> 30kbytes)");

    return nameSerializedSize;
  }

  uint32_t
  Serialize (Buffer::Iterator start) const
  {
    Buffer::Iterator i = start;

    i.WriteU16 (static_cast<uint16_t> (m_name->GetSerializedSize ()-2));

    for (std::list<std::string>::const_iterator item = m_name->begin ();
         item != m_name->end ();
         item++)
      {
        i.WriteU16 (static_cast<uint16_t> (item->size ()));
        i.Write (reinterpret_cast<const uint8_t*> (item->c_str ()), item->size ());
      }

    return i.GetDistanceFrom (start);
  }

  uint32_t
  Deserialize (Buffer::Iterator start)
  {
    Buffer::Iterator i = start;

    uint16_t nameLength = i.ReadU16 ();
    while (nameLength > 0)
      {
        uint16_t length = i.ReadU16 ();
        nameLength = nameLength - 2 - length;

        uint8_t tmp[length];
        i.Read (tmp, length);

        m_name->Add (string (reinterpret_cast<const char*> (tmp), length));
      }

    return i.GetDistanceFrom (start);
  }

private:
  Ptr<ndn::Name> m_name;
};

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
    .AddParent<Header> ()
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
      p = Create<Packet> (*interest->GetPayload ());
      Interest wireEncoding (interest);
      p->AddHeader (wireEncoding);
      interest->SetWire (p);
    }
  
  return p->Copy ();
}

Ptr<ndn::Interest>
Interest::FromWire (Ptr<Packet> packet)
{
  Ptr<ndn::Interest> interest = Create<ndn::Interest> ();
  // interest->SetWire (packet->Copy ()); /* not sure if this is needed */

  Interest wireEncoding (interest);
  packet->RemoveHeader (wireEncoding);

  interest->SetPayload (packet);

  return interest;
}

uint32_t
Interest::GetSerializedSize (void) const
{
  size_t size =
    1/*version*/ + 1 /*type*/ + 2/*length*/ +
    (4/*nonce*/ + 1/*scope*/ + 1/*nack type*/ + 2/*timestamp*/ +
     (Name (ConstCast<ns3::Name> (m_interest->GetNamePtr ())).GetSerializedSize ()) +
     (2 + 0)/* selectors */ +
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

  start.WriteU32 (m_nonce);
  start.WriteU8 (m_scope);
  start.WriteU8 (m_nackType);

  NS_ASSERT_MSG (0 <= m_interest->GetInterestLifetime.ToInteger (Time::S) && m_interest->GetInterestLifetime.ToInteger (Time::S) < 65535,
                 "Incorrect InterestLifetime (should not be smaller than 0 and larger than 65535");
  
  // rounding timestamp value to seconds
  start.WriteU16 (static_cast<uint16_t> (m_interest->GetInterestLifetime.ToInteger (Time::S)));

  uint32_t offset = Name (ConstCast<ns3::Name> (m_interest->GetNamePtr ())).Serialize (start);
  start.Next (offset);
  
  start.WriteU16 (0); // no selectors
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
  m_interest->SetNackType (i.ReadU8 ());

  m_interest->SetInterestLifetime (Seconds (i.ReadU16 ()));

  Name name;
  uint32_t offset = name.Deserialize (i);
  m_interest->SetName (name->GetName ());
  i.Next (offset);
  
  i.ReadU16 ();
  i.ReadU16 ();

  NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

  return i.GetDistanceFrom (start);
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
  : m_data (Create<ndn::ContentObject> ())
{
}

Data::Data (Ptr<ndn::ContentObject> data)
  : m_data (data)
{
}

Ptr<ndn::ContentObject>
GetData ()
{
  return m_data;
}

Ptr<Packet>
Data::ToWire (Ptr<const ndn::ContentObject> data)
{
  Ptr<const Packet> p = data->GetWire ();
  if (!p)
    {
      p = Create<Packet> (*data->GetPayload ());
      Data wireEncoding (data);
      p->AddHeader (wireEncoding);
      data->SetWire (p);
    }
  
  return p->Copy ();
}

Ptr<ndn::ContentObject>
Data::FromWire (Ptr<Packet> packet)
{
  Ptr<ndn::ContentObject> data = Create<ndn::ContentObject> ();
  // data->SetWire (packet->Copy ()); /* not sure if this is needed */

  Data wireEncoding (data);
  packet->RemoveHeader (wireEncoding);

  data->SetPayload (packet);

  return data;
}

uint32_t
Data::GetSerializedSize () const
{
  uint32_t size = 1 + 1 + 2 +
    ((2 + 2) + (Name (ConstCast<ns3::Name> (m_data->GetNamePtr ())).GetSerializedSize ()) + (2 + 2 + 4 + 2 + 2 + (2 + 0)));
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
  uint32_t offset = Name (ConstCast<ns3::Name> (m_data->GetNamePtr ())).Serialize (start);
  start.Next (offset);

  // content
  // for now assume that contentdata length is zero
  start.WriteU16 (2 + 4 + 2 + 2 + (2 + 0));
  start.WriteU16 (4 + 2 + 2 + (2 + 0));
  start.WriteU32 (static_cast<uint32_t> (m_data->GetTimestamp.ToInteger (Time::S)));
  start.WriteU16 (static_cast<uint16_t> (m_data->GetFreshness.ToInteger (Time::S)));
  start.WriteU16 (0); // reserved 
  start.WriteU16 (0); // Length (ContentInfoOptions)

  // that's it folks
}

uint32_t
Data::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  if (i.ReadU8 () != 0x80)
    throw new ContentObjectException ();

  if (i.ReadU8 () != 0x01)
    throw new ContentObjectException ();

  i.ReadU16 (); // length

  uint32_t signatureLength = i.ReadU16 ();
  if (signatureLength == 6)
    {
      if (i.ReadU16 () != 0xFF00) // signature type
        throw new ContentObjectException ();
      m_data.SetSignature (i.ReadU32 ());
    }
  else if (signatureLength == 2)
    {
      if (i.ReadU16 () != 0) // signature type
        throw new ContentObjectException ();
      m_data.SetSignature (0);
    }
  else
    throw new ContentObjectException ();

  Name name;
  uint32_t offset = name.Deserialize (i);
  m_data->SetName (name->GetName ());
  i.Next (offset);

  if (i.ReadU16 () != (2 + 4 + 2 + 2 + (2 + 0))) // content length
    throw new ContentObjectException ();

  if (i.ReadU16 () != (4 + 2 + 2 + (2 + 0))) // Length (content Info)
    throw new ContentObjectException ();

  m_data->SetTimestamp (Seconds (i.ReadU32 ()));
  m_data->SetFreshness (Seconds (i.ReadU16 ()));

  if (i.ReadU16 () != 0) // Reserved
    throw new ContentObjectException ();
  if (i.ReadU16 () != 0) // Length (ContentInfoOptions)
    throw new ContentObjectException ();

  NS_ASSERT_MSG (i.GetDistanceFrom (start) == GetSerializedSize (),
                 "Something wrong with ContentObject::Deserialize");
  
  return i.GetDistanceFrom (start);
}

}
}

NDN_NAMESPACE_END
