#pragma once

#include <db/BufferPool.hpp>
#include <db/DbFile.hpp>
#include <memory>

/**
 * @brief A database is a collection of files and a BufferPool.
 * @details The Database class is responsible for managing the database files.
 * It provides functions to add new database files, get the internal id of a file, and retrieve database files.
 * The class also supports removing all files from the catalog.
 * @note A Database owns the DbFile objects that are added to it.
 */
namespace db {
class Database {
  std::unordered_map<std::string, std::unique_ptr<DbFile>> files;

  BufferPool bufferPool;

  Database() = default;

public:
  friend Database &getDatabase();

  Database(Database const &) = delete;
  void operator=(Database const &) = delete;
  Database(Database &&) = delete;
  void operator=(Database &&) = delete;

  /**
   * @brief Provides access to the singleton instance of the BufferPool.
   * @return The buffer pool
   */
  BufferPool &getBufferPool();

  /**
   * @brief Adds a new file to the Database.
   * @param file The file to add.
   * @throws std::logic_error if the file name already exists.
   * @note This method takes ownership of the DbFile.
   */
  void add(std::unique_ptr<DbFile> file);

  /**
   * @brief Removes a file.
   * @param name The name of the file to remove.
   * @return The removed file.
   * @throws std::logic_error if the name does not exist.
   * @note This method should call BufferPool::flushFile(name)
   * @note This method moves the DbFile ownership to the caller.
   */
  std::unique_ptr<DbFile> remove(const std::string &name);

  /**
   * @brief Returns the DbFile of the specified id.
   * @param name The name of the file.
   * @return The DbFile object.
   * @throws std::logic_error if the name does not exist.
   */
  DbFile &get(const std::string &name) const;
};

/**
 * @brief Returns the singleton instance of the Database.
 * @return The Database object.
 */
Database &getDatabase();
} // namespace db
