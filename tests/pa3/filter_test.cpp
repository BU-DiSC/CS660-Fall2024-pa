#include <db/Database.hpp>
#include <db/HeapFile.hpp>
#include <db/Query.hpp>
#include <gtest/gtest.h>

TEST(FilterTest, All) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names{"id", "name", "price"};
  db::TupleDesc td(types, names);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);
  constexpr size_t capacity = 53;
  for (int i = 0; i < capacity * 6; ++i) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::filter(in, out, {});

  int i = 0;
  for (const auto &t : out) {
    EXPECT_EQ(get<int>(t.get_field(0)), i);
    ++i;
  }
  EXPECT_EQ(i, capacity * 6);
}

TEST(FilterTest, Some) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names{"id", "name", "price"};
  db::TupleDesc td(types, names);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);
  constexpr size_t capacity = 53;
  for (int i = 0; i < capacity * 6; ++i) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::FilterPredicate pred1{"id", db::PredicateOp::GE, 30};
  db::FilterPredicate pred2{"id", db::PredicateOp::LT, 300};
  db::filter(in, out, {pred1, pred2});

  int i = 30;
  for (const auto &t : out) {
    EXPECT_EQ(get<int>(t.get_field(0)), i);
    ++i;
  }
  EXPECT_EQ(i, 300);
}

TEST(FilterTest, One) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names{"id", "name", "price"};
  db::TupleDesc td(types, names);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);
  constexpr int capacity = 53;
  for (int i = 0; i < capacity * 6; ++i) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::FilterPredicate pred{"id", db::PredicateOp::EQ, 30};
  db::filter(in, out, {pred});

  int i = 30;
  for (const auto &t : out) {
    EXPECT_EQ(get<int>(t.get_field(0)), i);
    ++i;
  }
  EXPECT_EQ(i, 31);
}
