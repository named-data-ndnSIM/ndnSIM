/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *                     Zhenkai Zhu
 *
 * GNU v3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_CALLBACK_BASED_APP_H
#define NDN_CALLBACK_BASED_APP_H

#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"

namespace ns3 {

/**
 * @ingroup ndn-apps
 * @brief A meta application that can be used to create custom apps within Python bindings 
 */
class CallbackBasedApp: public Application
{
public:
  static TypeId GetTypeId ();

  /**
   * @brief Default constructor
   */
  CallbackBasedApp ();

  /**
   * @brief Virtual destructor
   */
  virtual
  ~CallbackBasedApp ();

  /**
   * @brief Define callback that will be fired when application need to start its work
   */
  void
  SetOnStartCallback (Callback< void, Ptr<Application> > onStart);

  /**
   * @brief Define callback that will be fired when application need to stop its work
   */
  void
  SetOnStopCallback (Callback< void, Ptr<Application> > onStart);

protected:
  // inherited from Application base class. Originally they were private
  virtual void
  StartApplication ();    ///< @brief Called at time specified by Start

  virtual void
  StopApplication ();     ///< @brief Called at time specified by Stop

private:
  Callback< void, Ptr<Application> > m_onStart;
  Callback< void, Ptr<Application> > m_onStop;
};

} // ns3

#endif // NDN_CALLBACK_BASED_APP_H
