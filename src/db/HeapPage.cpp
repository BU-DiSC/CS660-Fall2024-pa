#include <db/Database.hpp>
#include <db/HeapPage.hpp>
#include <stdexcept>

using namespace db;

HeapPage::HeapPage(Page &page, const TupleDesc &td) : td(td) {
  // TODO pa2: initialize private members
  // NOTE: header and data should point to locations inside the page buffer. Do not allocate extra memory.
}

size_t HeapPage::begin() const {
  // TODO pa2: implement
  throw std::runtime_error("not implemented");
}

size_t HeapPage::end() const {
  // TODO pa2: implement
}

bool HeapPage::insertTuple(const Tuple &t) {
  // TODO pa2: implement
}

void HeapPage::deleteTuple(size_t slot) {
  // TODO pa2: implement
}

Tuple HeapPage::getTuple(size_t slot) const {
  // TODO pa2: implement
}

void HeapPage::next(size_t &slot) const {
  // TODO pa2: implement
}

bool HeapPage::empty(size_t slot) const {
  // TODO pa2: implement
}
