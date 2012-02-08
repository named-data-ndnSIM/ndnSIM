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

#ifndef CCNX_NAME_COMPONENTS_TAG_H
#define CCNX_NAME_COMPONENTS_TAG_H

#include "ns3/tag.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/ptr.h"

namespace ns3
{

/**
 * \ingroup Ccnx
 * \brief Tag to optimize packet processing in NS-3 (to avoid unnecessary deserialization)
 */
class CcnxNameComponentsTag : public Tag
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief Constructor
   */
  CcnxNameComponentsTag (const Ptr<const CcnxNameComponents> &name);
  
  /**
   * \brief Set name for the tag
   */
  void
  SetName (const Ptr<const CcnxNameComponents> &name);
  
  /**
   * \brief Get name of the tag
   */
  const Ptr<const CcnxNameComponents> &
  GetName () const;

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
  Ptr<const CcnxNameComponents> m_name;
};

  
} // namespace ns3

#endif // CCNX_NAME_COMPONENTS_TAG_H
