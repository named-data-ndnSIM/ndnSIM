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

#include "callback-based-app.h"

#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("CallbackBasedApp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CallbackBasedApp);
    
TypeId
CallbackBasedApp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CallbackBasedApp")
    .SetGroupName ("Ndn")
    .SetParent<Application> ()
    .AddConstructor<CallbackBasedApp> ()

    .AddAttribute ("OnStart", "OnStart callback",
                   CallbackValue (),
                   MakeCallbackAccessor (&CallbackBasedApp::m_onStart),
                   MakeCallbackChecker ())

    .AddAttribute ("OnStop", "OnStop callback",
                   CallbackValue (),
                   MakeCallbackAccessor (&CallbackBasedApp::m_onStop),
                   MakeCallbackChecker ())
    ;
  return tid;
}

CallbackBasedApp::CallbackBasedApp ()
{
}

CallbackBasedApp::~CallbackBasedApp ()
{
}

void
CallbackBasedApp::SetOnStartCallback (Callback< void, Ptr<Application> > onStart)
{
  m_onStart = onStart;
}

void
CallbackBasedApp::SetOnStopCallback (Callback< void, Ptr<Application> > onStop)
{
  m_onStop = onStop;
}

void
CallbackBasedApp::StartApplication ()
{
  NS_LOG_FUNCTION (this);
  if (!m_onStart.IsNull ())
    m_onStart (this);
}

void
CallbackBasedApp::StopApplication ()
{
  NS_LOG_FUNCTION (this);
  if (!m_onStop.IsNull ())
    m_onStop (this);
}

}
