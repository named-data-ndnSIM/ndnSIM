/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef _NDN_NAMEBUILDER_H_
#define _NDN_NAMEBUILDER_H_

#include <string>

class ccn_charbuf;

namespace ns3 {
namespace NDNabstraction {
  
class NameBuilder
{
public:
  NameBuilder ();
  NameBuilder (const std::string &s);
  ~NameBuilder ();
  
  const ccn_charbuf* GetName () const;
  NameBuilder& operator () (const std::string &s);
private:
  ccn_charbuf *m_value;
};

}
}
#endif // _NDN_NAMEBUILDER_H_

