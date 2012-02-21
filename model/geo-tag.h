/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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
 * Authors: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCNX_GEO_TAG_H
#define CCNX_GEO_TAG_H

#include "ns3/tag.h"
#include "ns3/vector.h"

namespace ns3
{

/**
 * \ingroup Ccnx
 * \brief Tag to optimize packet processing in NS-3 (to avoid unnecessary deserialization)
 */
class GeoTag : public Tag
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief Constructor
   */
  GeoTag (const Vector &position);
  
  /**
   * \brief Set name for the tag
   */
  void
  SetPosition (const Vector &position);
  
  /**
   * \brief Get name of the tag
   */
  const Vector &
  GetPosition () const;

  // from Tag
  virtual uint32_t
  GetSerializedSize() const;
  
  virtual void
  Serialize(TagBuffer) const;
  
  virtual void
  Deserialize(ns3::TagBuffer);
  
  virtual void
  Print(std::ostream&) const;
  
private:
  Vector m_position;
};

class GeoTransmissionTag : public GeoTag
{
public:
  static TypeId
  GetTypeId ();

  GeoTransmissionTag (const Vector &position) : GeoTag(position) {}
};

class GeoSrcTag : public GeoTag
{
public:
  static TypeId
  GetTypeId ();

  GeoSrcTag (const Vector &position) : GeoTag(position) {}
};

} // namespace ns3

#endif // CCNX_GEO_TAG_H
