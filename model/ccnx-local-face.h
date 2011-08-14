/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Authors: 
 */
#ifndef CCNX_LOCAL_FACE_H
#define CCNX_LOCAL_FACE_H

#include "ccnx-face.h"

namespace ns3 {

class NetDevice;
class Packet;
class Node;

/**
 * \brief The Ccnx representation of a network interface
 *
 * This class roughly corresponds to the struct in_device
 * of Linux; the main purpose is to provide address-family
 * specific information (addresses) about an interface.
 *
 * By default, Ccnx interface are created in the "down" state
 * no IP addresses.  Before becoming useable, the user must 
 * add an address of some type and invoke Setup on them.
 */
class CcnxLocalFace  : public CcnxFace
{
public:
  static TypeId GetTypeId (void);

  CcnxLocalFace ();
  virtual ~CcnxLocalFace();

  // Create ();

  // Create (Name::Components);

  // typedef callback ?
  
  /**
   * \param p packet to send
   */ 
  virtual void Send (Ptr<Packet> p);

protected:
  virtual void DoDispose (void);

private:
  /** disabled for LocalFace **/
  virtual Ptr<NetDevice> GetDevice (void) const;
  virtual void SetDevice (Ptr<NetDevice> device);
};

std::ostream& operator<< (std::ostream& os, CcnxLocalFace const& localFace);

} // namespace ns3

#endif
