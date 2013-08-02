/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * GNU 3.0 license, See the LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ns3/ndnSIM-module.h"

using namespace ns3;
using namespace ndn;

static inline void
__dummy_function_on_interest_callback_instantiation (Ptr<const Name>, Ptr<const Interest>)
{
}

static inline void
__dummy_function_on_data_callback_instantiation (Ptr<const Interest>, Ptr<const Data>)
{
}

static inline void
__dummy_function_on_timeout_callback_instantiation (Ptr<const Interest>)
{
}

static inline void
__dummy_function_to_force_ndn_api_face_callback_instantiations ()
{
  ApiFace face (0);
  Ptr<Interest> interest;
  Ptr<Name> prefix;

  face.ExpressInterest (interest,
                        MakeCallback (__dummy_function_on_data_callback_instantiation),
                        MakeCallback (__dummy_function_on_timeout_callback_instantiation)
                        );

  face.SetInterestFilter (prefix,
                          MakeCallback (__dummy_function_on_interest_callback_instantiation));

  std::string tmp ("bla");
  char &test = tmp[0];
}


// /// @cond include_hidden
// #ifdef PYTHON_SCAN
// struct CallbackVoidNameInterest : public Callback<void, Ptr<const Name>, Ptr<const Interest> > { };
// struct CallbackVoidInterestData : public Callback<void, Ptr<const Interest>, Ptr<const Data> > { };
// struct CallbackVoidInterest : public Callback<void, Ptr<const Interest> > { };
// #endif
// /// @endcond

