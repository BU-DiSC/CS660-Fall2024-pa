#pragma once

#include <db/DbFile.hpp>

namespace db {
class HeapPage {
  const TupleDesc &td;
  size_t capacity;
  uint8_t *header;
  uint8_t *data;

public:
  /**
   * @brief Wrap a page with a heap page.
   * @details Wrap a page with a heap page by initializing the header and data pointers.
   * @param page The page to be wrapped.
   * @param td The tuple descriptor of the page.
   * @note header and data should point to locations inside the page buffer. Do not allocate extra memory.
   * @note initialize capacity to the number of slots that can fit in the page.
   */
  HeapPage(Page &page, const TupleDesc &td);

  /**
   * @brief Get the first occupied slot of the page.
   * @return The first occupied slot of the page.
   */
  size_t begin() const;

  /**
   * @brief Get the end of the page.
   * @return capacity can be used as the end of the page.
   */
  size_t end() const;

  /**
   * @brief Insert a tuple to the page.
   * @details Insert a tuple to the page by serializing the tuple to the page.
   * @param t The tuple to be inserted.
   * @return True if the tuple is inserted successfully, false otherwise if the page is full.
   */
  bool insertTuple(const Tuple &t);

  /**
   * @brief Delete a tuple from the page.
   * @details Delete a tuple from the page by marking the slot unused.
   * @param slot The slot of the tuple to be deleted.
   */
  void deleteTuple(size_t slot);

  /**
   * @brief Check if the slot is occupied.
   * @details Check if the slot is empty by examining the header.
   * @param slot The slot to be checked.
   * @return True if the slot is empty, false otherwise.
   */
  bool empty(size_t slot) const;

  /**
   * @brief Get the tuple at the specified slot.
   * @details Get the tuple at the specified slot by deserializing the tuple from the page.
   * @param slot The slot of the tuple to be deserialized.
   * @return The tuple read from the page.
   */
  Tuple getTuple(size_t slot) const;

  /**
   * @brief Advance the slot to the next occupied slot.
   * @details Advance the slot to the next occupied slot by scanning the header.
   */
  void next(size_t &slot) const;
};
} // namespace db
