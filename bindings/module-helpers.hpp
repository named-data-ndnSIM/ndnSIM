/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2016  Regents of the University of California.
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

#ifndef NDNSIM_BINDINGS_MODULE_HELPERS_HPP
#define NDNSIM_BINDINGS_MODULE_HELPERS_HPP

#include <ns3/ndnSIM/NFD/daemon/table/fib.hpp>

namespace nfd {
namespace fib {

std::shared_ptr<Face>
getFaceFromFibNextHop(const NextHop& nextHop);

} // namespace fib
} // namespace nfd

#endif // NDNSIM_BINDINGS_MODULE_HELPERS_HPP
