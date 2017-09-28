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

#include "batches.hpp"

namespace ns3 {

ATTRIBUTE_HELPER_CPP(Batches);

std::ostream&
operator<<(std::ostream& os, const Batches& batch)
{
  for (Batches::const_iterator i = batch.begin(); i != batch.end(); i++)
    os << std::get<0>(*i) << " " << std::get<1>(*i) << " ";

  return os;
}

/**
 * \brief Read components from input and add them to components. Will read input stream till eof
 * Substrings separated by slashes will become separate components
 */
std::istream&
operator>>(std::istream& is, Batches& batch)
{
  Time time;
  uint32_t amount;
  while (!is.eof()) {
    is >> time >> amount;
    batch.Add(time, amount);
    // std::cout << time << ", " << amount << ". \n";
  }

  return is;
}
}
