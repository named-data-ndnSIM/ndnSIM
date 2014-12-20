/**
 * \mainpage ndnSIM documentation
 *
 * Please refer to <a href="../index.html">ndnSIM documentation page</a>
 */

// explicit instantiation and registering

/**
 * @brief ContentStore with LRU cache replacement policy
 */
template class ContentStoreImpl<lru_policy_traits>;
/**
 * @brief ContentStore with random cache replacement policy
 */

template class ContentStoreImpl<random_policy_traits>;

/**
 * @brief ContentStore with FIFO cache replacement policy
 */
template class ContentStoreImpl<fifo_policy_traits>;

