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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/functional/hash.hpp>

namespace bi = boost::intrusive;

/////////////////////////////////////////////////////
// Allow customization for payload
//
template<typename Payload>
struct pointer_payload_traits
{
  typedef Payload*        pointer_type;
  typedef const Payload*  const_pointer_type;

  static const Payload* empty_payload;
};

template<typename Payload>
const Payload*
pointer_payload_traits<Payload>::empty_payload = 0;

template<typename Payload>
struct smart_pointer_payload_traits
{
  typedef ns3::Ptr<Payload>       pointer_type;
  typedef ns3::Ptr<const Payload> const_pointer_type;
  
  static const ns3::Ptr<const Payload> empty_payload;
};

template<typename Payload>
const ns3::Ptr<const Payload>
smart_pointer_payload_traits<Payload>::empty_payload = 0;


////////////////////////////////////////////////////
// forward declarations
//
template<typename FullKey, typename Payload, typename PayloadTraits = pointer_payload_traits<Payload> >
class trie; 

template<typename FullKey, typename Payload, typename PayloadTraits>
inline std::ostream&
operator << (std::ostream &os, const trie<FullKey, Payload, PayloadTraits> &trie_node);

template<typename FullKey, typename Payload, typename PayloadTraits>
bool
operator== (const trie<FullKey, Payload, PayloadTraits> &a, const trie<FullKey, Payload, PayloadTraits> &b);

template<typename FullKey, typename Payload, typename PayloadTraits >
std::size_t
hash_value (const trie<FullKey, Payload, PayloadTraits> &trie_node);

///////////////////////////////////////////////////
// actual definition
//


template<typename FullKey,
	 typename Payload, typename PayloadTraits>
class trie
{
public:
  typedef typename FullKey::partial_type Key;

  typedef trie*       iterator;
  typedef const trie* const_iterator;
  
  inline
  trie (const Key &key, size_t bucketSize = 10, size_t bucketIncrement = 10);

  inline
  ~trie ();
  
  // actual entry
  friend bool
  operator== <> (const trie<FullKey, Payload, PayloadTraits> &a, const trie<FullKey, Payload, PayloadTraits> &b);

  friend std::size_t
  hash_value <> (const trie<FullKey, Payload, PayloadTraits> &trie_node);

  inline std::pair<iterator, bool>
  insert (const FullKey &key, typename PayloadTraits::const_pointer_type payload);

  /**
   * @brief Removes payload (if it exists) and if there are no children, prunes parents trie
   */
  inline void
  erase ();

  /**
   * @brief Do exactly as erase, but without erasing the payload
   */
  inline void
  prune ();

  /**
   * @brief Perform the longest prefix match
   * @param key the key for which to perform the longest prefix match
   *
   * @return ->second is true if prefix in ->first is longer than key
   */
  inline boost::tuple<iterator, bool, iterator>
  find (const FullKey &key);

  /**
   * @brief Find next payload of the sub-trie
   * @returns end() or a valid iterator pointing to the trie leaf (order is not defined, enumeration )
   */
  inline iterator
  find ();

  /**
   * @brief Find next payload of the sub-trie satisfying the predicate
   * @param pred predicate
   * @returns end() or a valid iterator pointing to the trie leaf (order is not defined, enumeration )
   */
  template<class Predicate>
  inline iterator
  find_if (Predicate pred);

  iterator end ()
  {
    return 0;
  }

  const_iterator end () const
  {
    return 0;
  }
  
  // inline const_iterator
  // find () const;
  
private:
  //The disposer object function
  struct trie_delete_disposer
  {
    void operator() (trie *delete_this)
    {
      // std::cout << "Deleting " << delete_this << "\n";
      delete delete_this;
    }
  };  

  friend
  std::ostream&
  operator<< < > (std::ostream &os, const trie &trie_node);
  
private:
  bi::unordered_set_member_hook<> member_hook_;

  // necessary typedefs
  typedef trie self_type;
  typedef bi::member_hook< trie,
  			   bi::unordered_set_member_hook< >, &trie::member_hook_ > member_hook;
  typedef bi::unordered_set< trie, member_hook >                                   unordered_set;
  typedef typename unordered_set::bucket_type   bucket_type;
  typedef typename unordered_set::bucket_traits bucket_traits;

  ////////////////////////////////////////////////
  // Actual data
  ////////////////////////////////////////////////
  
  Key key_; ///< name component

  size_t initialBucketSize_;
  size_t bucketIncrement_;

  size_t bucketSize_;
  bucket_type *buckets_;
  unordered_set children_;
  
  typename PayloadTraits::const_pointer_type payload_;
  trie *parent_; // to make cleaning effective
};

template<typename FullKey, typename Payload, typename PayloadTraits>
trie<FullKey, Payload, PayloadTraits>::trie (const trie::Key &key, size_t bucketSize, size_t bucketIncrement)
  : key_ (key)
  , initialBucketSize_ (bucketSize)
  , bucketIncrement_ (bucketIncrement)
  , bucketSize_ (initialBucketSize_)
  , buckets_ (new bucket_type [bucketSize_])
  , children_ (bucket_traits (buckets_, bucketSize_))
  , payload_ (PayloadTraits::empty_payload)
  , parent_ (0)
{
}

template<typename FullKey, typename Payload, typename PayloadTraits>
trie<FullKey, Payload, PayloadTraits>::~trie ()
{
  children_.clear_and_dispose (trie_delete_disposer ());
  delete [] buckets_;
}

template<typename FullKey, typename Payload, typename PayloadTraits>
inline
std::pair<typename trie<FullKey, Payload, PayloadTraits>::iterator, bool>
trie<FullKey, Payload, PayloadTraits>::insert (const FullKey &key, typename PayloadTraits::const_pointer_type payload)
{
  trie *trieNode = this;
  
  BOOST_FOREACH (const Key &subkey, key)
    {
      typename unordered_set::iterator item = trieNode->children_.find (subkey);
      if (item == trieNode->children_.end ())
	{
	  trie *newNode = new trie (subkey, initialBucketSize_, bucketIncrement_);
	  // std::cout << "new " << newNode << "\n";
	  newNode->parent_ = trieNode;

	  if (trieNode->children_.size () > trieNode->bucketSize_)
	    {
	      trieNode->bucketSize_ += trieNode->bucketIncrement_;
	      bucket_type *newBuckets = new bucket_type [trieNode->bucketSize_];
	      trieNode->children_.rehash (bucket_traits (newBuckets, trieNode->bucketSize_));
	      delete [] trieNode->buckets_;
	      trieNode->buckets_ = newBuckets;
	    }
	  
	  std::pair< typename unordered_set::iterator, bool > ret =
	    trieNode->children_.insert (*newNode);
	  
	  trieNode = &(*ret.first);
	}
      else
	trieNode = &(*item);
    }

  if (trieNode->payload_ == 0)
    {
      trieNode->payload_ = payload;
      return std::make_pair (trieNode, true);
    }
  else
    return std::make_pair (trieNode, false);
}

template<typename FullKey, typename Payload, typename PayloadTraits>
inline
boost::tuple<typename trie<FullKey, Payload, PayloadTraits>::iterator,
	     bool,
	     typename trie<FullKey, Payload, PayloadTraits>::iterator>
trie<FullKey, Payload, PayloadTraits>::find (const FullKey &key)
{
  trie *trieNode = this;
  iterator foundNode = (payload_ != PayloadTraits::empty_payload) ? this : 0;
  bool reachLast = true;
  
  BOOST_FOREACH (const Key &subkey, key)
    {
      typename unordered_set::iterator item = trieNode->children_.find (subkey);
      if (item == trieNode->children_.end ())
	{
	  reachLast = false;
	  break;
	}
      else
	{
	  trieNode = &(*item);
	  
	  if (trieNode->payload_ != PayloadTraits::empty_payload)
	    foundNode = trieNode;
	}
    }

  return boost::make_tuple (foundNode, reachLast, trieNode);  
}

template<typename FullKey, typename Payload, typename PayloadTraits>
inline typename trie<FullKey, Payload, PayloadTraits>::iterator
trie<FullKey, Payload, PayloadTraits>::find ()
{
  if (payload_ != PayloadTraits::empty_payload)
    return this;

  typedef trie<FullKey, Payload, PayloadTraits> trie;
  BOOST_FOREACH (const trie &subnode, children_)
    {
      iterator value = subnode.find ();
      if (value != 0)
	return value;
    }
  
  return 0;
}

template<typename FullKey, typename Payload, typename PayloadTraits>
template<class Predicate>
inline typename trie<FullKey, Payload, PayloadTraits>::iterator
trie<FullKey, Payload, PayloadTraits>::find_if (Predicate pred)
{
  if (payload_ != PayloadTraits::empty_payload && pred (payload_))
    return this;

  typedef trie<FullKey, Payload, PayloadTraits> trie;
  BOOST_FOREACH (const trie &subnode, children_)
    {
      iterator value = subnode.find ();
      if (value != 0)
	return value;
    }
  
  return 0;
}


template<typename FullKey, typename Payload, typename PayloadTraits>
inline void
trie<FullKey, Payload, PayloadTraits>::erase ()
{
  payload_ = PayloadTraits::empty_payload;
  prune ();
}

template<typename FullKey, typename Payload, typename PayloadTraits>
inline void
trie<FullKey, Payload, PayloadTraits>::prune ()
{
  if (payload_ == 0 && children_.size () == 0)
    {
      if (parent_ == 0) return;

      trie *parent = parent_;
      parent->children_.erase_and_dispose (*this, trie_delete_disposer ()); // delete this; basically, committing a suicide

      parent->prune ();
    }
}

template<typename FullKey, typename Payload, typename PayloadTraits>
inline std::ostream&
operator << (std::ostream &os, const trie<FullKey, Payload, PayloadTraits> &trie_node)
{
  os << "# " << trie_node.key_ << ((trie_node.payload_ != 0)?"*":"") << std::endl;
  typedef trie<FullKey, Payload, PayloadTraits> trie;
  BOOST_FOREACH (const trie &subnode, trie_node.children_)
    {
      os << "\"" << &trie_node << "\"" << " [label=\"" << trie_node.key_ << ((trie_node.payload_ != 0)?"*":"") << "\"]\n";
      os << "\"" << &subnode << "\"" << " [label=\"" << subnode.key_ << ((subnode.payload_ != 0)?"*":"") << "\"]""\n";
      
      os << "\"" << &trie_node << "\"" << " -> " << "\"" << &subnode << "\"" << "\n";
      os << subnode;
    }

  return os;
}

template<typename FullKey, typename Payload, typename PayloadTraits>
inline bool
operator == (const trie<FullKey, Payload, PayloadTraits> &a, const trie<FullKey, Payload, PayloadTraits> &b)
{
  return a.key_ == b.key_;
}

template<typename FullKey, typename Payload, typename PayloadTraits>
inline std::size_t
hash_value (const trie<FullKey, Payload, PayloadTraits> &trie_node)
{
  return boost::hash_value (trie_node.key_);
}
