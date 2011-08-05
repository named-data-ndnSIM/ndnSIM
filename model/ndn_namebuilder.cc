//
//  ndn_namebuilder.cpp
//  Abstraction
//
//  Created by Ilya Moiseenko on 04.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ndn_namebuilder.h"

namespace ns3 
{
namespace NDNabstraction
{
    NameBuilder::NameBuilder()
    {
        m_value = ccn_charbuf_create();
        ccn_name_init(m_value);
    }
    
    ccn_charbuf*
    NameBuilder::GetName()
    {
        return m_value;
    }
    
    void
    NameBuilder::AddComponent(string s)
    {
        
        ccn_name_append_str(m_value,s.c_str());
    }
}
}