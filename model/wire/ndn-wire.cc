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
#include "ns3/ndn-content-object.h"
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
Wire::FromData (Ptr<const ContentObject> data, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
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

Ptr<ContentObject>
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


uint32_t
Wire::FromNameSize (Ptr<const Name> name, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
  if (wireFormat == WIRE_FORMAT_DEFAULT)
    wireFormat = GetWireFormat ();

  if (wireFormat == WIRE_FORMAT_NDNSIM)
    return wire::NdnSim::SerializedSizeName (*name);
  else if (wireFormat == WIRE_FORMAT_CCNB)
    return wire::Ccnb::SerializedSizeName (*name);
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
      return 0;
    }
}
  
void
Wire::FromName (Buffer::Iterator start, Ptr<const Name> name, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
  if (wireFormat == WIRE_FORMAT_DEFAULT)
    wireFormat = GetWireFormat ();

  if (wireFormat == WIRE_FORMAT_NDNSIM)
    wire::NdnSim::SerializedSizeName (*name);
  else if (wireFormat == WIRE_FORMAT_CCNB)
    wire::Ccnb::SerializedSizeName (*name);
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
    }
}

Ptr<Name>
Wire::ToName (Buffer::Iterator start, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
  if (wireFormat == WIRE_FORMAT_DEFAULT)
    wireFormat = GetWireFormat ();

  if (wireFormat == WIRE_FORMAT_NDNSIM)
    return wire::NdnSim::DeserializeName (start);
  else if (wireFormat == WIRE_FORMAT_CCNB)
    {
      return wire::Ccnb::DeserializeName (start);
      return 0;
    }
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
    }
}

NDN_NAMESPACE_END
