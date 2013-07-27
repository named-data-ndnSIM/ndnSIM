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

#ifndef _NDN_LIMITS_WINDOW_H_
#define	_NDN_LIMITS_WINDOW_H_

#include "ndn-limits.h"

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn-fw
 * \brief Structure to manage limits for outstanding interests (window-based limiting)
 */
class LimitsWindow :
    public Limits
{
public:
  typedef Limits super;
  
  static TypeId
  GetTypeId ();
  
  /**
   * @brief Default Constructor
   */
  LimitsWindow ()
  : m_outstanding (0)
  { }

  /**
   * @brief Virtual destructor
   */
  virtual
  ~LimitsWindow () { }

  // from ndn::Limits

  virtual void
  SetLimits (double rate, double delay)
  {
    super::SetLimits (rate, delay);

    m_curMaxLimit = GetMaxRate () * GetMaxDelay ();
  }

  virtual
  double
  GetMaxLimit () const
  {
    return GetMaxRate () * GetMaxDelay ();
  }
  
  virtual void
  UpdateCurrentLimit (double limit);
  
  virtual double
  GetCurrentLimit () const
  {
    return m_curMaxLimit;
  }

  virtual double
  GetCurrentLimitRate () const
  {
    return m_curMaxLimit / GetMaxDelay ();
  }
  
  /**
   * @brief Check if current interest window (number of pending interests) if less than maximum 
   */
  virtual bool
  IsBelowLimit ();

  /**
   * @brief Increase current window of outstanding interests
   */
  virtual void
  BorrowLimit ();

  /**
   * @brief Decrease current window of outstanding interests
   */
  virtual void
  ReturnLimit ();
  
private:
  TracedValue< double > m_curMaxLimit;
  TracedValue< double > m_outstanding;
};
  

} // namespace ndn
} // namespace ns3

#endif // _NDN_LIMITS_WINDOW_H_
