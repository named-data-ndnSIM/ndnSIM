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
#include <boost/intrusive/list.hpp>
#include <boost/functional/hash.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/tuple/tuple.hpp>

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
template<typename FullKey,
         typename Payload,
         typename PayloadTraits = pointer_payload_traits<Payload>,
         typename PolicyHook = bi::list_member_hook<> >
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
  
  // actual entry
  friend bool
  operator== <> (const trie<FullKey, Payload, PayloadTraits, PolicyHook> &a,
                 const trie<FullKey, Payload, PayloadTraits, PolicyHook> &b);

  friend std::size_t
  hash_value <> (const trie<FullKey, Payload, PayloadTraits, PolicyHook> &trie_node);

  inline std::pair<iterator, bool>
  insert (const FullKey &key,
          typename PayloadTraits::const_pointer_type payload);

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

  typename PayloadTraits::const_pointer_type
  payload () const
  {
    return payload_;
  }

  void
  set_payload (typename PayloadTraits::const_pointer_type payload)
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
  bi::unordered_set_member_hook<> unordered_set_member_hook_;

  // necessary typedefs
  typedef trie self_type;
  typedef bi::member_hook< trie,
  			   bi::unordered_set_member_hook< >, &trie::unordered_set_member_hook_ > member_hook;
  typedef bi::unordered_set< trie, member_hook >                                                 unordered_set;
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
  
  typename PayloadTraits::const_pointer_type payload_;
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
::insert (const FullKey &key, typename PayloadTraits::const_pointer_type payload)
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

  typedef trie<FullKey, Payload, PayloadTraits> trie;
  BOOST_FOREACH (const trie &subnode, children_)
    {
      iterator value = subnode.find ();
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
  BOOST_FOREACH (const trie &subnode, children_)
    {
      iterator value = subnode.find ();
      if (value != 0)
	return value;
    }
  
  return 0;
}


template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline void
trie<FullKey, Payload, PayloadTraits, PolicyHook>::erase ()
{
  payload_ = PayloadTraits::empty_payload;
  prune ();
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline void
trie<FullKey, Payload, PayloadTraits, PolicyHook>::prune ()
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
  BOOST_FOREACH (const trie &subnode, trie_node.children_)
    {
      os << "\"" << &trie_node << "\"" << " [label=\"" << trie_node.key_ << ((trie_node.payload_ != 0)?"*":"") << "\"]\n";
      os << "\"" << &subnode << "\"" << " [label=\"" << subnode.key_ << ((subnode.payload_ != 0)?"*":"") << "\"]""\n";
      
      os << "\"" << &trie_node << "\"" << " -> " << "\"" << &subnode << "\"" << "\n";
      os << subnode;
    }

  return os;
}

template<typename FullKey, typename Payload, typename PayloadTraits, typename PolicyHook>
inline void
trie<FullKey, Payload, PayloadTraits, PolicyHook>::PrintStat (std::ostream &os) const
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
  BOOST_FOREACH (const trie &subnode, children_)
    {
      subnode.PrintStat (os);
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



template<typename FullKey,
	 typename Payload, typename PayloadTraits
         >
struct lru_policy_traits
{
  typedef trie< FullKey, Payload, PayloadTraits, bi::list_member_hook<> > parent_trie;
  typedef typename bi::list< parent_trie,
                             bi::member_hook< parent_trie,
                                              bi::list_member_hook<>,
                                              &parent_trie::policy_hook_ > > policy_container;

  class policy : public policy_container
  {
  public:
    policy ()
      : max_size_ (100)
    {
    }

    inline void
    update (typename parent_trie::iterator item)
    {
      // do relocation
      policy_container::splice (policy_container::end (),
                                *this,
                                policy_container::s_iterator_to (*item));
    }
  
    inline void
    insert (typename parent_trie::iterator item)
    {
      if (policy_container::size () >= max_size_)
        {
          typename parent_trie::iterator oldItem = &(*policy_container::begin ());
          policy_container::pop_front ();
          oldItem->erase ();
        }
      
      policy_container::push_back (*item);
    }
  
    inline void
    lookup (typename parent_trie::iterator item)
    {
      // do relocation
      policy_container::splice (policy_container::end (),
                                *this,
                                policy_container::s_iterator_to (*item));
    }
  
    inline void
    erase (typename parent_trie::iterator item)
    {
      policy_container::erase (policy_container::s_iterator_to (*item));
    }

    inline void
    set_max_size (size_t max_size)
    {
      max_size_ = max_size;
    }

  private:
    size_t max_size_;
  };
};



template<typename FullKey,
	 typename Payload, typename PayloadTraits = pointer_payload_traits<Payload>,
         typename policy_traits = lru_policy_traits<FullKey, Payload, PayloadTraits>
         >
class indexed_trie 
{
public:
  typedef trie< FullKey, Payload, PayloadTraits > parent_trie;
  typedef typename parent_trie::iterator          iterator;

  inline
  indexed_trie (size_t bucketSize = 10, size_t bucketIncrement = 10)
    : trie_ ("", bucketSize, bucketIncrement)
  {
  }

  inline std::pair< iterator, bool >
  insert (const FullKey &key, typename PayloadTraits::const_pointer_type payload)
  {
    std::pair<iterator, bool> item =
      trie_.insert (key, payload);

    if (item.second) // real insert
      {
        policy_.insert (s_iterator_to (item.first));
      }
    else
      {
        item.first->set_payload (payload);
        // policy_traits::update (*item.first);
      }
    
    return item;
  }

  inline void
  erase (iterator node)
  {
    if (node == end ()) return;

    policy_.erase (s_iterator_to (node));
    node->erase (); // will do cleanup here
  }

  /**
   * @brief Perform the longest prefix match
   * @param key the key for which to perform the longest prefix match
   *
   * For subsequent direct searches, policy_.lookup () should be called manually
   *
   * @return ->second is true if prefix in ->first is longer than key
   *         ->third is always last node searched
   */
  inline boost::tuple< iterator, bool, iterator >
  find (const FullKey &key)
  {
    boost::tuple< iterator, bool, iterator > item = trie_.find (key);
    if (item.template get<0> () != trie_.end ())
      {
        policy_.lookup (s_iterator_to (item.template get<0> ()));
      }
    return boost::make_tuple (s_iterator_to (item.template get<0> ()),
                              item.template get<1> (),
                              s_iterator_to (item.template get<2> ()));
  }

  iterator end ()
  {
    return 0;
  }

  const parent_trie &
  getTrie () const { return trie_; }

  const typename policy_traits::policy &
  getPolicy () const { return policy_; }

  typename policy_traits::policy &
  getPolicy () { return policy_; }

  static inline iterator
  s_iterator_to (typename parent_trie::iterator item)
  {
    if (item == 0)
      return 0;
    else
      return &(*item);
  }
  
private:
  parent_trie      trie_;
  typename policy_traits::policy policy_;
};

