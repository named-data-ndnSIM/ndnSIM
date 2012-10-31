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
 * \ingroup ndn
 * \brief Structure to manage limits for outstanding interests
 */
class LimitsWindow :
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
  LimitsWindow ()
  : m_outstanding (0)
  { }

  virtual
  ~LimitsWindow () { }


  // from limits
  virtual bool
  IsBelowLimit ();

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // specific to window-based limits
  
  /**
   * @brief Remove outstanding interests
   */
  void
  RemoveOutstanding ();

  double
  GetOutstanding () const
  {
    return m_outstanding;
  }
  
protected:
  TracedValue< double > m_outstanding;
};
  

} // namespace ndn
} // namespace ns3

#endif // _NDN_LIMITS_WINDOW_H_
