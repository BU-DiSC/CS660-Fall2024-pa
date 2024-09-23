#include <gtest/gtest.h>

#include <db/Database.hpp>
#include <db/DbFile.hpp>

TEST(BufferPoolTest, getPage) {
  db::Database &db = db::getDatabase();
  db::BufferPool &bufferPool = db.getBufferPool();

  std::string name{"file"};
  db::TupleDesc td;
  db.add(std::make_unique<db::DbFile>(name, td));
  std::array<db::Page *, db::DEFAULT_NUM_PAGES> pages{};
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    pages[i] = &bufferPool.getPage({name, i});
  }
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    EXPECT_EQ(pages[i], &bufferPool.getPage({name, i}));
  }

  const db::DbFile &file = db.get(name);
  const auto &reads = file.getReads();
  const auto &writes = file.getWrites();
  EXPECT_EQ(reads.size(), db::DEFAULT_NUM_PAGES);
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    EXPECT_EQ(reads[i], i);
  }
  EXPECT_EQ(writes.size(), 0);
}

TEST(BufferPoolTest, getPageMultipleFiles) {
  db::Database &db = db::getDatabase();
  db::BufferPool &bufferPool = db.getBufferPool();
  db::TupleDesc td;

  std::array<db::DbFile *, db::DEFAULT_NUM_PAGES> files{};
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    auto file = std::make_unique<db::DbFile>(std::to_string(i), td);
    files[i] = file.get();
    db.add(std::move(file));
  }
  std::array<db::Page *, db::DEFAULT_NUM_PAGES> pages{};
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    pages[i] = &bufferPool.getPage({std::to_string(i), 0});
  }
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    EXPECT_EQ(pages[i], &bufferPool.getPage({std::to_string(i), 0}));
  }
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    EXPECT_EQ(pages[i], &bufferPool.getPage({std::to_string(i), 0}));
  }
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    const auto &reads = files[i]->getReads();
    const auto &writes = files[i]->getWrites();
    EXPECT_EQ(reads.size(), 1);
    EXPECT_EQ(reads[0], 0);
    EXPECT_EQ(writes.size(), 0);
  }
}

TEST(BufferPoolTest, eviction) {
  db::Database &db = db::getDatabase();
  db::BufferPool &bufferPool = db.getBufferPool();

  std::string name{"file"};
  db::TupleDesc td;
  db.add(std::make_unique<db::DbFile>(name, td));
  std::array<db::Page *, db::DEFAULT_NUM_PAGES> pages{};
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    pages[i] = &bufferPool.getPage({name, i});
  }
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    EXPECT_EQ(pages[i], &bufferPool.getPage({name, i}));
  }
  db::Page &page = bufferPool.getPage({name, db::DEFAULT_NUM_PAGES});
  auto it = std::find(pages.begin(), pages.end(), &page);
  EXPECT_NE(it, pages.end());
  size_t index = std::distance(pages.begin(), it);
  EXPECT_FALSE(bufferPool.contains({name, index}));

  const db::DbFile &file = db.get(name);
  const auto &reads = file.getReads();
  const auto &writes = file.getWrites();
  EXPECT_EQ(reads.size(), db::DEFAULT_NUM_PAGES + 1);
  for (size_t i = 0; i <= db::DEFAULT_NUM_PAGES; i++) {
    EXPECT_EQ(reads[i], i);
  }
  EXPECT_EQ(writes.size(), 0);
}

TEST(BufferPoolTest, markDirty) {
  db::Database &db = db::getDatabase();
  db::BufferPool &bufferPool = db.getBufferPool();

  std::string name{"file"};
  db::TupleDesc td;
  db.add(std::make_unique<db::DbFile>(name, td));
  std::array<db::Page *, db::DEFAULT_NUM_PAGES> pages{};
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    pages[i] = &bufferPool.getPage({name, i});
    if (i % 2 == 0) {
      bufferPool.markDirty({name, i});
    }
  }
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    pages[i] = &bufferPool.getPage({name, i});
    if (i % 2 == 0) {
      EXPECT_TRUE(bufferPool.isDirty({name, i}));
    } else {
      EXPECT_FALSE(bufferPool.isDirty({name, i}));
    }
  }

  const db::DbFile &file = db.get(name);
  const auto &reads = file.getReads();
  const auto &writes = file.getWrites();
  EXPECT_EQ(reads.size(), db::DEFAULT_NUM_PAGES);
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    EXPECT_EQ(reads[i], i);
  }
  EXPECT_EQ(writes.size(), 0);
}

TEST(BufferPoolTest, flushPage) {
  db::Database &db = db::getDatabase();
  db::BufferPool &bufferPool = db.getBufferPool();

  std::string name{"file"};
  db::TupleDesc td;
  db.add(std::make_unique<db::DbFile>(name, td));
  db::PageId pid{name, 0};
  bufferPool.getPage(pid);
  bufferPool.markDirty(pid);
  EXPECT_TRUE(bufferPool.contains(pid));
  EXPECT_TRUE(bufferPool.isDirty(pid));
  bufferPool.flushPage(pid);
  EXPECT_TRUE(bufferPool.contains(pid));
  EXPECT_FALSE(bufferPool.isDirty(pid));
  bufferPool.flushPage(pid);

  const db::DbFile &file = db.get(name);
  const auto &reads = file.getReads();
  const auto &writes = file.getWrites();
  EXPECT_EQ(reads.size(), 1);
  EXPECT_EQ(reads[0], 0);
  EXPECT_EQ(writes.size(), 1);
  EXPECT_EQ(writes[0], 0);
}

TEST(BufferPoolTest, discardPage) {
  db::Database &db = db::getDatabase();
  db::BufferPool &bufferPool = db.getBufferPool();

  std::string name{"file"};
  db::TupleDesc td;
  db.add(std::make_unique<db::DbFile>(name, td));
  db::PageId pid{name, 0};
  bufferPool.getPage(pid);
  bufferPool.markDirty(pid);
  EXPECT_TRUE(bufferPool.contains(pid));
  EXPECT_TRUE(bufferPool.isDirty(pid));
  bufferPool.discardPage(pid);
  EXPECT_FALSE(bufferPool.contains(pid));
  EXPECT_ANY_THROW(bufferPool.isDirty(pid));

  const db::DbFile &file = db.get(name);
  const auto &reads = file.getReads();
  const auto &writes = file.getWrites();
  EXPECT_EQ(reads.size(), 1);
  EXPECT_EQ(reads[0], 0);
  EXPECT_EQ(writes.size(), 0);
}

TEST(BefferPoolTest, flushFile) {
  constexpr size_t size = 10;
  db::Database &db = db::getDatabase();
  db::BufferPool &bufferPool = db.getBufferPool();

  std::string name{"file"};
  db::TupleDesc td;
  db.add(std::make_unique<db::DbFile>(name, td));
  for (size_t i = 0; i < size; i++) {
    db::PageId pid{name, i};
    bufferPool.getPage(pid);
    if (i % 2 == 0) {
      bufferPool.markDirty(pid);
    }
  }
  bufferPool.flushFile(name);
  for (size_t i = 0; i < size; i++) {
    db::PageId pid{name, i};
    EXPECT_TRUE(bufferPool.contains(pid));
    EXPECT_FALSE(bufferPool.isDirty(pid));
  }

  const db::DbFile &file = db.get(name);
  const auto &reads = file.getReads();
  const auto &writes = file.getWrites();
  EXPECT_EQ(reads.size(), size);
  EXPECT_EQ(writes.size(), size / 2);
}

TEST(BufferPoolTest, LRU) {
  db::Database &db = db::getDatabase();
  db::BufferPool &bufferPool = db.getBufferPool();

  std::string name{"file"};
  db::TupleDesc td;
  db.add(std::make_unique<db::DbFile>(name, td));
  std::array<db::Page *, db::DEFAULT_NUM_PAGES> pages{};
  // fill the buffer pool with pages [0, DEFAULT_NUM_PAGES)
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES; i++) {
    db::PageId pid{name, i};
    pages[i] = &bufferPool.getPage(pid);
    bufferPool.markDirty(pid);
  }

  constexpr size_t size = 10;
  // touch pages [0, size)
  for (size_t i = 0; i < size; i++) {
    bufferPool.getPage({name, i});
  }

  // read some new pages. This should evict pages [size, 2 * size)
  for (size_t i = 0; i < size; i++) {
    bufferPool.getPage({name, db::DEFAULT_NUM_PAGES + i});
  }

  const db::DbFile &file = db.get(name);
  const auto &reads = file.getReads();
  const auto &writes = file.getWrites();
  EXPECT_EQ(reads.size(), db::DEFAULT_NUM_PAGES + size);
  EXPECT_EQ(writes.size(), size);

  // fetch pages [size, 2 * size) again. This should evict pages [2 * size, 3 * size)
  for (size_t i = size; i < size + size; i++) {
    bufferPool.getPage({name, i});
  }
  EXPECT_EQ(reads.size(), db::DEFAULT_NUM_PAGES + size + size);
  EXPECT_EQ(writes.size(), size + size);
  for (size_t i = 0; i < db::DEFAULT_NUM_PAGES + size; i++) {
    EXPECT_EQ(reads[i], i);
  }
  for (size_t i = 0; i < size; i++) {
    EXPECT_EQ(reads[db::DEFAULT_NUM_PAGES + size + i], size + i);
  }

  for (size_t i = 0; i < size + size; i++) {
    EXPECT_EQ(writes[i], size + i);
  }
}
