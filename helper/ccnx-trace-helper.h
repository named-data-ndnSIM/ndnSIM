/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCNX_TRACE_HELPER_H
#define CCNX_TRACE_HELPER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/ccnx.h"

namespace ns3 {

class CcnxApp;

class CcnxAppTracer : public SimpleRefCount<CcnxAppTracer>
{
public:
  CcnxAppTracer (const std::string &app, const std::string &node = "*", const std::string &appId = "*");
  virtual ~CcnxAppTracer ()  { };

  virtual void
  PrintHeader (std::ostream &os) const = 0;
  
  virtual void
  Print (std::ostream &os) const = 0;

  virtual void
  OutInterests (std::string context,
                Ptr<const CcnxInterestHeader>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;

  virtual void
  InInterests  (std::string context,
                Ptr<const CcnxInterestHeader>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;

  virtual void
  InNacks (std::string context,
           Ptr<const CcnxInterestHeader>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;

  virtual void
  OutData (std::string context,
           Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;
  
  virtual void
  InData  (std::string context,
           Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;

protected:
  std::string m_app;
  std::string m_appId;
  std::string m_node;
};

std::ostream&
operator << (std::ostream &os, const CcnxAppTracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

class CcnxL3Tracer : public SimpleRefCount<CcnxL3Tracer>
{
public:
  CcnxL3Tracer (const std::string &node = "*");
  virtual ~CcnxL3Tracer () { };

  virtual void
  PrintHeader (std::ostream &os) const = 0;

  virtual void
  Print (std::ostream &os) const = 0;
  
  virtual void
  OutInterests  (std::string context,
                 Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>) = 0;

  virtual void
  InInterests   (std::string context,
                 Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>) = 0;

  virtual void
  DropInterests (std::string context,
                 Ptr<const CcnxInterestHeader>, Ccnx::DropReason, Ptr<const CcnxFace>) = 0;
  
  virtual void
  OutNacks  (std::string context,
             Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>) = 0;

  virtual void
  InNacks   (std::string context,
             Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>) = 0;

  virtual void
  DropNacks (std::string context,
             Ptr<const CcnxInterestHeader>, Ccnx::DropReason, Ptr<const CcnxFace>) = 0;

  
  virtual void
  OutData  (std::string context,
            Ptr<const CcnxContentObjectHeader>, bool fromCache, Ptr<const CcnxFace>) = 0;

  virtual void
  InData   (std::string context,
            Ptr<const CcnxContentObjectHeader>, Ptr<const CcnxFace>) = 0;

  virtual void
  DropData (std::string context,
            Ptr<const CcnxContentObjectHeader>, Ccnx::DropReason, Ptr<const CcnxFace>) = 0;

protected:
  std::string m_node;
};

std::ostream&
operator << (std::ostream &os, const CcnxL3Tracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

class CcnxAggregateAppTracer : public CcnxAppTracer
{
public:
  CcnxAggregateAppTracer (const std::string &app, const std::string &node = "*", const std::string &appId = "*");
  virtual ~CcnxAggregateAppTracer () { };

  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

  virtual void
  OutInterests (std::string context,
                Ptr<const CcnxInterestHeader>, Ptr<CcnxApp>, Ptr<CcnxFace>);

  virtual void
  InInterests  (std::string context,
                Ptr<const CcnxInterestHeader>, Ptr<CcnxApp>, Ptr<CcnxFace>);

  virtual void
  InNacks (std::string context,
           Ptr<const CcnxInterestHeader>, Ptr<CcnxApp>, Ptr<CcnxFace>);

  virtual void
  OutData (std::string context,
           Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>, Ptr<CcnxApp>, Ptr<CcnxFace>);
  
  virtual void
  InData  (std::string context,
           Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>, Ptr<CcnxApp>, Ptr<CcnxFace>);

private:
  uint64_t m_inInterests;
  uint64_t m_outInterests;
  uint64_t m_inNacks;
  uint64_t m_inData; 
  uint64_t m_outData;
};

class CcnxAggregateL3Tracer : public CcnxL3Tracer
{
public:
  CcnxAggregateL3Tracer (const std::string &node = "*");
  virtual ~CcnxAggregateL3Tracer () { };
  
  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

  virtual void
  OutInterests  (std::string context,
                 Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>);

  virtual void
  InInterests   (std::string context,
                 Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>);

  virtual void
  DropInterests (std::string context,
                 Ptr<const CcnxInterestHeader>, Ccnx::DropReason, Ptr<const CcnxFace>);
  
  virtual void
  OutNacks  (std::string context,
             Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>);

  virtual void
  InNacks   (std::string context,
             Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>);

  virtual void
  DropNacks (std::string context,
             Ptr<const CcnxInterestHeader>, Ccnx::DropReason, Ptr<const CcnxFace>);
  
  virtual void
  OutData  (std::string context,
            Ptr<const CcnxContentObjectHeader>, bool fromCache, Ptr<const CcnxFace>);

  virtual void
  InData   (std::string context,
            Ptr<const CcnxContentObjectHeader>, Ptr<const CcnxFace>);

  virtual void
  DropData (std::string context,
            Ptr<const CcnxContentObjectHeader>, Ccnx::DropReason, Ptr<const CcnxFace>);

private:
  uint64_t m_inInterests;
  uint64_t m_outInterests;
  uint64_t m_dropInterests;
  uint64_t m_inNacks;
  uint64_t m_outNacks;
  uint64_t m_dropNacks;
  uint64_t m_inData;
  uint64_t m_outData;
  uint64_t m_dropData;
};

class CcnxTraceHelper
{
public:
  /**
   * @brief Destructor that invokes trace output procedures
   */
  ~CcnxTraceHelper ();

  /**
   * @brief Set filename to output app trace.
   *
   * By default, trace is output to NS_LOG_INFO stream
   *
   * @param file File where trace will be written to
   */
  void
  SetAppTraceFile (const std::string &appTrace = "apps.log");

  /**
   * @brief Set filename to output app trace.
   *
   * By default, trace is output to NS_LOG_INFO stream
   *
   * @param file File where trace will be written to
   */
  void
  SetL3TraceFile (const std::string &l3Trace = "l3.log");
  
  /**
   * @brief Enable aggregate app-level CCNx tracing on all CCNx applications (individual file per application)
   *
   * @param app  Class name of the application of interest
   */
  void
  EnableAggregateAppAll (const std::string &app);

  /**
   * @brief Enable aggregate network-level CCNx tracing on all CCNx nodes (individual file per node)
   */
  void
  EnableAggregateL3All ();

private:
  std::string m_appTrace;
  std::list<Ptr<CcnxAggregateAppTracer> > m_apps;

  std::string m_l3Trace;
  std::list<Ptr<CcnxAggregateL3Tracer> > m_l3s;
};


} // namespace ns3

#endif /* CCNX_TRACE_HELPER_H */
