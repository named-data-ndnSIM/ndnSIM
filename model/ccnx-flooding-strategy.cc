/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ccnx-flooding-strategy.h"
#include "ns3/assert.h"

#include "ccnx-route.h"

NS_LOG_COMPONENT_DEFINE ("CcnxFloodingStrategy");

namespace ns3 
{
    
NS_OBJECT_ENSURE_REGISTERED (CcnxFloodingStrategy);
    
TypeId CcnxFloodingStrategy::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::CcnxFloodingStrategy")
    .SetGroupName ("Ccnx")
    .SetParent<Object> ()
    ;
    return tid;
}
    
CcnxFloodingStrategy::CcnxFloodingStrategy ()
{
}
    
    
bool
CcnxFloodingStrategy::PropagateInterest  (const Ptr<CcnxFace> &incomingFace,
                                          Ptr<CcnxInterestHeader> &header,
                                          const Ptr<const Packet> &packet,
                                          SendCallback ucb)
{
    //CcnxFibEntryContainer::type::iterator fibEntryArray = GetFib()->LongestPrefixMatch(*header);
    NS_LOG_FUNCTION(this);
    
    CcnxFibEntryContainer::type::iterator fibEntryArray = GetCcnx()->GetObject<CcnxFib>()->LongestPrefixMatch(*header);
    NS_LOG_INFO(*fibEntryArray);
    
    int count = 0;
    for(CcnxFibFaceMetricContainer::type::iterator face = fibEntryArray->m_faces.begin ();
        face != fibEntryArray->m_faces.end ();
        face++)
    {
        if(face->m_face == incomingFace)
            continue;
        
        NS_LOG_INFO("count="<<count);
        ucb (face->m_face,  header, packet->Copy());
        count++;
    }
    
    /*const CcnxFibEntryContainer& s,
    
    for (CcnxFibEntryContainer::type::iterator entry = fibEntryArray.begin ();
         entry != fibEntryArray.end ();
         entry++)
    {
    
        const typename boost::multi_index::index<CcnxFibEntryContainer, Tag>::type& i = get<Tag>(s);
        
        typedef typename CcnxFibEntryContainer::value_type value_type;
        
        for(const CcnxFibEntryContainer& c = i.begin(); c != i.end (); c++)
        {
                c->
        }
        
        for(nth_index<CcnxFibEntryContainer,1>::type::iterator it1=get<i_prefix>(entry).begin();
            it1!=get<i_prefix>(entry).end();++it1)
        {
            //std::cout<<it1->name()<<std::endl;
            
           CcnxFibFaceMetricContainer faceContainer = it1->m_faces;
            
            const typename boost::multi_index::index<CcnxFibFaceMetricContainer, __ccnx_private::i_face>::type& i = get<__ccnx_private::i_face>(faceContainer);
             
             //typedef typename CcnxFibEntryContainer::value_type value_type;
             
             for(const CcnxFibFaceMetricContainer& c = i.begin(); c != i.end (); c++)
             {
                 Ptr<CcnxFace> face = c->m_face;
                 
                 typedef
                 Callback<void, const Ptr<CcnxFace> &, const Ptr<CcnxInterestHeader> &, const Ptr<Packet> &>
                 SendCallback;
                 
                 ucb (face, header, packet);
             }
            
        }
        
        
        // obtain a reference to the index tagged by Tag
        
        const typename boost::multi_index::index<MultiIndexContainer,Tag>::type& i=
        get<Tag>(s);
        
        typedef typename MultiIndexContainer::value_type value_type;
        
        // dump the elements of the index to cout 
        
        std::copy(i.begin(),i.end(),std::ostream_iterator<value_type>(std::cout));
        */
    
    if(count == 0)
        return false;
    else
        return true;
}
    
} //namespace ns3
