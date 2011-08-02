//
//  ndn_face.h
//  Abstraction
//
//  Copyright 2011 UCLA. All rights reserved.
//

#ifndef ndn_face_h
#define ndn_face_h

#include "ns3/header.h"

namespace ns3
{
    // see http://www.ccnx.org/releases/latest/doc/ccode/html/structface.html
    class NdnFace
    {
        public:
            NdnFace(int faceID);
            virtual ~NdnFace();
        
        private:
        int m_faceID;
        
    };
    
}

#endif