#include <db/LeafPage.hpp>
#include <gtest/gtest.h>

TEST(LeafTest, InsertFirst) {
  db::Page page{};
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  db::LeafPage leaf{page, td, 0};
  int capacity = leaf.capacity;
  EXPECT_EQ(capacity, 53);
  std::vector<int> ids;
  ids.push_back(0);
  for (int i = 1; i < capacity; i++) {
    ids.push_back(i * 2);
    db::Tuple t{{i * 2, "apple", 1.0}};
    EXPECT_FALSE(leaf.insertTuple(t));
  }
  EXPECT_TRUE(leaf.insertTuple({{0, "apple", 1.0}}));

  for (int i = 0; i < capacity; i++) {
    db::Tuple t = leaf.getTuple(i);
    EXPECT_EQ(t.get_field(0), db::field_t{ids[i]});
    EXPECT_EQ(t.get_field(1), db::field_t{"apple"});
  }
}

TEST(LeafTest, InsertLast) {
  db::Page page{};
  db::TupleDesc td({db::type_t::CHAR, db::type_t::INT, db::type_t::DOUBLE}, {"name", "id", "price"});
  db::LeafPage leaf{page, td, 1};
  int capacity = leaf.capacity;
  EXPECT_EQ(capacity, 53);
  std::vector<int> ids;
  for (int i = 1; i < capacity; i++) {
    ids.push_back(i * 2);
    db::Tuple t{{"apple", i * 2, 1.0}};
    EXPECT_FALSE(leaf.insertTuple(t));
  }
  ids.push_back(capacity * 2);
  EXPECT_TRUE(leaf.insertTuple({{"apple", capacity * 2, 1.0}}));

  for (int i = 0; i < capacity; i++) {
    db::Tuple t = leaf.getTuple(i);
    EXPECT_EQ(t.get_field(1), db::field_t{ids[i]});
    EXPECT_EQ(t.get_field(0), db::field_t{"apple"});
  }
}

TEST(LeafTest, InsertMiddle) {
  db::Page page{};
  db::TupleDesc td({db::type_t::CHAR, db::type_t::DOUBLE, db::type_t::INT}, {"name", "price", "id"});
  db::LeafPage leaf{page, td, 2};
  int capacity = leaf.capacity;
  EXPECT_EQ(capacity, 53);
  std::vector<int> ids;
  for (int i = 1; i < capacity; i++) {
    ids.push_back(i * 2);
    db::Tuple t{{"apple", 1.0, i * 2}};
    EXPECT_FALSE(leaf.insertTuple(t));
  }
  ids.push_back(21);
  std::sort(ids.begin(), ids.end());
  EXPECT_TRUE(leaf.insertTuple({{"apple", 1.0, 21}}));

  for (int i = 0; i < capacity; i++) {
    db::Tuple t = leaf.getTuple(i);
    EXPECT_EQ(t.get_field(2), db::field_t{ids[i]});
    EXPECT_EQ(t.get_field(0), db::field_t{"apple"});
  }
}

TEST(LeafTest, Update) {
  db::Page page{};
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  db::LeafPage leaf{page, td, 0};
  int capacity = leaf.capacity;
  EXPECT_EQ(capacity, 53);
  std::vector<int> ids;
  for (int i = 1; i < capacity; i++) {
    ids.push_back(i * 2);
    db::Tuple t{{i * 2, "apple", 1.0}};
    EXPECT_FALSE(leaf.insertTuple(t));
  }

  for (int i = 0; i < capacity - 1; i++) {
    db::Tuple t = leaf.getTuple(i);
    EXPECT_EQ(t.get_field(0), db::field_t{ids[i]});
    EXPECT_EQ(t.get_field(1), db::field_t{"apple"});
  }

  for (int i = 1; i < capacity; i++) {
    db::Tuple t{{i * 2, "orange", 1.0}};
    EXPECT_FALSE(leaf.insertTuple(t));
  }
  ids.push_back(capacity * 2);
  EXPECT_TRUE(leaf.insertTuple({{capacity * 2, "apple", 1.0}}));
  EXPECT_TRUE(leaf.insertTuple({{capacity * 2, "orange", 1.0}}));

  for (int i = 0; i < capacity; i++) {
    db::Tuple t = leaf.getTuple(i);
    EXPECT_EQ(t.get_field(0), db::field_t{ids[i]});
    EXPECT_EQ(t.get_field(1), db::field_t{"orange"});
  }
}

TEST(LeafTest, Split) {
  db::Page page{};
  db::TupleDesc td({db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE}, {"id", "name", "price"});
  db::LeafPage leaf{page, td, 0};
  const size_t rand_leaf = rand();
  leaf.header->next_leaf = rand_leaf;
  int capacity = leaf.capacity;
  EXPECT_EQ(capacity, 53);
  for (int i = 0; i < capacity - 1; i++) {
    db::Tuple t{{i * 2, "apple", 1.0}};
    EXPECT_FALSE(leaf.insertTuple(t));
  }

  EXPECT_TRUE(leaf.insertTuple({{(capacity - 1) * 2, "apple", 1.0}}));
  db::Page new_page{};
  db::LeafPage new_leaf{new_page, td, 0};
  leaf.split(new_leaf);
  EXPECT_EQ(leaf.header->size + new_leaf.header->size, capacity);
  EXPECT_EQ(leaf.header->size, capacity / 2);
  EXPECT_EQ(new_leaf.header->next_leaf, rand_leaf);
  for (int i = 0; i < leaf.header->size; i++) {
    db::Tuple t = leaf.getTuple(i);
    EXPECT_EQ(t.get_field(0), db::field_t{i * 2});
  }
  for (int i = 0; i < new_leaf.header->size; i++) {
    db::Tuple t = new_leaf.getTuple(i);
    EXPECT_EQ(t.get_field(0), db::field_t{(leaf.header->size + i) * 2});
  }
}
