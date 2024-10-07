#include <db/IndexPage.hpp>
#include <gtest/gtest.h>

TEST(IndexTest, InsertFirst) {
  db::Page page{};
  db::IndexPage index{page};
  int capacity = index.capacity;
  EXPECT_EQ(sizeof(size_t), 8);
  EXPECT_EQ(capacity, 340);

  for (int i = 1; i < capacity; i++) {
    EXPECT_FALSE(index.insert(i * 2, 1000 + i));
  }
  EXPECT_TRUE(index.insert(0, 1000));

  for (int i = 0; i < capacity; i++) {
    EXPECT_EQ(index.keys[i], i * 2);
  }
}

TEST(IndexTest, InsertLast) {
  db::Page page{};
  db::IndexPage index{page};
  int capacity = index.capacity;
  EXPECT_EQ(capacity, 340);

  for (int i = 1; i < capacity; i++) {
    EXPECT_FALSE(index.insert(i * 2, 1000 + i));
  }
  EXPECT_TRUE(index.insert(capacity * 2, 1000 + capacity));

  for (int i = 0; i < capacity; i++) {
    EXPECT_EQ(index.keys[i], (i + 1) * 2);
  }
}

TEST(IndexTest, InsertMiddle) {
  db::Page page{};
  db::IndexPage index{page};
  int capacity = index.capacity;
  EXPECT_EQ(capacity, 340);

  std::vector<int> ids;
  for (int i = 1; i < capacity; i++) {
    ids.push_back(i * 2);
    EXPECT_FALSE(index.insert(i * 2, 1000 + i));
  }
  ids.push_back(21);
  EXPECT_TRUE(index.insert(21, 1000 + capacity));
  std::sort(ids.begin(), ids.end());

  for (int i = 0; i < capacity; i++) {
    EXPECT_EQ(index.keys[i], ids[i]);
  }
}

TEST(IndexTest, Split) {
  db::Page page{};
  db::IndexPage index{page};
  int capacity = index.capacity;
  EXPECT_EQ(capacity, 340);

  for (int i = 0; i < capacity - 1; i++) {
    db::Tuple t{{i * 2, "apple", 1.0}};
    EXPECT_FALSE(index.insert(i * 2, 1000 + i));
  }

  EXPECT_TRUE(index.insert((capacity - 1) * 2, 1000 + capacity - 1));
  db::Page new_page{};
  db::IndexPage new_index{new_page};
  index.split(new_index);
  EXPECT_EQ(index.header->size + new_index.header->size + 1, capacity);
  EXPECT_EQ(index.header->size, capacity / 2);
  for (int i = 0; i < index.header->size; i++) {
    EXPECT_EQ(index.keys[i], i * 2);
  }
  for (int i = 0; i < new_index.header->size; i++) {
    EXPECT_EQ(new_index.keys[i], (i + 1 + index.header->size) * 2);
  }
}
