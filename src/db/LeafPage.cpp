#include <db/LeafPage.hpp>
#include <stdexcept>

using namespace db;

LeafPage::LeafPage(Page &page, const TupleDesc &td, size_t key_index) : td(td), key_index(key_index) {
  // TODO pa2: implement
}

bool LeafPage::insertTuple(const Tuple &t) {
  // TODO pa2: implement
}

int LeafPage::split(LeafPage &new_page) {
  // TODO pa2: implement
}

Tuple LeafPage::getTuple(size_t slot) const {
  // TODO pa2: implement
}
