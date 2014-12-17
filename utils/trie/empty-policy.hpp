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

#ifndef EMPTY_POLICY_H_
#define EMPTY_POLICY_H_

/// @cond include_hidden

namespace ns3 {
namespace ndn {
namespace ndnSIM {

/**
 * @brief Traits for empty (bogus) replacement policy
 */
struct empty_policy_traits {
  /// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
  static std::string
  GetName()
  {
    return "";
  }

  typedef void* policy_hook_type;

  template<class Container>
  struct container_hook {
    typedef void* type;
  };

  template<class Base, class Container, class Hook>
  struct policy {
    struct type {
      inline type(Base& base)
      {
      }

      inline void update(typename Container::iterator)
      {
      }
      inline bool insert(typename Container::iterator)
      {
        return true;
      }
      inline void
      lookup(typename Container::iterator item)
      {
      }
      inline void
      erase(typename Container::iterator item)
      {
      }
      inline void
      clear()
      {
      }
    };
  };
};

} // ndnSIM
} // ndn
} // ns3

/// @endcond

#endif // EMPTY_POLICY_H_
