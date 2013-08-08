/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU 3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndn-wire.h"

#include "ns3/global-value.h"
#include "ns3/integer.h"
#include "ns3/ndn-common.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"
#include "ns3/ndn-header-helper.h"

#include "ndnsim.h"
#include "ndnsim/wire-ndnsim.h"
#include "ccnb.h"
#include "ccnb/wire-ccnb.h"

NDN_NAMESPACE_BEGIN

static
GlobalValue g_wireFormat ("ndn::WireFormat",
                          "Default wire format for ndnSIM.  ndnSIM will be accepting packets "
                          "in any supported packet formats, but if encoding requested, it will "
                          "use default wire format to encode (0 for ndnSIM (default), 1 for CCNb)",
                          IntegerValue (Wire::WIRE_FORMAT_NDNSIM),
                          MakeIntegerChecker<int32_t> ());

static inline uint32_t
GetWireFormat ()
{
  static int32_t format = -1;
  if (format >= 0)
    return format;

  IntegerValue value;
  g_wireFormat.GetValue (value);
  format = value.Get ();

  return format;
}

Ptr<Packet>
Wire::FromInterest (Ptr<const Interest> interest, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
  if (wireFormat == WIRE_FORMAT_DEFAULT)
    wireFormat = GetWireFormat ();

  if (wireFormat == WIRE_FORMAT_NDNSIM)
    return wire::ndnSIM::Interest::ToWire (interest);
  else if (wireFormat == WIRE_FORMAT_CCNB)
    return wire::ccnb::Interest::ToWire (interest);
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
      return 0;
    }
}

Ptr<Interest>
Wire::ToInterest (Ptr<Packet> packet, int8_t wireFormat/* = WIRE_FORMAT_AUTODETECT*/)
{
  if (wireFormat == WIRE_FORMAT_AUTODETECT)
    {
      try
        {
          HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (packet);
          switch (type)
            {
            case HeaderHelper::INTEREST_NDNSIM:
              {
                return wire::ndnSIM::Interest::FromWire (packet);
              }
            case HeaderHelper::INTEREST_CCNB:
              {
                return wire::ccnb::Interest::FromWire (packet);
              }
            case HeaderHelper::CONTENT_OBJECT_NDNSIM:
            case HeaderHelper::CONTENT_OBJECT_CCNB:
              NS_FATAL_ERROR ("Data packet supplied for InterestFromWire function");
              break;
            default:
              NS_FATAL_ERROR ("Unsupported format");
              return 0;
            }

          // exception will be thrown if packet is not recognized
        }
      catch (UnknownHeaderException)
        {
          NS_FATAL_ERROR ("Unknown NDN header");
          return 0;
        }
    }
  else
    {
      if (wireFormat == WIRE_FORMAT_NDNSIM)
        return wire::ndnSIM::Interest::FromWire (packet);
      else if (wireFormat == WIRE_FORMAT_CCNB)
        return wire::ccnb::Interest::FromWire (packet);
      else
        {
          NS_FATAL_ERROR ("Unsupported format requested");
          return 0;
        }
    }
}

Ptr<Packet>
Wire::FromData (Ptr<const Data> data, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
  if (wireFormat == WIRE_FORMAT_DEFAULT)
    wireFormat = GetWireFormat ();

  if (wireFormat == WIRE_FORMAT_NDNSIM)
    return wire::ndnSIM::Data::ToWire (data);
  else if (wireFormat == WIRE_FORMAT_CCNB)
    return wire::ccnb::Data::ToWire (data);
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
      return 0;
    }
}

Ptr<Data>
Wire::ToData (Ptr<Packet> packet, int8_t wireFormat/* = WIRE_FORMAT_AUTODETECT*/)
{
  if (wireFormat == WIRE_FORMAT_AUTODETECT)
    {
      try
        {
          HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (packet);
          switch (type)
            {
            case HeaderHelper::CONTENT_OBJECT_NDNSIM:
              {
                return wire::ndnSIM::Data::FromWire (packet);
              }
            case HeaderHelper::CONTENT_OBJECT_CCNB:
              {
                return wire::ccnb::Data::FromWire (packet);
              }
            case HeaderHelper::INTEREST_NDNSIM:
            case HeaderHelper::INTEREST_CCNB:
              NS_FATAL_ERROR ("Interest supplied for DataFromWire function");
              break;
            default:
              NS_FATAL_ERROR ("Unsupported format");
              return 0;
            }

          // exception will be thrown if packet is not recognized
        }
      catch (UnknownHeaderException)
        {
          NS_FATAL_ERROR ("Unknown NDN header");
          return 0;
        }
    }
  else
    {
      if (wireFormat == WIRE_FORMAT_NDNSIM)
        return wire::ndnSIM::Data::FromWire (packet);
      else if (wireFormat == WIRE_FORMAT_CCNB)
        return wire::ccnb::Data::FromWire (packet);
      else
        {
          NS_FATAL_ERROR ("Unsupported format requested");
          return 0;
        }
    }
}


std::string
Wire::FromInterestStr (Ptr<const Interest> interest, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
  Ptr<Packet> pkt = FromInterest (interest, wireFormat);
  std::string wire;
  wire.resize (pkt->GetSize ());
  pkt->CopyData (reinterpret_cast<uint8_t*> (&wire[0]), wire.size ());

  return wire;
}

Ptr<Interest>
Wire::ToInterestStr (const std::string &wire, int8_t type/* = WIRE_FORMAT_AUTODETECT*/)
{
  Ptr<Packet> pkt = Create<Packet> (reinterpret_cast<const uint8_t*> (&wire[0]), wire.size ());
  return ToInterest (pkt, type);
}

std::string
Wire::FromDataStr (Ptr<const Data> data, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
  Ptr<Packet> pkt = FromData (data, wireFormat);
  std::string wire;
  wire.resize (pkt->GetSize ());
  pkt->CopyData (reinterpret_cast<uint8_t*> (&wire[0]), wire.size ());

  return wire;
}

Ptr<Data>
Wire::ToDataStr (const std::string &wire, int8_t type/* = WIRE_FORMAT_AUTODETECT*/)
{
  Ptr<Packet> pkt = Create<Packet> (reinterpret_cast<const uint8_t*> (&wire[0]), wire.size ());
  return ToData (pkt, type);
}

// uint32_t
// Wire::FromNameSize (Ptr<const Name> name, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
// {
//   if (wireFormat == WIRE_FORMAT_DEFAULT)
//     wireFormat = GetWireFormat ();

//   if (wireFormat == WIRE_FORMAT_NDNSIM)
//     {
//       std::cout << wire::NdnSim::SerializedSizeName (*name) << std::endl;
//       return wire::NdnSim::SerializedSizeName (*name);
//     }
//   else if (wireFormat == WIRE_FORMAT_CCNB)
//     return wire::Ccnb::SerializedSizeName (*name);
//   else
//     {
//       NS_FATAL_ERROR ("Unsupported format requested");
//       return 0;
//     }
// }

std::string
Wire::FromName (Ptr<const Name> name, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{

  if (wireFormat == WIRE_FORMAT_DEFAULT)
    wireFormat = GetWireFormat ();

  if (wireFormat == WIRE_FORMAT_NDNSIM)
    {
      Buffer buf;
      buf.AddAtStart (wire::NdnSim::SerializedSizeName (*name));
      Buffer::Iterator i = buf.Begin ();

      wire::NdnSim::SerializeName (i, *name);

      std::string wire;
      wire.resize (buf.GetSize ());
      buf.CopyData (reinterpret_cast<uint8_t *>(&wire[0]), wire.size ());

      return wire;
    }
  else if (wireFormat == WIRE_FORMAT_CCNB)
    {
      Buffer buf;
      buf.AddAtStart (wire::NdnSim::SerializedSizeName (*name));
      Buffer::Iterator i = buf.Begin ();

      wire::Ccnb::SerializeName (i, *name);

      std::string wire;
      wire.resize (buf.GetSize ());
      buf.CopyData (reinterpret_cast<uint8_t *>(&wire[0]), wire.size ());

      return wire;
    }
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
    }
}

Ptr<Name>
Wire::ToName (const std::string &name, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
  Buffer buf;
  buf.AddAtStart (name.size ());
  Buffer::Iterator i = buf.Begin ();
  i.Write (reinterpret_cast<const uint8_t *> (&name[0]), name.size ());

  i = buf.Begin ();

  if (wireFormat == WIRE_FORMAT_DEFAULT)
    wireFormat = GetWireFormat ();

  if (wireFormat == WIRE_FORMAT_NDNSIM)
    {
      return wire::NdnSim::DeserializeName (i);
    }
  else if (wireFormat == WIRE_FORMAT_CCNB)
    {
      return wire::Ccnb::DeserializeName (i);
    }
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
    }
}

NDN_NAMESPACE_END
