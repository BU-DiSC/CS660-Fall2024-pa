#pragma once

#include <db/types.hpp>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace db {
constexpr size_t DEFAULT_NUM_PAGES = 50;
/**
 * @brief Represents a buffer pool for database pages.
 * @details The BufferPool class is responsible for managing the database pages in memory.
 * It provides functions to get a page, mark a page as dirty, and check the status of pages.
 * The class also supports flushing pages to disk and discarding pages from the buffer pool.
 * @note A BufferPool owns the Page objects that are stored in it.
 */
class BufferPool {
  std::array<Page, DEFAULT_NUM_PAGES> pages;
  std::array<PageId, DEFAULT_NUM_PAGES> pos_to_pid;
  std::unordered_map<const PageId, size_t> pid_to_pos;
  std::unordered_set<size_t> dirty;
  std::vector<size_t> available;
  std::list<size_t> lru_list;
  std::unordered_map<size_t, std::list<size_t>::iterator> pos_to_lru;

public:
  /**
   * @brief: Constructs a BufferPool object with the default number of pages.
   */
  explicit BufferPool();

  /**
   * @brief: Destructs a BufferPool object after flushing all dirty pages to disk.
   */
  ~BufferPool();

  BufferPool(const BufferPool &) = delete;

  BufferPool(BufferPool &&) = delete;

  BufferPool &operator=(const BufferPool &) = delete;

  BufferPool &operator=(BufferPool &&) = delete;

  /**
   * @brief: Returns the page with the specified page id.
   * @param pid: The page id of the page to return.
   * @return: The page with the specified page id.
   * @note This method should make this page the most recently used page.
   */
  Page &getPage(const PageId &pid);

  /**
   * @brief: Marks the page with the specified page id as dirty.
   * @param pid: The page id of the page to mark as dirty.
   */
  void markDirty(const PageId &pid);

  /**
   * @brief: Returns whether the page with the specified page id is dirty.
   * @param pid: The page id of the page to check.
   * @return: True if the page is dirty, false otherwise.
   */
  bool isDirty(const PageId &pid) const;

  /**
   * @brief: Returns whether the buffer pool contains the page with the specified page id.
   * @param pid: The page id of the page to check.
   * @return: True if the buffer pool contains the page, false otherwise.
   */
  bool contains(const PageId &pid) const;

  /**
   * @brief: Discards the page with the specified page id from the buffer pool.
   * @param pid: The page id of the page to discard.
   * @note This method does NOT flush the page to disk.
   * @note This method also updates the LRU and dirty pages to exclude tracking this page.
   */
  void discardPage(const PageId &pid);

  /**
   * @brief: Flushes the page with the specified page id to disk.
   * @param pid: The page id of the page to flush.
   * @note This method should remove the page from dirty pages.
   */
  void flushPage(const PageId &pid);
  /**
   * @brief: Flushes all dirty pages in the specified file to disk.
   * @param file: The name of the associated file.
   * @note This method should call BufferPool::flushPage(pid).
   */
  void flushFile(const std::string &file);
};
} // namespace db
