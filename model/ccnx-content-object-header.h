/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef _CCNX_CONTENT_OBJECT_HEADER_H_
#define _CCNX_CONTENT_OBJECT_HEADER_H_

#include "ns3/integer.h"
#include "ns3/header.h"

#include <string>
#include <vector>
#include <list>

#include "name-components.h"

namespace ns3
{

/**
 * CCNx XML definition of ContentObject
 * 
 * Only few important fields are actually implemented in the simulation
 *
 *
 * NDN ContentObjectHeader and routines to serialize/deserialize
 *
 *  Simplifications:
 */
  
class CcnxContentObjectHeader : public Header
{
public:
  /**
   * Constructor
   *
   * Creates a null header
   **/
  CcnxContentObjectHeader ();

  /**
   * \brief Set interest name
   *
   * Sets name of the interest. For example, SetName( Name::Components("prefix")("postfix") );
   **/
  void
  SetName (const Ptr<Name::Components> &name);

  const Name::Components&
  GetName () const;

  // void
  // SetSignature ();

  // ?
  // GetSignature () const;

  // void
  // SetSignedInfo ();

  // ?
  // GetSignedInfo () const;

  // void
  // SetContent ();

  // ?
  // GetContent ();
  
  //////////////////////////////////////////////////////////////////
  
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  Ptr<Name::Components> m_name;
  // m_signature;
  // m_signedInfo;
  // ? content
};

} // namespace NDNabstraction
} // namespace ns3

#endif // _CCNX_CONTENT_OBJECT_HEADER_H_
