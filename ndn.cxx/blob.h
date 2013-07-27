/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *                     Zhenkai Zhu
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Zhenkai Zhu <zhenkai@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_BLOB_H
#define NDN_BLOB_H

#include "ns3/ndn-common.h"

#include <vector>

NDN_NAMESPACE_BEGIN

/**
 * @ingroup ndn-cxx
 * @brief Class representing a general-use binary blob
 */
class Blob
{
public:
  typedef std::vector<char> base;
  
  typedef base::value_type             value_type;
  typedef base::pointer                pointer;
  typedef base::const_pointer          const_pointer;
  typedef base::reference              reference;
  typedef base::const_reference        const_reference;
  typedef base::iterator               iterator;
  typedef base::const_iterator         const_iterator;
  typedef base::const_reverse_iterator const_reverse_iterator;
  typedef base::reverse_iterator       reverse_iterator;
  typedef base::size_type              size_type;
  typedef base::difference_type        difference_type;
  typedef base::allocator_type         allocator_type;
  
public:
  /**
   * @brief Creates an empty blob
   */
  Blob ()
  {
  }

  Blob (const std::string &data)
    : m_data (data.begin (), data.end ())
  {
  }

  Blob (const void *buf, size_t length)
    : m_data (reinterpret_cast<const char*> (buf), reinterpret_cast<const char*> (buf) + length)
  {
  }
  
  /**
   * @brief Get pointer to the first byte of the binary blob
   */
  inline char*
  buf ()
  {
    return &m_data.front ();
  }

  /**
   * @brief Get const pointer to the first byte of the binary blob
   */
  inline const char*
  buf () const
  {
    return &m_data.front ();
  }

  iterator begin () { return m_data.begin (); }
  const_iterator begin () const { return m_data.begin (); }
  iterator end () { return m_data.end (); }
  const_iterator end () const { return m_data.end (); }
  size_t size () const { return m_data.size (); }

  void swap (Blob &x) { m_data.swap (x.m_data); }
  void push_back (value_type val) { m_data.push_back (val); }

  bool empty () const { return m_data.empty (); }

  Blob &
  operator = (const Blob &other) { m_data = other.m_data; return *this; }

  reference operator [] (size_type pos) { return m_data [pos]; }
  const_reference operator [] (size_type pos) const { return m_data [pos]; }

  char getItem (size_type pos) const { return m_data [pos]; }

  void clear () { m_data.clear (); }

private:
  friend bool operator == (const Blob &a, const Blob &b);
  friend bool operator <  (const Blob &a, const Blob &b);
  friend bool operator <= (const Blob &a, const Blob &b);
  friend bool operator >  (const Blob &a, const Blob &b);
  friend bool operator >= (const Blob &a, const Blob &b);

private:
  std::vector< char > m_data;
};

inline bool operator == (const Blob &a, const Blob &b)  { return a.m_data == b.m_data; }
inline bool operator <  (const Blob &a, const Blob &b)  { return a.m_data <  b.m_data; }
inline bool operator <= (const Blob &a, const Blob &b)  { return a.m_data <= b.m_data; }
inline bool operator >  (const Blob &a, const Blob &b)  { return a.m_data >  b.m_data; }
inline bool operator >= (const Blob &a, const Blob &b)  { return a.m_data >= b.m_data; }

NDN_NAMESPACE_END

#include <boost/functional/hash.hpp>
namespace boost
{
inline std::size_t
hash_value (const ns3::ndn::Blob v)
{
  return boost::hash_range (v.begin(), v.end());
}
}

#endif // NDN_BLOB_H
