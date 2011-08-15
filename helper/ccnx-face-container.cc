/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ccnx-face-container.h"
#include "ns3/node-list.h"
#include "ns3/names.h"

namespace ns3 {

CcnxFaceContainer::CcnxFaceContainer ()
{
}

void
CcnxFaceContainer::Add (CcnxFaceContainer other)
{
  for (FaceVector::const_iterator i = other.m_faces.begin (); i != other.m_faces.end (); i++)
    {
      m_faces.push_back (*i);
    }
}

CcnxFaceContainer::Iterator
CcnxFaceContainer::Begin (void) const
{
  return m_faces.begin ();
}

CcnxFaceContainer::Iterator
CcnxFaceContainer::End (void) const
{
  return m_faces.end ();
}

uint32_t
CcnxFaceContainer::GetN (void) const
{
  return m_faces.size ();
}

// CcnxAddress
// CcnxFaceContainer::GetAddress (uint32_t i, uint32_t j) const
// {
//   Ptr<Ccnx> ccnx = m_faces[i].first;
//   uint32_t face = m_faces[i].second;
//   return ccnx->GetAddress (face, j).GetLocal ();
// }

void 
CcnxFaceContainer::SetMetric (uint32_t i, uint16_t metric)
{
  Ptr<Ccnx> ccnx = m_faces[i].first;
  uint32_t face = m_faces[i].second;
  ccnx->SetMetric (face, metric);
}

void 
CcnxFaceContainer::Add (Ptr<Ccnx> ccnx, uint32_t face)
{
  m_faces.push_back (std::make_pair (ccnx, face));
}

void CcnxFaceContainer::Add (std::pair<Ptr<Ccnx>, uint32_t> a)
{
  Add (a.first, a.second);
}

void 
CcnxFaceContainer::Add (std::string ccnxName, uint32_t face)
{
  Ptr<Ccnx> ccnx = Names::Find<Ccnx> (ccnxName);
  m_faces.push_back (std::make_pair (ccnx, face));
}

std::pair<Ptr<Ccnx>, uint32_t>
CcnxFaceContainer::Get (uint32_t i) const
{
  return m_faces[i];
}


} // namespace ns3
