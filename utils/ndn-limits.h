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

#ifndef _NDN_LIMITS_H_
#define	_NDN_LIMITS_H_

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/traced-value.h"

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn
 * \brief Structure to manage limits for outstanding interests
 */
class Limits :
    public Object
{
public:
  static TypeId
  GetTypeId ();
  
  /**
   * \brief Constructor
   * \param prefix smart pointer to the prefix for the FIB entry
   */
  Limits ()
  : m_maxLimit (0)
  , m_curMaxLimit (0)
  , m_outstanding (0)
  { }
 
  /**
   * @brief Set limit for the number of outstanding interests
   */
  void
  SetMaxLimit (uint32_t max);

  /**
   * @brief Get limit for the number of outstanding interests
   */
  uint32_t
  GetMaxLimit () const;

  /**
   * @brief Check whether limits are enabled or not
   */
  inline bool
  IsEnabled () const;

  /**
   * @brief Decay current limit (exponential decaying)
   *
   * If needed, this method should be called externally periodically (doesn't matter how often, decaying amount will remain the same).
   * Decaying is most likely needed for per-prefix limits, but definitely not needed for per-face limits.
   */
  void
  DecayCurrentLimit ();

  /**
   * @brief Increase current limit (additive increase)
   */
  void
  IncreaseLimit ();

  /**
   * @brief Decrease current limit (multiplicative decrease)
   */
  void
  DecreaseLimit ();

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  
  /**
   * @brief Check if new interest can be send out, if yes, number of outstanding will be increased
   */
  bool
  IsBelowLimit ();

  /**
   * @brief Remove outstanding interests
   */
  void
  RemoveOutstanding ();
  
public:
  uint32_t m_maxLimit;
  
  TracedValue< double >   m_curMaxLimit;
  TracedValue< uint32_t > m_outstanding;

  Time     m_exponentialDecayTau;
  Time     m_nonDecreasePeriod;
  double   m_additiveIncrease;
  double   m_multiplicativeDecrease;

  Time     m_lastDecrease;
  Time     m_lastDecay;
};

inline bool
Limits::IsEnabled () const
{
  return m_maxLimit != 0;
}
  

} // namespace ndn
} // namespace ns3

#endif // _NDN_LIMITS_H_
