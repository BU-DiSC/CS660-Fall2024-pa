#include <cstring>
#include <db/BTreeFile.hpp>
#include <db/Database.hpp>
#include <db/IndexPage.hpp>
#include <db/LeafPage.hpp>
#include <stdexcept>

using namespace db;

BTreeFile::BTreeFile(const std::string &name, const TupleDesc &td, size_t key_index)
    : DbFile(name, td), key_index(key_index) {}

void BTreeFile::insertTuple(const Tuple &t) {
  std::vector<size_t> path;
  BufferPool &bufferPool = getDatabase().getBufferPool();
  PageId pid{name, root_id};

  Page &root_page = bufferPool.getPage(pid);
  IndexPage root(root_page);
  if (root.header->size == 0 && root.children[0] != 1) {
    bufferPool.markDirty({name, root_id});
    pid.page = numPages++;
    root.children[0] = pid.page;
  } else {
    while (true) {
      Page &page = bufferPool.getPage(pid);
      IndexPage node(page);
      auto pos = std::lower_bound(node.keys, node.keys + node.header->size, std::get<int>(t.get_field(key_index)));
      auto slot = pos - node.keys;
      pid.page = node.children[slot];
      if (!node.header->index_children) {
        break;
      }
      path.push_back(pid.page);
    }
  }

  Page &page = bufferPool.getPage(pid);
  bufferPool.markDirty(pid);
  LeafPage leaf(page, td, key_index);
  if (!leaf.insertTuple(t)) {
    return;
  }

  pid.page = numPages++;
  Page &new_leaf_page = bufferPool.getPage(pid);
  bufferPool.markDirty(pid);
  LeafPage new_leaf(new_leaf_page, td, key_index);
  int new_key = leaf.split(new_leaf);
  leaf.header->next_leaf = pid.page;
  size_t new_child = pid.page;

  while (!path.empty()) {
    size_t parent_id = path.back();
    path.pop_back();
    pid.page = parent_id;
    Page &parent_page = bufferPool.getPage(pid);
    bufferPool.markDirty(pid);
    IndexPage parent(parent_page);
    if (!parent.insert(new_key, new_child)) {
      return;
    }

    pid.page = numPages++;
    Page &new_internal_page = bufferPool.getPage(pid);
    bufferPool.markDirty(pid);
    IndexPage new_internal(new_internal_page);
    new_key = parent.split(new_internal);
    new_child = pid.page;
  }

  bufferPool.markDirty({name, root_id});
  if (!root.insert(new_key, new_child)) {
    return;
  }
  pid.page = numPages++;
  Page &new_child1 = bufferPool.getPage(pid);
  bufferPool.markDirty(pid);
  size_t child1 = pid.page;
  new_child1 = root_page;
  IndexPage child1_page(new_child1);

  pid.page = numPages++;
  Page &new_child2 = bufferPool.getPage(pid);
  bufferPool.markDirty(pid);
  size_t child2 = pid.page;
  IndexPage child2_page(new_child2);

  int key = child1_page.split(child2_page);
  root.header->size = 1;
  root.header->index_children = true;
  root.keys[0] = key;
  root.children[0] = child1;
  root.children[1] = child2;
}

void BTreeFile::deleteTuple(const Iterator &it) {
}

Tuple BTreeFile::getTuple(const Iterator &it) const {
  BufferPool &bufferPool = getDatabase().getBufferPool();
  PageId pid{name, it.page};
  Page &page = bufferPool.getPage(pid);
  LeafPage leaf(page, td, key_index);
  return leaf.getTuple(it.slot);
}

void BTreeFile::next(Iterator &it) const {
  BufferPool &bufferPool = getDatabase().getBufferPool();
  PageId pid{name, it.page};
  Page &page = bufferPool.getPage(pid);
  LeafPage leaf(page, td, key_index);
  if (it.slot + 1 < leaf.header->size) {
    it.slot++;
  } else {
    it.page = leaf.header->next_leaf;
    it.slot = 0;
  }
}

Iterator BTreeFile::begin() const {
  BufferPool &bufferPool = getDatabase().getBufferPool();
  PageId pid{name, root_id};
  while (true) {
    Page &page = bufferPool.getPage(pid);
    IndexPage node(page);
    pid.page = node.children[0];
    if (!node.header->index_children) {
      break;
    }
  }
  return {*this, pid.page, 0};
}

Iterator BTreeFile::end() const {
  return {*this, 0, 0};
}
