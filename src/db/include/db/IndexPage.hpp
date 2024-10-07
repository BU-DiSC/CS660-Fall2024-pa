#pragma once

#include <db/Tuple.hpp>

namespace db {

struct IndexPageHeader {
  /// Number of keys in the page
  uint16_t size;

  /// Whether the next level is internal or leaf
  bool index_children;
};

struct IndexPage {
  uint16_t capacity;

  IndexPageHeader *header;
  int *keys;
  size_t *children;

  /**
   * @brief Initialize a leaf page
   *
   * @details The provided page has a header of type IndexPageHeader, followed by `IndexPageHeader::size` keys and
   * `IndexPageHeader::size + 1` page numbers. The keys are sorted in ascending order.
   * The capacity of the page is calculated based on the remaining size of the page.
   *
   * @param page the page contents
   * @param key_index the index of the key in the tuple
   */
  explicit IndexPage(Page &page);

  /**
   * @brief Insert a new key with a corresponding child page number
   * @param key the key to insert
   * @param child the child page number
   * @return true if the page is full and needs to be split
   */
  bool insert(int key, size_t child);

  /**
   * @brief Split the index page
   * @details The page is split into two pages. The old page contains the first half of the tuples, and the new page contains the second half.
   * @param new_page a new empty page
   * @return the split key (this key is moved to the parent page)
   */
  int split(IndexPage &new_page);
};

} // namespace db
