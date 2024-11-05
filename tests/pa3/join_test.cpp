#include <db/Database.hpp>
#include <db/HeapFile.hpp>
#include <db/Query.hpp>
#include <gtest/gtest.h>
#include <random>

TEST(JoinTest, Small) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT, db::type_t::INT};
  std::vector<std::string> names2{"quantity", "id"};
  db::TupleDesc td2(types2, names2);

  std::vector<db::type_t> types3{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE, db::type_t::INT};
  std::vector<std::string> names3{"id", "name", "price", "quantity"};
  db::TupleDesc td3(types3, names3);

  const char *left_name = "left.in";
  const char *right_name = "right.in";
  const char *out_name = "heapfile.out";
  std::remove(left_name);
  std::remove(right_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(left_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(right_name, td2));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td3));
  auto &left = db::getDatabase().get(left_name);
  auto &right = db::getDatabase().get(right_name);
  auto &out = db::getDatabase().get(out_name);

  std::unordered_set<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 300) {
    values.insert(dis(gen));
  }

  int expected = 0;
  for (const auto &i : values) {
    left.insertTuple({{i, "Hello", 3.14}});
    if (dis(gen) % 2 == 0) {
      right.insertTuple({{10 + i, i}});
      ++expected;
    }
  }

  db::join(left, right, out, {"id", db::PredicateOp::EQ, "id"});
  auto it = out.begin();
  int i = 0;
  while (it != out.end()) {
    ++it;
    ++i;
  }
  EXPECT_EQ(i, expected);
}

TEST(JoinTest, NotEqual) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT, db::type_t::INT};
  std::vector<std::string> names2{"quantity", "id"};
  db::TupleDesc td2(types2, names2);

  std::vector<db::type_t> types3{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE, db::type_t::INT, db::type_t::INT};
  std::vector<std::string> names3{"id1", "name", "price", "quantity", "id2"};
  db::TupleDesc td3(types3, names3);

  const char *left_name = "left.in";
  const char *right_name = "right.in";
  const char *out_name = "heapfile.out";
  std::remove(left_name);
  std::remove(right_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(left_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(right_name, td2));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td3));
  auto &left = db::getDatabase().get(left_name);
  auto &right = db::getDatabase().get(right_name);
  auto &out = db::getDatabase().get(out_name);

  std::unordered_set<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 1500) {
    values.insert(dis(gen));
  }

  for (const auto &i : values) {
    right.insertTuple({{10 + i, i * 2}});
  }
  for (const auto &i : values) {
    left.insertTuple({{i * 2 + 1, "Hello", 3.14}});
  }

  db::join(left, right, out, {"id", db::PredicateOp::NE, "id"});
  auto it = out.begin();
  int i = 0;
  while (it != out.end()) {
    ++it;
    ++i;
  }
  EXPECT_EQ(i, values.size() * values.size());
}

TEST(JoinTest, Large) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT, db::type_t::INT};
  std::vector<std::string> names2{"quantity", "id"};
  db::TupleDesc td2(types2, names2);

  std::vector<db::type_t> types3{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE, db::type_t::INT};
  std::vector<std::string> names3{"id", "name", "price", "quantity"};
  db::TupleDesc td3(types3, names3);

  const char *left_name = "left.in";
  const char *right_name = "right.in";
  const char *out_name = "heapfile.out";
  std::remove(left_name);
  std::remove(right_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(left_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(right_name, td2));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td3));
  auto &left = db::getDatabase().get(left_name);
  auto &right = db::getDatabase().get(right_name);
  auto &out = db::getDatabase().get(out_name);

  std::unordered_set<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 500) {
    values.insert(dis(gen));
  }

  int expected = 0;
  for (const auto &i : values) {
    left.insertTuple({{i, "Hello", 3.14}});
    if (dis(gen) % 2 == 0) {
      right.insertTuple({{10 + i, i}});
      ++expected;
    }
  }

  db::join(left, right, out, {"id", db::PredicateOp::EQ, "id"});
  auto it = out.begin();
  int i = 0;
  while (it != out.end()) {
    ++it;
    ++i;
  }
  EXPECT_EQ(i, expected);
}
