#include <db/BufferPool.hpp>
#include <db/Database.hpp>
#include <numeric>

using namespace db;

BufferPool::BufferPool() : available(DEFAULT_NUM_PAGES) { std::iota(available.rbegin(), available.rend(), 0); }

BufferPool::~BufferPool() {
  for (const size_t &pos : dirty) {
    const Page &page = pages[pos];
    const PageId &pid = pos_to_pid[pos];
    getDatabase().get(pid.file).writePage(page, pid.page);
  }
}

Page &BufferPool::getPage(const PageId &pid) {
  // If already in buffer pool, make it the most recent page and return it
  if (contains(pid)) {
    size_t pos = pid_to_pos.at(pid);
    lru_list.splice(lru_list.begin(), lru_list, pos_to_lru[pos]);
    pos_to_lru[pos] = lru_list.begin();
    return pages[pos];
  }

  // If there are no available pages, evict the least recently used page. If the page is dirty, flush it to disk
  if (available.empty()) {
    size_t pos = lru_list.back();
    const PageId &old_pid = pos_to_pid.at(pos);
    if (isDirty(old_pid)) {
      flushPage(old_pid);
    }
    discardPage(old_pid);
  }

  // Read the page from disk to one of the available slots, make it the most recent page
  size_t pos = available.back();
  available.pop_back();

  Page &page = pages[pos];
  getDatabase().get(pid.file).readPage(page, pid.page);
  pid_to_pos[pid] = pos;
  pos_to_pid[pos] = pid;

  lru_list.push_front(pos);
  pos_to_lru[pos] = lru_list.begin();

  return page;
}

void BufferPool::markDirty(const PageId &pid) {
  size_t pos = pid_to_pos.at(pid);
  dirty.insert(pos);
}

bool BufferPool::isDirty(const PageId &pid) const {
  size_t pos = pid_to_pos.at(pid);
  return dirty.contains(pos);
}

bool BufferPool::contains(const PageId &pid) const { return pid_to_pos.contains(pid); }

void BufferPool::discardPage(const PageId &pid) {
  size_t pos = pid_to_pos.at(pid);
  pid_to_pos.erase(pid);
  pos_to_pid[pos] = {};

  lru_list.erase(pos_to_lru[pos]);
  pos_to_lru.erase(pos);
  dirty.erase(pos);
  available.push_back(pos);
}

void BufferPool::flushPage(const PageId &pid) {
  size_t pos = pid_to_pos.at(pid);
  if (dirty.erase(pos) == 0)
    return;
  const Page &page = pages[pos];
  getDatabase().get(pid.file).writePage(page, pid.page);
}

void BufferPool::flushFile(const std::string &file) {
  std::vector<size_t> to_flush;
  for (const size_t &pos : dirty) {
    const PageId &pid = pos_to_pid[pos];
    if (pid.file == file) {
      to_flush.emplace_back(pid.page);
    }
  }
  for (const auto &page : to_flush) {
    flushPage({file, page});
  }
}
