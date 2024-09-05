#pragma once

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
  const std::string name;
  mutable std::vector<size_t> reads;
  mutable std::vector<size_t> writes;

public:
  /**
   * @brief Construct a new Db File object with the specified file name and tuple descriptor
   * @param The name of the file to be opened or created.
   * @throws std::runtime_error if the file cannot be opened or if the `fstat` system call fails.
   * @note This method calculates the number of pages in the file by dividing the file size (in bytes)
   * by the `DEFAULT_PAGE_SIZE`.
   */
  explicit DbFile(const std::string &name);

  /**
   * @brief closes the file descriptor.
   */
  virtual ~DbFile() = default;

  const std::string &getName() const;

  const std::vector<size_t> &getReads() const;

  const std::vector<size_t> &getWrites() const;

  /**
   * @brief Read a page from the file.
   * @param page The page to read into.
   * @param id The page number of the page to be read. It determines the offset within the file.
   */
  virtual void readPage(Page &page, size_t id) const;

  /**
   * @brief Write a page to the file.
   * @param page The page to write.
   * @param id The page number of the page to which the data will be written.
   * It determines the offset in the file.
   */
  virtual void writePage(const Page &page, size_t id) const;
};
} // namespace db
