#pragma once

#include <db/DbFile.hpp>

namespace db {

class BTreeFile : public DbFile {
  static constexpr size_t root_id = 0;
  size_t key_index;

public:

  /**
   * @brief Initialize a BTreeFile
   *
   * @param key_index the index of the key in the tuple
   */
  BTreeFile(const std::string &name, const TupleDesc &td, size_t key_index);

  /**
   * @brief Insert a tuple into the file
   * @details Insert a tuple into the file. Traverse the BTree from the root to find the leaf node to insert the tuple.
   * If the leaf node is full, split the node and insert the new key and child to the parent node. This process is repeated
   * until no more split is needed. If the root node is split, create a create two new nodes with the contents of the root
   * and set the root to be the parent of the two new nodes.
   * @param t the tuple to insert
   */
  void insertTuple(const Tuple &t) override;

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
   * If the iterator is at the end of the page, move to the next page.
   * @param it The iterator to be advanced.
   */
  void next(Iterator &it) const override;

  /**
   * @brief Get the iterator to the first tuple of the leftmost leaf (head).
   * @details Traverse the tree to reach the head leaf and return the first tuple.
   * @return The iterator to the first tuple.
   */
  Iterator begin() const override;

  /**
   * @brief Get the iterator to the end of the file.
   * @details Return an iterator that points to the end of the file.
   * @return The iterator to the end of the file.
   */
  Iterator end() const override;
};
} // namespace db
