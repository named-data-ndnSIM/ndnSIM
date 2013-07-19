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

  for (Name::const_iterator item = name.begin ();
       item != name.end ();
       item++)
    {
      i.WriteU16 (static_cast<uint16_t> (item->size ()));
      i.Write (reinterpret_cast<const uint8_t*> (item->buf ()), item->size ());
    }

  return i.GetDistanceFrom (start);
}

size_t
NdnSim::SerializedSizeName (const Name &name)
{
  size_t nameSerializedSize = 2;

  for (Name::const_iterator i = name.begin ();
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

      name->append (tmp, length);
    }

  return name;
}


size_t
NdnSim::SerializeExclude (Buffer::Iterator &i, const Exclude &exclude)
{
  Buffer::Iterator start = i;

  i.WriteU16 (static_cast<uint16_t> (SerializedSizeExclude (exclude)-2));

  for (Exclude::const_reverse_iterator item = exclude.rbegin ();
       item != exclude.rend ();
       item++)
    {
      if (!item->first.empty ())
        {
          i.WriteU8 (ExcludeNameType);
          i.WriteU16 (static_cast<uint16_t> (item->first.size ()));
          i.Write (reinterpret_cast<const uint8_t*> (item->first.buf ()), item->first.size ());
        }
      if (item->second)
        {
          i.WriteU8 (ExcludeAnyType);
        }
    }
  return i.GetDistanceFrom (start);
}

size_t
NdnSim::SerializedSizeExclude (const Exclude &exclude)
{
  size_t excludeSerializedSize = 2;

  for (Exclude::const_reverse_iterator item = exclude.rbegin ();
       item != exclude.rend ();
       item++)
    {
      if (!item->first.empty ())
        {
          excludeSerializedSize += 1 + 2 + item->first.size ();
        }
      if (item->second)
        {
          excludeSerializedSize += 1;
        }
    }

  return excludeSerializedSize;
}

Ptr<Exclude>
NdnSim::DeserializeExclude (Buffer::Iterator &i)
{
  Ptr<Exclude> exclude = Create<Exclude> ();

  uint16_t excludeLength = i.ReadU16 ();
  while (excludeLength > 0)
    {
      uint8_t type = i.ReadU8 ();
      excludeLength --;
      
      if (type == ExcludeAnyType)
        {
          exclude->appendExclude (name::Component (), true);
        }
      else if (type == ExcludeNameType)
        {
          uint16_t length = i.ReadU16 ();
          excludeLength = excludeLength - 2 - length;

          uint8_t tmp[length];
          i.Read (tmp, length);

          bool any = false;
          if (excludeLength > 0)
            {
              uint8_t type = i.ReadU8 ();
              if (type != ExcludeAnyType)
                {
                  i.Prev ();
                }
              else
                {
                  any = true;
                  excludeLength --;
                }
            }

          exclude->appendExclude (name::Component (tmp, length), any);
        }
      else
        {
          NS_FATAL_ERROR ("Incorrect format of Exclude filter");
        }
    }
  return exclude;
}


} // wire

NDN_NAMESPACE_END
