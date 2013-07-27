/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_LOCAL_INFO_TAG_H
#define NDN_LOCAL_INFO_TAG_H

#include "ns3/tag.h"
#include "ns3/ptr.h"

namespace ns3 {
namespace ndn {

class Face;

/**
 * @ingroup ndn-fw
 * @brief Packet tag that is used to keep information about face from which packet was received
 *
 * This tag may be extended later to include more information, if necessary
 */
class LocalInfoTag : public Tag
{
public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Constructor
   */
  LocalInfoTag (Ptr<Face> inFace);

  /**
   * @brief Destructor
   */
  ~LocalInfoTag ();

  /**
   * @brief Get smart pointer to the face
   *
   * @returns 0 or a valid smart pointer to a face
   */
  uint32_t
  Get () const
  {
    return m_faceId;
  }

  ////////////////////////////////////////////////////////
  // from ObjectBase
  ////////////////////////////////////////////////////////
  virtual TypeId
  GetInstanceTypeId () const;
  
  ////////////////////////////////////////////////////////
  // from Tag
  ////////////////////////////////////////////////////////
  
  virtual uint32_t
  GetSerializedSize () const;

  virtual void
  Serialize (TagBuffer i) const;
  
  virtual void
  Deserialize (TagBuffer i);

  virtual void
  Print (std::ostream &os) const;

public:
  static const uint32_t CACHE_FACE = static_cast<uint32_t> (-1);
  
private:
  uint32_t m_faceId;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_LOCAL_INFO_TAG_H
