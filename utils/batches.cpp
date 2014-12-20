/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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

#include "batches.h"

namespace ns3 {

ATTRIBUTE_HELPER_CPP (Batches);

std::ostream &
operator << (std::ostream &os, const Batches &batch)
{
  for (Batches::const_iterator i = batch.begin (); i != batch.end (); i++)
    os << i->get<0> () << " " << i->get<1> () << " ";

  return os;
}

/**
 * \brief Read components from input and add them to components. Will read input stream till eof
 * Substrings separated by slashes will become separate components
 */
std::istream &
operator >> (std::istream &is, Batches &batch)
{
  Time time;
  uint32_t amount;
  while (!is.eof ())
    {
      is >> time >> amount;
      batch.Add (time, amount);
      // std::cout << time << ", " << amount << ". \n";
    }
  
  is.clear ();
  return is;
}


}
