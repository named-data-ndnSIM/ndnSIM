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

#ifndef NDN_FACE_CONTAINER_H
#define NDN_FACE_CONTAINER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include <stdint.h>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/ndnSIM/model/ndn-face.hpp"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-helpers
 * @brief A pool for Ndn faces
 *
 * Provides tools to perform basic manipulation on faces, such as setting metrics and
 * states on faces
 *
 * @see ndn::StackHelper
 */
class FaceContainer : public SimpleRefCount<FaceContainer> {
private:
  typedef std::vector<shared_ptr<Face>> Container;

public:
  typedef Container::const_iterator Iterator; ///< \brief Iterator over FaceContainer

  /**
   * \brief Create an empty FaceContainer.
   */
  FaceContainer();

  /**
   * \brief Copy constructor for FaceContainer. Calls AddAll method
   *
   * \see FaceContainer::AddAll
   */
  FaceContainer(const FaceContainer& other);

  /**
   * \brief Copy operator for FaceContainer. Empties vector and calls AddAll method
   *
   * All previously obtained iterators (Begin() and End()) will be invalidated
   *
   * \see FaceContainer::AddAll
   */
  FaceContainer&
  operator=(const FaceContainer& other);

  /**
   * \brief Add all entries from other container
   *
   * \param other smart pointer to a container
   */
  void
  AddAll(Ptr<FaceContainer> other);

  /**
   * \brief Add all entries from other container
   *
   * \param other container
   */
  void
  AddAll(const FaceContainer& other);

  /**
   * \brief Get an iterator which refers to the first pair in the
   * container.
   *
   * \returns an iterator which refers to the first pair in the container.
   */
  Iterator
  Begin() const;

  /**
   * \brief Get an iterator which indicates past-the-last Node in the
   * container.
   *
   * \returns an iterator which indicates an ending condition for a loop.
   */
  Iterator
  End() const;

  /**
   * \brief Get the number of faces stored in this container
   *
   * \returns the number of faces stored in this container
   */
  uint32_t
  GetN() const;

  // /**
  //  * \brief Set a metric for all faces in the container
  //  *
  //  * \param metric value of metric to assign to all faces in the container
  //  */
  // void SetMetricToAll (uint16_t metric);

  /**
   * Add an entry to the container
   *
   * \param face a smart pointer to a Face-derived object
   *
   * @see Face
   */
  void
  Add(const shared_ptr<Face>& face);

  /**
   * Get a smart pointer to Face-derived object stored in the container
   *
   * \param i the iterator corresponding to the requested object
   *
   * This method is redundant and simple dereferencing of the iterator should be used instead
   *
   * @see Face
   */
  shared_ptr<Face>
  Get(Iterator i) const;

private:
  Container m_faces;
};

} // namespace ndn
} // namespace ns3

#endif /* NDN_FACE_CONTAINER_H */
