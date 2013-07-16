/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * GNU 3.0 license, See the LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "wire-ndnsim.h"
#include <boost/foreach.hpp>

NDN_NAMESPACE_BEGIN

namespace wire {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

size_t
NdnSim::SerializeName (Buffer::Iterator &i, const Name &name)
{
  Buffer::Iterator start = i;

  i.WriteU16 (static_cast<uint16_t> (SerializedSizeName (name)-2));

  for (std::list<std::string>::const_iterator item = name.begin ();
       item != name.end ();
       item++)
    {
      i.WriteU16 (static_cast<uint16_t> (item->size ()));
      i.Write (reinterpret_cast<const uint8_t*> (item->c_str ()), item->size ());
    }

  return i.GetDistanceFrom (start);
}

size_t
NdnSim::SerializedSizeName (const Name &name)
{
  size_t nameSerializedSize = 2;

  for (std::list<std::string>::const_iterator i = name.begin ();
       i != name.end ();
       i++)
    {
      nameSerializedSize += 2 + i->size ();
    }
  NS_ASSERT_MSG (nameSerializedSize < 30000, "Name is too long (> 30kbytes)");

  return nameSerializedSize;
}

Ptr<Name>
NdnSim::DeserializeName (Buffer::Iterator &i)
{
  Ptr<Name> name = Create<Name> ();

  uint16_t nameLength = i.ReadU16 ();
  while (nameLength > 0)
    {
      uint16_t length = i.ReadU16 ();
      nameLength = nameLength - 2 - length;

      uint8_t tmp[length];
      i.Read (tmp, length);

      name->Add (std::string (reinterpret_cast<const char*> (tmp), length));
    }

  return name;
}

} // wire

NDN_NAMESPACE_END
