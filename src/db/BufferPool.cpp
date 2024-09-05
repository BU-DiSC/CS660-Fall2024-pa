#include <db/BufferPool.hpp>
#include <db/Database.hpp>
#include <numeric>

using namespace db;

BufferPool::BufferPool()
// TODO pa1: add initializations if needed
{
  // TODO pa1: additional initialization if needed
}

BufferPool::~BufferPool() {
  // TODO pa1: flush any remaining dirty pages
}

Page &BufferPool::getPage(const PageId &pid) {
  // TODO pa1: If already in buffer pool, make it the most recent page and return it

  // TODO pa1: If there are no available pages, evict the least recently used page. If it is dirty, flush it to disk

  // TODO pa1: Read the page from disk to one of the available slots, make it the most recent page
}

void BufferPool::markDirty(const PageId &pid) {
  // TODO pa1: Mark the page as dirty. Note that the page must already be in the buffer pool
}

bool BufferPool::isDirty(const PageId &pid) const {
  // TODO pa1: Return whether the page is dirty. Note that the page must already be in the buffer pool
}

bool BufferPool::contains(const PageId &pid) const {
  // TODO pa1: Return whether the page is in the buffer pool
}

void BufferPool::discardPage(const PageId &pid) {
  // TODO pa1: Discard the page from the buffer pool. Note that the page must already be in the buffer pool
}

void BufferPool::flushPage(const PageId &pid) {
  // TODO pa1: Flush the page to disk. Note that the page must already be in the buffer pool
}

void BufferPool::flushFile(const std::string &file) {
  // TODO pa1: Flush all pages of the file to disk
}
