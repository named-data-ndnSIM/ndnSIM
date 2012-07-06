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

#ifndef TRIE_H_
#define TRIE_H_

#include "ns3/ptr.h"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/functional/hash.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

namespace ndnSIM
{

/////////////////////////////////////////////////////
// Allow customization for payload
//
template<typename Payload>
struct pointer_payload_traits
{
  typedef Payload         payload_type;
  typedef Payload*        pointer_type;
  typedef const Payload*  const_pointer_type;

  static Payload* empty_payload;
};

template<typename Payload>
Payload*
pointer_payload_traits<Payload>::empty_payload = 0;

template<typename Payload>
struct smart_pointer_payload_traits
{
  typedef Payload                 payload_type;
  typedef ns3::Ptr<Payload>       pointer_type;
  typedef ns3::Ptr<const Payload> const_pointer_type;
  
  static ns3::Ptr<Payload> empty_payload;
};

template<typename Payload>
ns3::Ptr<Payload>
smart_pointer_payload_traits<Payload>::empty_payload = 0;


////////////////////////////////////////////////////
// forward declarations
//
template<typename FullKey,
         typename Payload,
         typename PayloadTraits,
         typename PolicyHook >
class trie; 

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline std::ostream&
operator << (std::ostream &os,
             const trie<FullKey, Payload, PayloadTraits, PolicyHook> &trie_node);

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
bool
operator== (const trie<FullKey, Payload, PayloadTraits, PolicyHook> &a,
            const trie<FullKey, Payload, PayloadTraits, PolicyHook> &b);

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook >
std::size_t
hash_value (const trie<FullKey, Payload, PayloadTraits, PolicyHook> &trie_node);

///////////////////////////////////////////////////
// actual definition
//


template<typename FullKey,
	 typename Payload, typename PayloadTraits,
         typename PolicyHook >
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

  void
  clear ()
  {
    children_.clear_and_dispose (trie_delete_disposer ());
  }
  
  // actual entry
  friend bool
  operator== <> (const trie<FullKey, Payload, PayloadTraits, PolicyHook> &a,
                 const trie<FullKey, Payload, PayloadTraits, PolicyHook> &b);

  friend std::size_t
  hash_value <> (const trie<FullKey, Payload, PayloadTraits, PolicyHook> &trie_node);

  inline std::pair<iterator, bool>
  insert (const FullKey &key,
          typename PayloadTraits::pointer_type payload);

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

  inline boost::tuple<const iterator, bool, const iterator>
  find (const FullKey &key) const
  {
    return const_cast<trie*> (this)->find (key);
  }

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

  typename PayloadTraits::const_pointer_type
  payload () const
  {
    return payload_;
  }

  typename PayloadTraits::pointer_type
  payload ()
  {
    return payload_;
  }

  void
  set_payload (typename PayloadTraits::pointer_type payload)
  {
    payload_ = payload;
  }
  
  inline void
  PrintStat (std::ostream &os) const;  
  
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

  template<class D>
  struct array_disposer
  {
    void operator() (D *array)
    {
      delete [] array;
    }
  };

  friend
  std::ostream&
  operator<< < > (std::ostream &os, const trie &trie_node);

public:
  PolicyHook policy_hook_;

private:
  boost::intrusive::unordered_set_member_hook<> unordered_set_member_hook_;

  // necessary typedefs
  typedef trie self_type;
  typedef boost::intrusive::member_hook< trie,
                                         boost::intrusive::unordered_set_member_hook< >,
                                         &trie::unordered_set_member_hook_ > member_hook;
  typedef boost::intrusive::unordered_set< trie, member_hook > unordered_set;
  typedef typename unordered_set::bucket_type   bucket_type;
  typedef typename unordered_set::bucket_traits bucket_traits;

  ////////////////////////////////////////////////
  // Actual data
  ////////////////////////////////////////////////
  
  Key key_; ///< name component

  size_t initialBucketSize_;
  size_t bucketIncrement_;

  size_t bucketSize_;
  typedef boost::interprocess::unique_ptr< bucket_type, array_disposer<bucket_type> > buckets_array;
  buckets_array buckets_;
  unordered_set children_;
  
  typename PayloadTraits::pointer_type payload_;
  trie *parent_; // to make cleaning effective
};

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::trie (const trie::Key &key, size_t bucketSize, size_t bucketIncrement)
  : key_ (key)
  , initialBucketSize_ (bucketSize)
  , bucketIncrement_ (bucketIncrement)
  , bucketSize_ (initialBucketSize_)
  , buckets_ (new bucket_type [bucketSize_]) //cannot use normal pointer, because lifetime of buckets should be larger than lifetime of the container
  , children_ (bucket_traits (buckets_.get (), bucketSize_))
  , payload_ (PayloadTraits::empty_payload)
  , parent_ (0)
{
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::~trie ()
{
  payload_ = PayloadTraits::empty_payload; // necessary for smart pointers...
  children_.clear_and_dispose (trie_delete_disposer ());
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline
std::pair<typename trie<FullKey, Payload, PayloadTraits, PolicyHook>::iterator, bool>
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::insert (const FullKey &key, typename PayloadTraits::pointer_type payload)
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

	  if (trieNode->children_.size () >= trieNode->bucketSize_)
	    {
	      trieNode->bucketSize_ += trieNode->bucketIncrement_;
              buckets_array newBuckets (new bucket_type [trieNode->bucketSize_]);
	      trieNode->children_.rehash (bucket_traits (newBuckets.get (), trieNode->bucketSize_));
	      trieNode->buckets_.swap (newBuckets);
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

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline
boost::tuple<typename trie<FullKey, Payload, PayloadTraits, PolicyHook>::iterator,
	     bool,
	     typename trie<FullKey, Payload, PayloadTraits, PolicyHook>::iterator>
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::find (const FullKey &key)
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

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline typename trie<FullKey, Payload, PayloadTraits, PolicyHook>::iterator
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::find ()
{
  if (payload_ != PayloadTraits::empty_payload)
    return this;

  typedef trie<FullKey, Payload, PayloadTraits, PolicyHook> trie;
  for (typename trie::unordered_set::iterator subnode = children_.begin ();
       subnode != children_.end ();
       subnode++ )
  // BOOST_FOREACH (trie &subnode, children_)
    {
      iterator value = subnode->find ();
      if (value != 0)
	return value;
    }
  
  return 0;
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
template<class Predicate>
inline typename trie<FullKey, Payload, PayloadTraits, PolicyHook>::iterator
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::find_if (Predicate pred)
{
  if (payload_ != PayloadTraits::empty_payload && pred (payload_))
    return this;

  typedef trie<FullKey, Payload, PayloadTraits, PolicyHook> trie;
  for (typename trie::unordered_set::iterator subnode = children_.begin ();
       subnode != children_.end ();
       subnode++ )
  // BOOST_FOREACH (const trie &subnode, children_)
    {
      iterator value = subnode->find ();
      if (value != 0)
        return value;
    }
  
  return 0;
}


template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline void
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::erase ()
{
  payload_ = PayloadTraits::empty_payload;
  prune ();
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline void
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::prune ()
{
  if (payload_ == 0 && children_.size () == 0)
    {
      if (parent_ == 0) return;

      trie *parent = parent_;
      parent->children_.erase_and_dispose (*this, trie_delete_disposer ()); // delete this; basically, committing a suicide

      parent->prune ();
    }
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline std::ostream&
operator << (std::ostream &os, const trie<FullKey, Payload, PayloadTraits, PolicyHook> &trie_node)
{
  os << "# " << trie_node.key_ << ((trie_node.payload_ != 0)?"*":"") << std::endl;
  typedef trie<FullKey, Payload, PayloadTraits, PolicyHook> trie;

  for (typename trie::unordered_set::const_iterator subnode = trie_node.children_.begin ();
       subnode != trie_node.children_.end ();
       subnode++ )
  // BOOST_FOREACH (const trie &subnode, trie_node.children_)
    {
      os << "\"" << &trie_node << "\"" << " [label=\"" << trie_node.key_ << ((trie_node.payload_ != 0)?"*":"") << "\"]\n";
      os << "\"" << &(*subnode) << "\"" << " [label=\"" << subnode->key_ << ((subnode->payload_ != 0)?"*":"") << "\"]""\n";
      
      os << "\"" << &trie_node << "\"" << " -> " << "\"" << &(*subnode) << "\"" << "\n";
      os << *subnode;
    }

  return os;
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline void
trie<FullKey, Payload, PayloadTraits, PolicyHook>
::PrintStat (std::ostream &os) const
{
  os << "# " << key_ << ((payload_ != 0)?"*":"") << ": " << children_.size() << " children" << std::endl;
  for (size_t bucket = 0, maxbucket = children_.bucket_count ();
       bucket < maxbucket;
       bucket++)
    {
      os << " " << children_.bucket_size (bucket);
    }
  os << "\n";

  typedef trie<FullKey, Payload, PayloadTraits, PolicyHook> trie;
  for (typename trie::unordered_set::const_iterator subnode = children_.begin ();
       subnode != children_.end ();
       subnode++ )
  // BOOST_FOREACH (const trie &subnode, children_)
    {
      subnode->PrintStat (os);
    }
}


template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline bool
operator == (const trie<FullKey, Payload, PayloadTraits, PolicyHook> &a,
             const trie<FullKey, Payload, PayloadTraits, PolicyHook> &b)
{
  return a.key_ == b.key_;
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline std::size_t
hash_value (const trie<FullKey, Payload, PayloadTraits, PolicyHook> &trie_node)
{
  return boost::hash_value (trie_node.key_);
}

} // ndnSIM

#endif // TRIE_H_
