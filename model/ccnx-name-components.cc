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

#include "ccnx-name-components.h"

#include <iostream>

using namespace std;

namespace ns3 {

CcnxNameComponents::CcnxNameComponents ()
{
  // m_value = ccn_charbuf_create ();
  // ccn_name_init(m_value);
}

CcnxNameComponents::CcnxNameComponents (const string &s)
{
  // m_value = ccn_charbuf_create ();
  // ccn_name_init(m_value);
  // (*this) (s);
  m_prefix.push_back (s);
}

CcnxNameComponents::~CcnxNameComponents ()
{
  // ccn_charbuf_destroy(&m_value);
}

const std::list<std::string> &
CcnxNameComponents::GetComponents () const
{
  return m_prefix;
}

  
// const ccn_charbuf*
// Components::GetName () const
// {
//   return m_value;
// }

CcnxNameComponents&
CcnxNameComponents::operator () (const string &s)
{
  // ccn_name_append_str (m_value,s.c_str());
  m_prefix.push_back (s);
  return *this;
}

// Components::operator const unsigned char* ()
// {
//   return m_value->buf;
// }

void
CcnxNameComponents::Print (std::ostream &os) const
{
  for (const_iterator i=m_prefix.begin(); i!=m_prefix.end(); i++)
    {
      os << "/" << *i;
    }
}
  
std::ostream &
operator << (std::ostream &os, const CcnxNameComponents &components)
{
  components.Print (os);
  return os;
}
}

