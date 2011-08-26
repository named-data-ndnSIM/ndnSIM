/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 */

#ifndef _NDN_NAME_COMPONENTS_H_
#define _NDN_NAME_COMPONENTS_H_

#include "ns3/simple-ref-count.h"
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"

#include <string>
#include <algorithm>
#include <list>
#include "ns3/object.h"

#include <boost/ref.hpp>

namespace ns3 {

class CcnxNameComponents : public SimpleRefCount<CcnxNameComponents>
{
public:
  /**
   * \brief Creates a prefix with zero components (can be looked as root "/")
   */
  CcnxNameComponents ();
  CcnxNameComponents (const std::string &s);
  CcnxNameComponents (const std::list<boost::reference_wrapper<const std::string> > &components);
  
  inline void
  Add (const std::string &s);
       
  CcnxNameComponents&
  operator () (const std::string &s);

  const std::list<std::string> &
  GetComponents () const;

  /**
   * \brief Get subcomponents of the name, starting with first component
   * \param num Number of components to return. Valid value is in range [1, GetComponents ().size ()]
   */
  std::list<boost::reference_wrapper<const std::string> >
  GetSubComponents (size_t num) const;
  
  // virtual uint32_t
  // GetSerializedSize (void) const;

  // virtual void
  // Serialize (Buffer::Iterator start) const;

  // virtual uint32_t
  // Deserialize (Buffer::Iterator start);

  void Print (std::ostream &os) const;

  inline size_t
  size () const;

  inline bool
  operator== (const CcnxNameComponents &prefix) const;

  inline bool
  operator< (const CcnxNameComponents &prefix) const;
  
private:
  std::list<std::string> m_prefix;

  typedef std::list<std::string>::iterator iterator;
  typedef std::list<std::string>::const_iterator const_iterator;
};

/**
 * \brief Print out name components separated by slashes, e.g., /first/second/third
 */
std::ostream &
operator << (std::ostream &os, const CcnxNameComponents &components);

/**
 * \brief Read components from input and add them to components. Will read input stream till eof
 *
 * \todo Check that NS-3 doesn't give unlimited input streams... Otherwise it would be disaster
 *
 * Substrings separated by slashes will become separate components
 */
std::istream &
operator >> (std::istream &is, CcnxNameComponents &components);
  
size_t
CcnxNameComponents::size () const
{
  return m_prefix.size ();
}
  
void
CcnxNameComponents::Add (const std::string &s)
{
  (*this) (s);
}

bool
CcnxNameComponents::operator== (const CcnxNameComponents &prefix) const
{
  if (m_prefix.size () != prefix.m_prefix.size ())
    return false;
  
  return std::equal (m_prefix.begin (), m_prefix.end (), prefix.m_prefix.begin ());
}

bool
CcnxNameComponents::operator< (const CcnxNameComponents &prefix) const
{
  return std::lexicographical_compare (m_prefix.begin (), m_prefix.end (),
                                       prefix.m_prefix.begin (), prefix.m_prefix.end ());
}

    
/**
* \class ns3::ComponentsValue
* \brief hold objects of type ns3:CcnxNameComponents
*/
ATTRIBUTE_HELPER_HEADER (CcnxNameComponents);
} // namespace ns3

#endif // _NDN_NAME_COMPONENTS_H_

