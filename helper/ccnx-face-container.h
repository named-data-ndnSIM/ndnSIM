/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef CCNX_FACE_CONTAINER_H
#define CCNX_FACE_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "ns3/ccnx.h"

namespace ns3 {

/**
 * \brief holds a vector of std::pair of Ptr<Ccnx> and face index.
 *
 * Typically ns-3 CcnxFaces are installed on devices using an Ccnx address
 * helper.  The helper's Assign() method takes a NetDeviceContainer which holds 
 * some number of Ptr<NetDevice>.  For each of the NetDevices in the 
 * NetDeviceContainer the helper will find the associated Ptr<Node> and
 * Ptr<Ccnx>.  It makes sure that an face exists on the node for the 
 * device and then adds an CcnxAddress according to the address helper settings
 * (incrementing the CcnxAddress somehow as it goes).  The helper then converts
 * the Ptr<Ccnx> and the face index to a std::pair and adds them to a 
 * container -- a container of this type.
 *
 * The point is then to be able to implicitly associate an index into the 
 * original NetDeviceContainer (that identifies a particular net device) with
 * an identical index into the CcnxFaceContainer that has a std::pair with
 * the Ptr<Ccnx> and face index you need to play with the face.
 *
 * @see CcnxAddressHelper
 * @see Ccnx
 */
class CcnxFaceContainer
{
public:
  typedef std::vector<std::pair<Ptr<Ccnx>, uint32_t> >::const_iterator Iterator;

  /**
   * Create an empty CcnxFaceContainer.
   */
  CcnxFaceContainer ();

  /**
   * Concatenate the entries in the other container with ours.
   * \param other container
   */
  void Add (CcnxFaceContainer other);

  /**
   * \brief Get an iterator which refers to the first pair in the 
   * container.
   *
   * Pairs can be retrieved from the container in two ways.  First,
   * directly by an index into the container, and second, using an iterator.
   * This method is used in the iterator method and is typically used in a 
   * for-loop to run through the pairs
   *
   * \code
   *   ccnxFaceContainer::Iterator i;
   *   for (i = container.Begin (); i != container.End (); ++i)
   *     {
   *       std::pair<Ptr<Ccnx>, uint32_t> pair = *i;
   *       method (pair.first, pair.second);  // use the pair
   *     }
   * \endcode
   *
   * \returns an iterator which refers to the first pair in the container.
   */
  Iterator Begin (void) const;

  /**
   * \brief Get an iterator which indicates past-the-last Node in the 
   * container.
   *
   * Nodes can be retrieved from the container in two ways.  First,
   * directly by an index into the container, and second, using an iterator.
   * This method is used in the iterator method and is typically used in a 
   * for-loop to run through the Nodes
   *
   * \code
   *   NodeContainer::Iterator i;
   *   for (i = container.Begin (); i != container.End (); ++i)
   *     {
   *       std::pair<Ptr<Ccnx>, uint32_t> pair = *i;
   *       method (pair.first, pair.second);  // use the pair
   *     }
   * \endcode
   *
   * \returns an iterator which indicates an ending condition for a loop.
   */
  Iterator End (void) const;

  /**
   * \returns the number of Ptr<Ccnx> and face pairs stored in this 
   * ccnxFaceContainer.
   *
   * Pairs can be retrieved from the container in two ways.  First,
   * directly by an index into the container, and second, using an iterator.
   * This method is used in the direct method and is typically used to
   * define an ending condition in a for-loop that runs through the stored
   * Nodes
   *
   * \code
   *   uint32_t nNodes = container.GetN ();
   *   for (uint32_t i = 0 i < nNodes; ++i)
   *     {
   *       std::pair<Ptr<Ccnx>, uint32_t> pair = container.Get (i);
   *       method (pair.first, pair.second);  // use the pair
   *     }
   * \endcode
   *
   * \returns the number of Ptr<Node> stored in this container.
   */
  uint32_t GetN (void) const;

  /**
   * \param i index of ipfacePair in container
   * \param j face address index (if face has multiple addresses)
   * \returns the ccnx address of the j'th address of the face
   *  corresponding to index i.
   * 
   * If the second parameter is omitted, the zeroth indexed address of 
   * the face is returned.  Unless IP aliasing is being used on
   * the face, the second parameter may typically be omitted.
   */
  // ccnxAddress GetAddress (uint32_t i, uint32_t j = 0) const;

  void SetMetric (uint32_t i, uint16_t metric);

  /**
   * Manually add an entry to the container consisting of the individual parts
   * of an entry std::pair.
   *
   * \param ccnx pointer to ccnx object
   * \param face face index of the ccnxface to add to the container
   *
   * @see ccnxfaceContainer
   */
  void Add (Ptr<Ccnx> ccnx, uint32_t face);

  /**
   * Manually add an entry to the container consisting of a previously composed 
   * entry std::pair.
   *
   * \param ipfacePair the pair of a pointer to ccnx object and face index of the ccnxface to add to the container
   *
   * @see ccnxfaceContainer
   */
  void Add (std::pair<Ptr<Ccnx>, uint32_t> ipFacePair);

  /**
   * Manually add an entry to the container consisting of the individual parts
   * of an entry std::pair.
   *
   * \param ccnxName std:string referring to the saved name of an ccnx Object that
   *        has been previously named using the Object Name Service.
   * \param face face index of the ccnxface to add to the container
   *
   * @see ccnxfaceContainer
   */
  void Add (std::string ccnxName, uint32_t face);

  /**
   * Get the std::pair of an Ptr<Ccnx> and face stored at the location 
   * specified by the index.
   *
   * \param i the index of the entery to retrieve.
   *
   * @see ccnxfaceContainer
   */
  std::pair<Ptr<Ccnx>, uint32_t> Get (uint32_t i) const;

private:

  typedef std::vector<std::pair<Ptr<Ccnx>,uint32_t> > FaceVector;
  FaceVector m_faces;
};

} // namespace ns3

#endif /* CCNX_FACE_CONTAINER_H */
