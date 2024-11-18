#include <db/LeafPage.hpp>
#include <stdexcept>

using namespace db;

LeafPage::LeafPage(Page &page, const TupleDesc &td, size_t key_index) : td(td), key_index(key_index) {
  header = reinterpret_cast<LeafPageHeader *>(page.data());
  capacity = (DEFAULT_PAGE_SIZE - sizeof(LeafPageHeader)) / td.length();
  data = page.data() + DEFAULT_PAGE_SIZE - td.length() * capacity;
}

bool LeafPage::insertTuple(const Tuple &t) {
  return false;
}

int LeafPage::split(LeafPage &new_page) {
  size_t half = header->size / 2;
  new_page.header->size = header->size - half;
  new_page.header->next_leaf = header->next_leaf;
  std::copy(data + half * td.length(), data + header->size * td.length(), new_page.data);
  header->size = half;
  return std::get<int>(td.deserialize(new_page.data).get_field(key_index));
}

Tuple LeafPage::getTuple(size_t slot) const {
  if (slot >= header->size) {
    throw std::out_of_range("slot out of range");
  }
  return td.deserialize(data + slot * td.length());
}
