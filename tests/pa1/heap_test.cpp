#include <db/Database.hpp>
#include <db/HeapPage.hpp>
#include <db/HeapFile.hpp>
#include <gtest/gtest.h>

TEST(HeapPageTest, EmptyPage) {
  db::Page page{};
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  db::HeapPage hp(page, td);
  EXPECT_EQ(hp.begin(), hp.end());
}

TEST(HeapPageTest, FullHeader) {
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  // A page can fit 53 tuples, but the header has 56 bits. The last 3 bits are not used.
  db::Page full_page1{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0b11111000}; // full header
  db::HeapPage hp1(full_page1, td);
  constexpr size_t capacity = 53;
  size_t count = 0;
  size_t slot = hp1.begin();
  while (slot != hp1.end()) {
    hp1.next(slot);
    count++;
  }
  EXPECT_EQ(count, capacity);

  db::Page full_page2{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // the last 3 bits (padding) should not affect the size
  db::HeapPage hp2(full_page2, td);
  count = 0;
  slot = hp2.begin();
  while (slot != hp2.end()) {
    hp2.next(slot);
    count++;
  }
  EXPECT_EQ(count, capacity);
}

TEST(HeapPageTest, CountTuples) {
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  db::Page page{0xff, 0b00111101, 0x00, 0b01110000, 0xff, 0xff, 0b11111000}; // 8 + 5 + 0 + 3 + 8 + 8 + 5
  db::HeapPage hp(page, td);
  int capacity = 37;
  size_t count = 0;
  size_t slot = hp.begin();
  while (slot != hp.end()) {
    hp.next(slot);
    count++;
  }
  EXPECT_EQ(count, capacity);
}

TEST(HeapPageTest, InsertTuple) {
  db::Page page{};
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  db::HeapPage hp(page, td);
  hp.insertTuple({{660, "Hello CS660!", 0.0}});
  EXPECT_NE(hp.begin(), hp.end());
  int capacity = 53;
  for (int i = 1; i < capacity; i++) {
    hp.insertTuple({{i, "Hello CS660!", 0.0}});
  }
  size_t count = 0;
  size_t slot = hp.begin();
  while (slot != hp.end()) {
    hp.next(slot);
    count++;
  }
  EXPECT_EQ(count, capacity);
  EXPECT_FALSE(hp.insertTuple({{capacity, "full page", 0.0}}));
}

TEST(HeapPageTest, DeleteTuple) {
  db::Page page{0b11111111, 0b00111101};
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  db::HeapPage hp(page, td);
  EXPECT_NO_THROW(hp.deleteTuple(0));
  EXPECT_NO_THROW(hp.deleteTuple(1));
  EXPECT_NO_THROW(hp.deleteTuple(2));
  EXPECT_ANY_THROW(hp.deleteTuple(2)); // already deleted
  EXPECT_NO_THROW(hp.deleteTuple(6));
  EXPECT_NO_THROW(hp.deleteTuple(7));
  EXPECT_ANY_THROW(hp.deleteTuple(8));
  EXPECT_ANY_THROW(hp.deleteTuple(9));
  EXPECT_NO_THROW(hp.deleteTuple(10));
  EXPECT_NO_THROW(hp.deleteTuple(11));
  EXPECT_NO_THROW(hp.deleteTuple(12));
  EXPECT_ANY_THROW(hp.deleteTuple(14));

  // slots 3 4 5 13 15 are still populated
  EXPECT_NE(hp.begin(), hp.end());
  size_t count = 0;
  size_t slot = hp.begin();
  while (slot != hp.end()) {
    hp.next(slot);
    count++;
  }
  EXPECT_EQ(count, 5);
}

TEST(HeapPageTest, GetTuple) {
  db::Page page{
      // header
      0xff, 0x00, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      // data
      0x94, 0x02, 0x00, 0x00, // int

      0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x43, 0x53, 0x36, 0x36, 0x30, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // char

      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // double

      0x00, 0x00, 0x01, 0x00, // int ...
  };
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  db::HeapPage hp(page, td);
  EXPECT_NE(hp.begin(), hp.end());
  EXPECT_EQ(hp.end(), db::DEFAULT_PAGE_SIZE / (db::INT_SIZE + db::CHAR_SIZE + db::DOUBLE_SIZE));

  size_t slot = hp.begin();
  const auto &t1 = hp.getTuple(slot);
  EXPECT_EQ(std::get<int>(t1.get_field(0)), 660);
  EXPECT_EQ(std::get<std::string>(t1.get_field(1)), "Hello CS660!");
  hp.next(slot);
  const auto &t2 = hp.getTuple(slot);
  EXPECT_EQ(std::get<int>(t2.get_field(0)), 65535 + 1);

  size_t count = 1;
  while (slot != hp.end()) {
    const auto &t = hp.getTuple(slot);
    hp.next(slot);
    count++;
  }
  EXPECT_EQ(count, 20);
}

TEST(HeapFileTest, InsertTuple) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names{"id", "name", "price"};
  db::TupleDesc td(types, names);

  const char *name = "heapfile";
  std::remove(name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(name, td));
  auto &file = db::getDatabase().get(name);
  EXPECT_EQ(file.begin(), file.end());
  constexpr size_t capacity = 53;
  for (int i = 0; i < capacity * 3; ++i) {
    file.insertTuple({{i, "Hello", 3.14}});
  }

  auto it = file.begin();
  for (int i = 0; i < capacity * 3; i += 2) {
    it.page = i / capacity;
    it.slot = i % capacity;
    file.deleteTuple(it);
  }

  int i = 1;
  for (const auto &t : file) {
    EXPECT_EQ(std::get<int>(t.get_field(0)), i);
    i += 2;
  }
}

TEST(HeapFileTest, EmptyPage) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names{"id", "name", "price"};
  db::TupleDesc td(types, names);

  const char *name = "heapfile";
  std::remove(name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(name, td));
  auto &file = db::getDatabase().get(name);
  constexpr size_t capacity = 53;
  for (int i = 0; i < capacity; ++i) {
    file.insertTuple({{i, "Hello", 3.14}});
    EXPECT_EQ(file.getNumPages(), 1);
  }
  for (int i = capacity; i < capacity + capacity; ++i) {
    file.insertTuple({{i, "Hello", 3.14}});
    EXPECT_EQ(file.getNumPages(), 2);
  }

  auto it = file.begin();
  for (int i = 0; i < capacity; ++i) {
    it.slot = i;
    file.deleteTuple(it);
    EXPECT_EQ(file.getNumPages(), 2);
  }
  EXPECT_EQ(file.getNumPages(), 2);

  int i = capacity;
  for (const auto &t : file) {
    EXPECT_EQ(std::get<int>(t.get_field(0)), i);
    i++;
  }
}
