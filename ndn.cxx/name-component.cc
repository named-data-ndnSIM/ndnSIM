/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder"
#elif __GNUC__
#pragma GCC diagnostic ignored "-Wreorder"
#endif

#include "name-component.h"

#include "detail/error.h"
#include "detail/uri.h"

NDN_NAMESPACE_BEGIN

namespace name {
  
Component::Component ()
{
}

Component::Component (const std::string &uri)
{
  copy (uri.begin (), uri.end (), std::back_inserter (*this));
}

Component::Component (std::string::const_iterator begin, std::string::const_iterator end)
{
  copy (begin, end, std::back_inserter (*this));
}

Component::Component (const void *buf, size_t length)
{
  copy (static_cast<const char*> (buf),
        static_cast<const char*> (buf)+length,
        std::back_inserter (*this));
}

Component &
Component::fromUri (const std::string &uri)
{
  try
    {
      Uri::fromEscaped (uri.begin (), uri.end (), std::back_inserter (*this));
    }
  catch (error::Uri &err)
    {
      // re-throwing different exception
      BOOST_THROW_EXCEPTION (error::name::Component ()
                             << error::msg (uri)
                             << error::pos (error::get_pos (err)));
    }

  return *this;
}

Component &
Component::fromUri (std::string::const_iterator begin, std::string::const_iterator end)
{
  try
    {
      Uri::fromEscaped (begin, end, std::back_inserter (*this));
    }
  catch (error::Uri &err)
    {
      // re-throwing different exception
      BOOST_THROW_EXCEPTION (error::name::Component ()
                             << error::msg (std::string (begin, end))
                             << error::pos (error::get_pos (err)));
    }
  return *this;
}

int
Component::compare (const Component &other) const
{
  if (size () < other.size ())
    return -1;

  if (size () > other.size ())
    return +1;

  // now we know that sizes are equal

  std::pair<const_iterator, const_iterator> diff = mismatch (begin (), end (), other.begin ());
  if (diff.first == end ()) // components are actually equal
    return 0;

  return (std::lexicographical_compare (diff.first, end (), diff.second, other.end ())) ? -1 : +1;    
}

Component &
Component::fromNumber (uint64_t number)
{
  while (number > 0)
    {
      this->push_back (static_cast<unsigned char> (number & 0xFF));
      number >>= 8;
    }
  std::reverse (this->begin (), this->end ());
  return *this;
}

Component &
Component::fromNumberWithMarker (uint64_t number, unsigned char marker)
{
  this->push_back (marker);

  while (number > 0)
    {
      this->push_back (static_cast<unsigned char> (number & 0xFF));
      number >>= 8;
    }

  std::reverse (this->begin () + 1, this->end ());
  return *this;
}

std::string
Component::toBlob () const
{
  return std::string (begin (), end ());
}

void
Component::toBlob (std::ostream &os) const
{
  os.write (buf (), size ());
}

std::string
Component::toUri () const
{
  std::ostringstream os;
  toUri (os);
  return os.str ();  
}

void
Component::toUri (std::ostream &os) const
{
  Uri::toEscaped (begin (), end (), std::ostream_iterator<char> (os));
}

uint64_t
Component::toNumber () const
{
  uint64_t ret = 0;
  for (const_iterator i = begin (); i != end (); i++)
    {
      ret <<= 8;
      ret |= static_cast<unsigned char> (*i);
    }
  return ret;
}

uint64_t
Component::toNumberWithMarker (unsigned char marker) const
{
  if (empty () ||
      static_cast<unsigned char> (*(begin ())) != marker)
    {
      BOOST_THROW_EXCEPTION (error::name::Component ()
                             << error::msg ("Name component does not have required marker [" + toUri () + "]"));
    }

  uint64_t ret = 0;
  for (const_iterator i = begin () + 1; i != end (); i++)
    {
      ret <<= 8;
      ret |= static_cast<unsigned char> (*i);
    }
  return ret;
}

} // name

NDN_NAMESPACE_END
