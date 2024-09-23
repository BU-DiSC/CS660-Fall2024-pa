#include <db/Database.hpp>
#include <db/HeapFile.hpp>
#include <db/HeapPage.hpp>
#include <stdexcept>

using namespace db;

HeapFile::HeapFile(const std::string &name, const TupleDesc &td) : DbFile(name, td) {}

void HeapFile::insertTuple(const Tuple &t) {
  // TODO pa2: implement
}

void HeapFile::deleteTuple(const Iterator &it) {
  // TODO pa2: implement
}

Tuple HeapFile::getTuple(const Iterator &it) const {
  // TODO pa2: implement
}

void HeapFile::next(Iterator &it) const {
  // TODO pa2: implement
}

Iterator HeapFile::begin() const {
  // TODO pa2: implement
}

Iterator HeapFile::end() const {
  // TODO pa2: implement
}
