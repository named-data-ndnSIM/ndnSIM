/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 * Author: 
 */

#ifndef _CCNX_DECODING_HELPER_H_
#define _CCNX_DECODING_HELPER_H_

#include <sys/types.h>
#include <boost/any.hpp>
#include <list>

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/buffer.h"
#include "ns3/simple-ref-count.h"


namespace ns3 {

namespace Name{ class Components; }

class CcnxInterestHeader;
class CcnxContentObjectHeader;

namespace CcnxParser {
class InterestVisitor;
class ContentObjectVisitor;
}

/**
 * Helper to encode/decode ccnb formatted CCNx message
 *
 */
class CcnxDecodingHelper
{
public:
  static size_t
  Deserialize (Buffer::Iterator start, const CcnxInterestHeader &interest);

  static size_t
  Deserialize (Buffer::Iterator start, const CcnxContentObjectHeader &contentObject);
  
private:
  static CcnxParser::InterestVisitor m_interestVisitor;
  static CcnxParser::ContentObjectVisitor m_contentObjectVisitor;
};

namespace CcnxParser {

class Block;
class Blob;
class Udata;
class Tag;
class Attr;
class Dtag;
class Dattr;
class Ext;

class Visitor
{
public:
  virtual void visit (Blob& )=0;
  virtual void visit (Udata&)=0;
  virtual void visit (Tag&  )=0;
  virtual void visit (Attr& )=0;
  virtual void visit (Dtag& )=0;
  virtual void visit (Dattr&)=0;
  virtual void visit (Ext&  )=0;
};
  
class GJVisitor
{
public:
  virtual boost::any visit (Blob&,  boost::any)=0;
  virtual boost::any visit (Udata&, boost::any)=0;
  virtual boost::any visit (Tag&,   boost::any)=0;
  virtual boost::any visit (Attr&,  boost::any)=0;
  virtual boost::any visit (Dtag&,  boost::any)=0;
  virtual boost::any visit (Dattr&, boost::any)=0;
  virtual boost::any visit (Ext&,   boost::any)=0;
};
  
class GJNoArguVisitor
{
public:
  virtual boost::any visit (Blob& )=0;
  virtual boost::any visit (Udata&)=0;
  virtual boost::any visit (Tag&  )=0;
  virtual boost::any visit (Attr& )=0;
  virtual boost::any visit (Dtag& )=0;
  virtual boost::any visit (Dattr&)=0;
  virtual boost::any visit (Ext&  )=0;
};

class GJVoidVisitor
{
public:
  virtual void visit (Blob&,  boost::any)=0;
  virtual void visit (Udata&, boost::any)=0;
  virtual void visit (Tag&,   boost::any)=0;
  virtual void visit (Attr&,  boost::any)=0;
  virtual void visit (Dtag&,  boost::any)=0;
  virtual void visit (Dattr&, boost::any)=0;
  virtual void visit (Ext&,   boost::any)=0;
};

class Block : public SimpleRefCount<Block>
{
public:
  /**
   * Parsing block header and creating an appropriate object
   */
  static Ptr<Block>
  ParseBlock (Buffer::Iterator &start);
  
  virtual void accept( Visitor &v )                           =0;
  virtual void accept (GJVoidVisitor &v, boost::any param)    =0;
  virtual boost::any accept( GJNoArguVisitor &v )             =0;
  virtual boost::any accept( GJVisitor &v, boost::any param ) =0;
};

class Blob : public Block
{
public:
  Blob (Buffer::Iterator &start, uint32_t length);
  
  virtual void accept( Visitor &v )                           { v.visit( *this ); }
  virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( GJNoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

  Buffer m_blob;
};

class Udata : public Block
{
public:
  Udata (Buffer::Iterator &start, uint32_t length);
  
  virtual void accept( Visitor &v )                           { v.visit( *this ); }
  virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( GJNoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

  std::string m_udata;
};

class Tag : public Block
{
public:
  Tag (Buffer::Iterator &start, uint32_t length);

  virtual void accept( Visitor &v )                           { v.visit( *this ); }
  virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( GJNoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

  std::string m_tag;
  std::list<Ptr<Block> > m_nestedBlocks;
};

class Attr : public Block
{
public:
  Attr (Buffer::Iterator &start, uint32_t length);
  
  virtual void accept( Visitor &v )                           { v.visit( *this ); }
  virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( GJNoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

  std::string m_attr;
  Ptr<Udata> m_value;
};

class Dtag : public Block
{
public:
  Dtag (Buffer::Iterator &start, uint32_t dtag);

  virtual void accept( Visitor &v )                           { v.visit( *this ); }
  virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( GJNoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

  uint32_t m_dtag;
  std::list<Ptr<Block> > m_nestedBlocks;
};

class Dattr : public Block
{
public:
  Dattr (Buffer::Iterator &start, uint32_t dattr);

  virtual void accept( Visitor &v )                           { v.visit( *this ); }
  virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( GJNoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

  uint32_t m_dattr;
  Ptr<Udata> m_value;
};

class Ext : public Block
{
public:
  Ext (Buffer::Iterator &start, uint32_t extSubtype);

  virtual void accept( Visitor &v )                           { v.visit( *this ); }
  virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( GJNoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

  uint64_t m_extSubtype;
};

class DepthFirstVisitor : public Visitor
{
public:
  virtual void visit (Blob& );
  virtual void visit (Udata&);
  virtual void visit (Tag&  );
  virtual void visit (Attr& );
  virtual void visit (Dtag& );
  virtual void visit (Dattr&);
  virtual void visit (Ext&  );
};

class GJDepthFirstVisitor : public GJVisitor
{
public:
  virtual boost::any visit (Blob&,  boost::any);
  virtual boost::any visit (Udata&, boost::any);
  virtual boost::any visit (Tag&,   boost::any);
  virtual boost::any visit (Attr&,  boost::any);
  virtual boost::any visit (Dtag&,  boost::any);
  virtual boost::any visit (Dattr&, boost::any);
  virtual boost::any visit (Ext&,   boost::any);
};

class GJNoArguDepthFirstVisitor : public GJNoArguVisitor
{
public:
  virtual boost::any visit (Blob& );
  virtual boost::any visit (Udata&);
  virtual boost::any visit (Tag&  );
  virtual boost::any visit (Attr& );
  virtual boost::any visit (Dtag& );
  virtual boost::any visit (Dattr&);
  virtual boost::any visit (Ext&  );
};

class GJVoidDepthFirstVisitor : public GJVoidVisitor
{
public:
  virtual void visit (Blob&,  boost::any);
  virtual void visit (Udata&, boost::any);
  virtual void visit (Tag&,   boost::any);
  virtual void visit (Attr&,  boost::any);
  virtual void visit (Dtag&,  boost::any);
  virtual void visit (Dattr&, boost::any);
  virtual void visit (Ext&,   boost::any);
};

// class NameComponentsVisitor : public 

class NonNegativeIntegerVisitor : public GJNoArguDepthFirstVisitor
{
public:
  virtual boost::any visit (Blob &n); //to throw parsing error
  virtual boost::any visit (Udata &n);
};

class StringVisitor : public GJNoArguDepthFirstVisitor
{
public:
  virtual boost::any visit (Blob &n); //to throw parsing error
  virtual boost::any visit (Udata &n);
};

class NameComponentsVisitor : public GJVoidDepthFirstVisitor
{
public:
  virtual void visit (Dtag &n, boost::any param/*should be Name::Components*/);
private:
  static StringVisitor m_stringVisitor; 
};

class InterestVisitor : public GJVoidDepthFirstVisitor
{
public:
  virtual void visit (Dtag &n, boost::any param/*should be CcnxInterestHeader&*/);
  
private:
  static NonNegativeIntegerVisitor m_nonNegativeIntegerVisitor;
  static NameComponentsVisitor     m_nameComponentsVisitor;
};

class ContentObjectVisitor : public GJVoidDepthFirstVisitor
{
public:
  virtual void visit (Dtag &n, boost::any param/*should be CcnxContentObjectHeader&*/);
  
private:
  static NameComponentsVisitor     m_nameComponentsVisitor;
};


class CcnxDecodingException {};

} // namespace CcnxParser

} // namespace ns3

#endif // _CCNX_DECODING_HELPER_H_

