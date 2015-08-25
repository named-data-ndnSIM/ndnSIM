/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef MULTI_TYPE_CONTAINER_H_
#define MULTI_TYPE_CONTAINER_H_

/// @cond include_hidden

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/at.hpp>

namespace ns3 {
namespace ndn {
namespace ndnSIM {
namespace detail {

template<class T>
struct wrap {
  T value_;
};

template<class Vector>
struct multi_type_container
  : public boost::mpl::inherit_linearly<Vector, boost::mpl::inherit<wrap<boost::mpl::_2>,
                                                                    boost::mpl::_1>>::type {
  template<int N>
  struct index {
    typedef typename boost::mpl::at_c<Vector, N>::type type;
  };

  template<class T>
  T&
  get()
  {
    return static_cast<wrap<T>&>(*this).value_;
  }

  template<class T>
  const T&
  get() const
  {
    return static_cast<const wrap<T>&>(*this).value_;
  }

  template<int N>
  typename boost::mpl::at_c<Vector, N>::type&
  get()
  {
    typedef typename boost::mpl::at_c<Vector, N>::type T;
    return static_cast<wrap<T>&>(*this).value_;
  }

  template<int N>
  const typename boost::mpl::at_c<Vector, N>::type&
  get() const
  {
    typedef typename boost::mpl::at_c<Vector, N>::type T;
    return static_cast<const wrap<T>&>(*this).value_;
  }
};

} // detail
} // ndnSIM
} // ndn
} // ns3

/// @endcond include_hidden

#endif // MULTI_TYPE_CONTAINER_H_
