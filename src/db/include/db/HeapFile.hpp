#pragma once

#include <db/DbFile.hpp>

namespace db {
class HeapFile : public DbFile {
public:
  HeapFile(const std::string &name, const TupleDesc &td);

  /**
   * @brief Insert a tuple to the database file.
   * @details Insert a tuple to the first available slot of the last page. If the last page is full, create a new page.
   * @param t The tuple to be inserted.
   */
  void insertTuple(const Tuple &t) override;

  /**
   * @brief Delete a tuple from the database file.
   * @details Delete a tuple from the database file by marking the slot unused.
   * @param it The iterator that identifies the tuple to be deleted.
   */
  void deleteTuple(const Iterator &it) override;

  /**
   * @brief Get a tuple from the database file.
   * @details Get a tuple from the database file by reading the tuple from the page.
   * @param it The iterator that identifies the tuple to be read.
   * @return The tuple read from the page.
   */
  Tuple getTuple(const Iterator &it) const override;

  /**
   * @brief Advance the iterator to the next tuple.
   * @details Advance the iterator to the next tuple by moving to the next slot of the page.
   * @param it The iterator to be advanced.
   * @note The next tuple may be on a subsequent page (pages might be empty).
   */
  void next(Iterator &it) const override;

  /**
   * @brief Get the iterator to the first tuple.
   * @details Get the iterator to the first tuple by finding the first occupied slot.
   * @return The iterator to the first tuple.
   * @note The first tuple may not be on the first page.
   */
  Iterator begin() const override;

  /**
   * @brief Get the iterator to the end of the file.
   * @details Return a sentinel value indicating there are no more tuples.
   * @return The iterator to the end of the file.
   */
  Iterator end() const override;
};
} // namespace db
