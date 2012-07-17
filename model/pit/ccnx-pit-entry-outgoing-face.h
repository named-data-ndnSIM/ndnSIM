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

#ifndef _CCNX_PIT_ENTRY_OUTGOING_FACE_H_
#define	_CCNX_PIT_ENTRY_OUTGOING_FACE_H_

#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "ns3/ccnx-face.h"

namespace ns3 {

/**
 * \ingroup ccnx
 * \brief PIT state component for each outgoing interest
 */
struct CcnxPitEntryOutgoingFace
{
  Ptr<CcnxFace> m_face;     ///< \brief face of the outgoing Interest
  Time m_sendTime;          ///< \brief time when the first outgoing interest is sent (for RTT measurements)
                            ///< \todo handle problem of retransmitted interests... Probably, we should include something similar
                            ///<       to TimeStamp TCP option for retransmitted (i.e., only lost interests will suffer)
  uint32_t m_retxCount;     ///< \brief number of retransmission
  bool m_waitingInVain;     ///< \brief when flag is set, we do not expect data for this interest, only a small hope that it will happen
	
public:
  /**
   * @brief Constructor to create CcnxPitEntryOutgoingFace
   * \param face face of the outgoing interest
   */
  CcnxPitEntryOutgoingFace (Ptr<CcnxFace> face);

  /**
   * @brief Update outgoing entry upon retransmission
   */
  void
  UpdateOnRetransmit ();

  /**
   * @brief Compare to CcnxPitEntryOutgoingFace
   */
  bool operator== (const CcnxPitEntryOutgoingFace &dst) { return *m_face==*dst.m_face; }

  /**
   * @brief Compare CcnxPitEntryOutgoingFace with CcnxFace
   */
  bool operator== (Ptr<CcnxFace> face) { return *m_face==*face; }

  /**
   * \brief Comparison operator used by boost::multi_index::identity<>
   */
  bool
  operator< (const CcnxPitEntryOutgoingFace &m) const { return *m_face < *(m.m_face); } // return identity of the face
};


} // namespace ns3

#endif	/* CCNX_PIT_ENTRY_OUTGOING_FACE_H */
