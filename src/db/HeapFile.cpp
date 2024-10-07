#include <db/Database.hpp>
#include <db/HeapFile.hpp>
#include <db/HeapPage.hpp>
#include <stdexcept>

using namespace db;

HeapFile::HeapFile(const std::string &name, const TupleDesc &td) : DbFile(name, td) {}

void HeapFile::insertTuple(const Tuple &t) {
  if (!td.compatible(t)) {
    throw std::runtime_error("Tuple not compatible with TupleDesc");
  }
  BufferPool &bufferPool = getDatabase().getBufferPool();
  PageId pid{name, 0};
  pid.page = numPages - 1;
  Page &p = bufferPool.getPage(pid);
  HeapPage hp(p, td);
  if (!hp.insertTuple(t)) {
    numPages++;
    pid.page++;
    Page &np = bufferPool.getPage(pid);
    HeapPage nhp(np, td);
    nhp.insertTuple(t);
  }
  bufferPool.markDirty(pid);
}

void HeapFile::deleteTuple(const Iterator &it) {
  BufferPool &bufferPool = getDatabase().getBufferPool();
  PageId pid{name, it.page};
  Page &p = bufferPool.getPage(pid);
  HeapPage hp(p, td);
  bufferPool.markDirty(pid);
  hp.deleteTuple(it.slot);
}

Tuple HeapFile::getTuple(const Iterator &it) const {
  BufferPool &bufferPool = getDatabase().getBufferPool();
  PageId pid{name, it.page};
  Page &p = bufferPool.getPage(pid);
  HeapPage hp(p, td);
  return hp.getTuple(it.slot);
}

void HeapFile::next(Iterator &it) const {
  BufferPool &bufferPool = getDatabase().getBufferPool();
  if (it.page < numPages) {
    PageId pid{name, it.page};
    Page &p = bufferPool.getPage(pid);
    const HeapPage hp(p, td);
    hp.next(it.slot);
    if (it.slot != hp.end()) {
      return;
    }
    it.page++;
  }
  while (it.page < numPages) {
    PageId pid{name, it.page};
    Page &p = bufferPool.getPage(pid);
    const HeapPage hp(p, td);
    it.slot = hp.begin();
    if (it.slot != hp.end()) {
      return;
    }
    it.page++;
  }
  it.slot = 0;
}

Iterator HeapFile::begin() const {
  BufferPool &bufferPool = getDatabase().getBufferPool();
  size_t page = 0;
  while (page < numPages) {
    PageId pid{name, page};
    Page &p = bufferPool.getPage(pid);
    const HeapPage hp(p, td);
    size_t slot = hp.begin();
    if (slot != hp.end())
      return {*this, page, slot};
    page++;
  }
  return {*this, numPages, 0};
}

Iterator HeapFile::end() const { return {*this, numPages, 0}; }
