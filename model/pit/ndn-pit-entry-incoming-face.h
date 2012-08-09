/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef _NDN_PIT_ENTRY_INCOMING_FACE_H_
#define	_NDN_PIT_ENTRY_INCOMING_FACE_H_

#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "ns3/ndn-face.h"
// #include <iostream>

namespace ns3 {

/**
 * \ingroup ndn
 * \brief PIT state component for each incoming interest (not including duplicates)
 */
struct NdnPitEntryIncomingFace
{
  Ptr<NdnFace> m_face; ///< \brief face of the incoming Interest
  Time m_arrivalTime;   ///< \brief arrival time of the incoming Interest

public:
  /**
   * \brief Constructor
   * \param face face of the incoming interest
   * \param lifetime lifetime of the incoming interest
   */
  NdnPitEntryIncomingFace (Ptr<NdnFace> face);

  /**
   * @brief Default constructor, necessary for Python bindings, but should not be used anywhere else.
   */
  NdnPitEntryIncomingFace ();
  /**
   * @brie Copy operator
   */
  NdnPitEntryIncomingFace &
  operator = (NdnPitEntryIncomingFace &other);

  /**
   * @brief Compare two NdnPitEntryIncomingFace
   */
  bool operator== (const NdnPitEntryIncomingFace &dst) { return *m_face==*(dst.m_face); }

  /**
   * @brief Compare NdnPitEntryIncomingFace with NdnFace
   */
  bool operator== (Ptr<NdnFace> face) { return *m_face==*face; }

  /**
   * \brief Comparison operator used by boost::multi_index::identity<>
   */
  bool
  operator< (const NdnPitEntryIncomingFace &m) const { return *m_face < *(m.m_face); } // return identity of the face
};


} // namespace ns3

#endif	/* NDN_PIT_ENTRY_INCOMING_FACE_H */
