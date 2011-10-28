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

#ifndef CCNX_PRODUCER_H
#define CCNX_PRODUCER_H

#include "ns3/application.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/simulator.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-local-face.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/packet.h"
#include "ns3/boolean.h"
#include "ns3/integer.h"
#include "ns3/uinteger.h"
#include "ns3/random-variable.h"
#include <limits> 
#include "ns3/pointer.h"
#include "ns3/traced-callback.h"
#include "ns3/ccnx-header-helper.h"

#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/ccnx.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/ccnx-fib.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace ns3 
{
    
    class DataStoreEntry
    {
    public:
        /**
         * \brief Construct data store entry
         *
         * \param header Parsed CcnxContentObject header
         * \param packet Original CCNx packet
         *
         * The constructor will make a copy of the supplied packet and calls
         * RemoveHeader and RemoveTail on the copy.
         */
        DataStoreEntry (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet);
        
        /**
         * \brief Get prefix of the stored entry
         * \returns prefix of the stored entry
         */
        inline const CcnxNameComponents&
        GetName () const;
        
        /**
         * \brief Get CcnxContentObjectHeader of the stored entry
         * \returns CcnxContentObjectHeader of the stored entry
         */
        inline Ptr<const CcnxContentObjectHeader>
        GetHeader () const;
        
        /**
         * \brief Get content of the stored entry
         * \returns content of the stored entry
         */
        inline Ptr<const Packet>
        GetPacket () const;
        
        /**
         * \brief Convenience method to create a fully formed CCNx packet from stored header and content
         * \returns A read-write copy of the packet with CcnxContentObjectHeader and CcxnContentObjectTail
         */
        Ptr<Packet>
        GetFullyFormedCcnxPacket () const;
        
        // Copy constructor is required by the container. Though, we're
        // storing only two pointers, so shouldn't be a problem
        // private:
        //   CcnxContentStoreEntry (const CcnxContentStoreEntry &); ///< disabled copy constructor
        //   CcnxContentStoreEntry& operator= (const CcnxContentStoreEntry&); ///< disabled copy operator
        
    private:
        //Ptr<CcnxContentObjectHeader> m_header; ///< \brief non-modifiable CcnxContentObjectHeader
        //Ptr<Packet> m_packet; ///< \brief non-modifiable content of the ContentObject packet
    };

    
class CcnxProducer: public Application
{
public: 
    static TypeId GetTypeId (void);
        
    CcnxProducer ();
        
    virtual ~CcnxProducer ();
        
    //void HandlePacket (const Ptr<CcnxFace> &face, const Ptr<const Packet> &packet);
    //uint32_t GetStoreCapacity();
    //void SetStoreCapacity(uint32_t capacity);
    //void AddContentStoreEntry (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet);
    
    void OnInterest(const Ptr<const CcnxInterestHeader> &interest);
    
    CcnxNameComponents GetPrefix() const;  
    /**
     * \brief Add a new content to the data store.
     *
     * \param header Fully parsed CcnxContentObjectHeader
     * \param packet Fully formed CCNx packet to add to content store
     * (will be copied and stripped down of headers)
     *
     * If entry with the same prefix exists, the old entry will be
     * promoted to the top of the MRU hash
     */
    //void
    //Add (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet);
    
protected:
    virtual void DoDispose (void);
private:
    // inherited from Application base class.
    virtual void StartApplication (void);    // Called at time specified by Start
    virtual void StopApplication (void);     // Called at time specified by Stop
        
    /**
     * \brief Find corresponding CS entry for the given interest
     *
     * \param interest Interest for which matching content store entry
     * will be searched
     *
     * If an entry is found, it is promoted to the top of most recent
     * used entries index, \see m_contentStore
     */
    //Ptr<Packet>
    //Lookup (Ptr<const CcnxInterestHeader> interest);

    
    Ptr<Ccnx> m_ccnx;
    Time m_offTime;
           
    //EventId         m_sendEvent;    // Eventid of pending "send packet" event
    TypeId          m_tid;
    Ptr<CcnxLocalFace> m_face;
    
    /*struct DataStoreContainer
    {
        typedef
        boost::multi_index::multi_index_container<
          DataStoreEntry,
          boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
              boost::multi_index::tag<__ccnx_private::i_prefix>,
              boost::multi_index::const_mem_fun<DataStoreEntry,
                                                const CcnxNameComponents&,
                                                &DataStoreEntry::GetName>,
              CcnxPrefixHash>,
          boost::multi_index::sequenced<boost::multi_index::tag<__ccnx_private::i_mru> >
#ifdef _DEBUG
          ,
          boost::multi_index::ordered_unique<
          boost::multi_index::tag<__ccnx_private::i_ordered>,
          boost::multi_index::const_mem_fun<DataStoreEntry,
          const CcnxNameComponents&,
          &CcnxContentStoreEntry::GetName>
          >
#endif
        >
      > type;
    };*/

    //helpers
    void CancelEvents ();
    
    //DataStoreContainer::type m_availableData;
    //uint32_t m_storeCapacity;
    uint32_t m_virtualPayloadSize;
    CcnxNameComponents m_prefix;
    TracedCallback<const Ptr<CcnxFace>&,const Ptr<const Packet>& > m_interestsTrace;
    TracedCallback<const Ptr<CcnxFace>&,const Ptr<const Packet>& > m_contentObjectsTrace;
};
}
#endif
