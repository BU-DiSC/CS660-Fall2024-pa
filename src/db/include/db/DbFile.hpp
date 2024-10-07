#pragma once

#include <db/Iterator.hpp>
#include <db/types.hpp>
#include <vector>

namespace db {

/**
 * @brief Represents a database file.
 * @details It provides functions to read and write pages to the file, as well as to insert and delete tuples.
 * The class also provides functions to iterate over the tuples in the file.
 * @note A `DbFile` object owns the `TupleDesc` object that describes the schema of the tuples in the file.
 */
class DbFile {
  mutable std::vector<size_t> reads;
  mutable std::vector<size_t> writes;

  int fd;

protected:
  const std::string name;
  const TupleDesc td;
  size_t numPages;

public:
  /**
   * @brief Construct a new Db File object with the specified file name and tuple descriptor
   * @param name of the file to be opened or created.
   * @param td tuple description of tuples in the file.
   * @throws std::runtime_error if the file cannot be opened or if the `fstat` system call fails.
   * @note This method calculates the number of pages in the file by dividing the file size (in bytes)
   * by the `DEFAULT_PAGE_SIZE`.
   */
  explicit DbFile(const std::string &name, const TupleDesc &td);

  /**
   * @brief closes the file descriptor.
   */
  virtual ~DbFile();

  const std::string &getName() const;

  const std::vector<size_t> &getReads() const;

  const std::vector<size_t> &getWrites() const;

  /**
   * @brief Read a page from the file.
   * @param page The page to read into.
   * @param id The page number of the page to be read. It determines the offset within the file.
   */
  void readPage(Page &page, size_t id) const;

  /**
   * @brief Write a page to the file.
   * @param page The page to write.
   * @param id The page number of the page to which the data will be written.
   * It determines the offset in the file.
   */
  void writePage(const Page &page, size_t id) const;

  virtual void insertTuple(const Tuple &t);

  virtual void deleteTuple(const Iterator &it);

  virtual Tuple getTuple(const Iterator &it) const;

  virtual void next(Iterator &it) const;

  virtual Iterator begin() const;

  virtual Iterator end() const;

  size_t getNumPages() const;

  const TupleDesc &getTupleDesc() const;
};
} // namespace db
