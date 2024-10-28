#include <db/IndexPage.hpp>
#include <stdexcept>

using namespace db;

IndexPage::IndexPage(Page &page) {
  capacity = (DEFAULT_PAGE_SIZE - sizeof(IndexPageHeader) - sizeof(int)) / (sizeof(int) + sizeof(size_t));
  header = reinterpret_cast<IndexPageHeader *>(page.data());
  keys = reinterpret_cast<int *>(header + 1);
  children = reinterpret_cast<size_t *>(keys + capacity + 1);
}

bool IndexPage::insert(int key, size_t child) {
  auto it = std::lower_bound(keys, keys + header->size, key);
  auto slot = it - keys;
  std::move_backward(keys + slot, keys + header->size, keys + header->size + 1);
  std::move_backward(children + slot + 1, children + header->size + 1, children + header->size + 2);
  keys[slot] = key;
  children[slot + 1] = child;
  ++header->size;
  return header->size == capacity;
}

int IndexPage::split(IndexPage &new_page) {
  size_t half = header->size / 2;
  new_page.header->size = header->size - half - 1;
  new_page.header->index_children = header->index_children;
  std::copy(keys + half + 1, keys + header->size, new_page.keys);
  std::copy(children + half + 1, children + header->size + 1, new_page.children);
  header->size = half;
  return keys[half];
}
