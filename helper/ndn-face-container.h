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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_FACE_CONTAINER_H
#define NDN_FACE_CONTAINER_H

#include <stdint.h>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/ndn-face.h"

namespace ns3 {

/**
 * \ingroup ndn-helpers
 * \brief A pool for Ndn faces
 * 
 * Provides tools to perform basic manipulation on faces, such as
 * setting metrics and states on faces
 *
 * \see NdnStackHelper
 */
class NdnFaceContainer : public SimpleRefCount<NdnFaceContainer>
{
private:
  typedef std::vector<Ptr<NdnFace> > FaceContainer;
public:
  typedef FaceContainer::const_iterator Iterator; ///< \brief Iterator over NdnFaceContainer

  /**
   * \brief Create an empty NdnFaceContainer.
   */
  NdnFaceContainer ();

  /**
   * \brief Copy constructor for NdnFaceContainer. Calls AddAll method
   *
   * \see NdnFaceContainer::AddAll
   */
  NdnFaceContainer (const NdnFaceContainer &other);

  /**
   * \brief Copy operator for NdnFaceContainer. Empties vector and calls AddAll method
   *
   * All previously obtained iterators (Begin() and End()) will be invalidated
   *
   * \see NdnFaceContainer::AddAll
   */
  NdnFaceContainer& operator= (const NdnFaceContainer &other);
  
  /**
   * \brief Add all entries from other container
   *
   * \param other smart pointer to a container
   */
  void AddAll (Ptr<NdnFaceContainer> other);

  /**
   * \brief Add all entries from other container
   *
   * \param other container
   */
  void AddAll (const NdnFaceContainer &other);

  /**
   * \brief Get an iterator which refers to the first pair in the
   * container.
   *
   * \returns an iterator which refers to the first pair in the container.
   */
  Iterator Begin () const;

  /**
   * \brief Get an iterator which indicates past-the-last Node in the 
   * container.
   *
   * \returns an iterator which indicates an ending condition for a loop.
   */
  Iterator End () const;

  /**
   * \brief Get the number of faces stored in this container
   *
   * \returns the number of faces stored in this container
   */
  uint32_t GetN () const;

  // /**
  //  * \brief Set a metric for all faces in the container
  //  *
  //  * \param metric value of metric to assign to all faces in the container
  //  */
  // void SetMetricToAll (uint16_t metric);

  /**
   * Add an entry to the container
   *
   * \param face a smart pointer to a NdnFace-derived object
   *
   * @see NdnFace
   */
  void Add (const Ptr<NdnFace> &face);

  /**
   * Get a smart pointer to NdnFace-derived object stored in the container
   *
   * \param i the iterator corresponding to the requested object
   *
   * This method is redundant and simple dereferencing of the iterator should be used instead
   *
   * @see NdnFace
   */
  Ptr<NdnFace> Get (Iterator i) const;

private:
  FaceContainer m_faces;
};

} // namespace ns3

#endif /* NDN_FACE_CONTAINER_H */
