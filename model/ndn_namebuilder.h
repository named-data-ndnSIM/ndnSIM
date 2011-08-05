//
//  ndn_namebuilder.h
//  Abstraction
//
//  Created by Ilya Moiseenko on 04.08.11.
//  Copyright 2011 UCLA. All rights reserved.
//

#include "ccn_ccn.h"
#include <string>

using namespace std;

namespace ns3 
{
namespace NDNabstraction
{
    class NameBuilder
    {
    public:
        NameBuilder();
        ccn_charbuf* GetName();
        void AddComponent(string s);
        //TODO add more overloads
    private:
        ccn_charbuf *m_value;
    };
}
}