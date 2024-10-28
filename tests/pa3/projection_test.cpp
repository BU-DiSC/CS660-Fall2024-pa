#include <db/Database.hpp>
#include <db/HeapFile.hpp>
#include <db/Query.hpp>
#include <gtest/gtest.h>

TEST(ProjectionTest, Rename) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id1", "name1", "price1"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names2{"id2", "name2", "price2"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";

  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  constexpr size_t capacity = 53;
  for (int i = 0; i < capacity * 3; ++i) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::projection(in, out, names1);

  int count = 0;
  auto it1 = in.begin();
  auto it2 = out.begin();
  while (it1 != in.end() && it2 != out.end()) {
    auto t1 = *it1;
    auto t2 = *it2;
    EXPECT_EQ(t1.size(), td1.size());
    EXPECT_EQ(t1.size(), t2.size());
    for (size_t i = 0; i < t1.size(); ++i) {
      EXPECT_EQ(t1.get_field(i), t2.get_field(i));
    }
    ++it1;
    ++it2;
    ++count;
  }
  EXPECT_EQ(it1, in.end());
  EXPECT_EQ(it2, out.end());
  EXPECT_EQ(count, capacity * 3);
}

TEST(ProjectionTest, Reorder) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::CHAR, db::type_t::DOUBLE, db::type_t::INT};
  std::vector<std::string> names2{"name", "price", "id"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";

  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  constexpr size_t capacity = 53;
  for (int i = 0; i < capacity * 3; ++i) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::projection(in, out, names2);

  int count = 0;
  auto it1 = in.begin();
  auto it2 = out.begin();
  while (it1 != in.end() && it2 != out.end()) {
    auto t1 = *it1;
    auto t2 = *it2;
    EXPECT_EQ(t1.size(), td1.size());
    EXPECT_EQ(t1.size(), t2.size());
    for (size_t i = 0; i < t1.size(); ++i) {
      EXPECT_EQ(t1.get_field(td1.index_of(names1[i])), t2.get_field(td2.index_of(names1[i])));
    }
    EXPECT_EQ(t1.get_field(0), t2.get_field(2));
    EXPECT_EQ(t1.get_field(1), t2.get_field(0));
    EXPECT_EQ(t1.get_field(2), t2.get_field(1));
    ++it1;
    ++it2;
    ++count;
  }
  EXPECT_EQ(it1, in.end());
  EXPECT_EQ(it2, out.end());
  EXPECT_EQ(count, capacity * 3);
}

TEST(ProjectionTest, Drop) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::DOUBLE, db::type_t::INT};
  std::vector<std::string> names2{"price", "id"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";

  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  constexpr size_t capacity = 53;
  for (int i = 0; i < capacity * 3; ++i) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::projection(in, out, names2);

  int count = 0;
  auto it1 = in.begin();
  auto it2 = out.begin();
  while (it1 != in.end() && it2 != out.end()) {
    auto t1 = *it1;
    auto t2 = *it2;
    EXPECT_EQ(t1.size(), td1.size());
    EXPECT_EQ(t1.size(), t2.size() + 1);
    for (size_t i = 0; i < t2.size(); ++i) {
      EXPECT_EQ(t1.get_field(td1.index_of(names2[i])), t2.get_field(td2.index_of(names2[i])));
    }
    ++it1;
    ++it2;
    ++count;
  }
  EXPECT_EQ(it1, in.end());
  EXPECT_EQ(it2, out.end());
  EXPECT_EQ(count, capacity * 3);
}

TEST(ProjectionTest, Duplicate) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT, db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE, db::type_t::DOUBLE};
  std::vector<std::string> names2{"id1", "id2", "name", "price1", "price2"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";

  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  constexpr size_t capacity = 53;
  for (int i = 0; i < capacity * 3; ++i) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::projection(in, out, {"id", "id", "name", "price", "price"});

  int count = 0;
  auto it1 = in.begin();
  auto it2 = out.begin();
  while (it1 != in.end() && it2 != out.end()) {
    auto t1 = *it1;
    auto t2 = *it2;
    EXPECT_EQ(t1.size(), td1.size());
    EXPECT_EQ(t1.size() + 2, t2.size());
    EXPECT_EQ(t1.get_field(0), t2.get_field(0));
    EXPECT_EQ(t1.get_field(0), t2.get_field(1));
    EXPECT_EQ(t1.get_field(1), t2.get_field(2));
    EXPECT_EQ(t1.get_field(2), t2.get_field(3));
    EXPECT_EQ(t1.get_field(2), t2.get_field(4));
    ++it1;
    ++it2;
    ++count;
  }
  EXPECT_EQ(it1, in.end());
  EXPECT_EQ(it2, out.end());
  EXPECT_EQ(count, capacity * 3);
}
