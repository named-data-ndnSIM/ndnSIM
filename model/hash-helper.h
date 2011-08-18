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

#ifndef ccnx_hash_helper_h
#define ccnx_hash_helper_h

#include <string>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/foreach.hpp>

//size of content store
#define NDN_CONTENT_STORE_SIZE 100
//maximum length of content name
#define NDN_MAX_NAME_LENGTH 30

//using namespace std;

#define KEY(x)		x->first
#define VALUE(x)	x->second


/*template<typename T> 
struct hash : public std::unary_function<T, std::size_t> {
    std::size_t operator()(T const&) const;
};*/

struct string_hash : public std::unary_function<std::string, std::size_t>
{
	inline std::size_t operator( )( std::string str ) const
	{
        std::size_t hash = str.size() + 23;
		for( std::string::const_iterator it = str.begin( ); it!=str.end(); it++ )
		{
			hash = ((hash << 6) ^ (hash >> 27)) + static_cast<std::size_t>( *it );
		}
		
		return boost::hash_value(hash); //hash;
	}
};

// A collision-chaining hash table mapping strings to ints.
template<typename Value>
class string_key_hash_t : public boost::unordered_map<std::string,Value, string_hash, std::equal_to<std::string>,std::allocator<std::string> >
{
};

#endif