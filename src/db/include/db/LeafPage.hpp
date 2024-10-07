#pragma once

#include <db/Tuple.hpp>

namespace db {

struct LeafPageHeader {
  /// The next page number
  size_t next_leaf;

  /// The number of tuples in the page
  uint16_t size;
};

struct LeafPage {
  const TupleDesc &td;

  /// The index of the key in a tuple (the key field should be of type int)
  const size_t key_index;

  uint16_t capacity;

  LeafPageHeader *header;
  uint8_t *data;

  /**
   * @brief Initialize a leaf page
   *
   * @details The provided page has a header of type LeafPageHeader, followed by a sequence of tuples.
   * The capacity of the page is calculated based on the remaining size of the page and the size of the tuples.
   *
   * @param page the page contents
   * @param td the tuple descriptor
   * @param key_index the index of the key in the tuple
   */
  LeafPage(Page &page, const TupleDesc &td, size_t key_index);

  /**
   * @brief Insert a tuple into the page
   * @details The tuple is inserted in sorted order based on the key. If the key already exists, the previous tuple is replaced.
   * @return true if the leaf is full and needs to be split.
   */
  bool insertTuple(const Tuple &t);

  /**
   * @brief Split the leaf page
   * @details The page is split into two pages. The old page contains the first half of the tuples, and the new page contains the second half.
   * @param new_page a new empty page
   * @return the split key (the first key of the new page)
   */
  int split(LeafPage &new_page);

  /**
   * @brief Get a tuple from the database file.
   * @details Get a tuple from the database file by reading the tuple from the page.
   * @param it The iterator that identifies the tuple to be read.
   * @return The tuple read from the page.
   */
  Tuple getTuple(size_t slot) const;
};

} // namespace db
