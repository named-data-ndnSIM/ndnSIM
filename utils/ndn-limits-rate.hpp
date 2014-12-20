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

#ifndef _NDN_LIMITS_RATE_H_
#define	_NDN_LIMITS_RATE_H_

#include "ndn-limits.h"
#include <ns3/nstime.h>

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn-fw
 * \brief Structure to manage limits for outstanding interests
 */
class LimitsRate :
    public Limits
{
public:
  typedef Limits super;

  static TypeId
  GetTypeId ();

  /**
   * \brief Constructor
   * \param prefix smart pointer to the prefix for the FIB entry
   */
  LimitsRate ()
    : m_isLeakScheduled (false)
    , m_bucketMax (0)
    , m_bucketLeak (1)
    , m_bucket (0)
  { }

  virtual
  ~LimitsRate () { }

  virtual void
  SetLimits (double rate, double delay);

  virtual
  double
  GetMaxLimit () const
  {
    return GetMaxRate ();
  }

  /**
   * @brief Check if Interest limit is reached (token bucket is not empty)
   */
  virtual bool
  IsBelowLimit ();

  /**
   * @brief Get token from the bucket
   */
  virtual void
  BorrowLimit ();

  /**
   * @brief Does nothing (token bucket leakage is time-dependent only)
   */
  virtual void
  ReturnLimit ();

  /**
   * @brief Update normalized amount that should be leaked every second (token bucket leak rate) and leak rate
   */
  virtual void
  UpdateCurrentLimit (double limit);

  /**
   * @brief Get normalized amount that should be leaked every second (token bucket leak rate)
   */
  virtual double
  GetCurrentLimit () const
  {
    return m_bucketLeak;
  }

  virtual double
  GetCurrentLimitRate () const
  {
    return m_bucketLeak;
  }

protected:
  // from Node
  void
  NotifyNewAggregate ();

private:
  /**
   * @brief Leak bucket, assuming `interval' seconds between leakages
   *
   * @param interval Time interval for leakage. Used to calculate size of the leak
   */
  void
  LeakBucket (double interval);

private:
  bool m_isLeakScheduled;

  double m_bucketMax;   ///< \brief Maximum Interest allowance for this face (maximum tokens that can be issued at the same time)
  double m_bucketLeak;  ///< \brief Normalized amount that should be leaked every second (token bucket leak rate)
  double m_bucket;      ///< \brief Value representing current size of the Interest allowance for this face (current size of token bucket)

  Time m_leakRandomizationInteral;
};


} // namespace ndn
} // namespace ns3

#endif // _NDN_LIMITS_RATE_H_
