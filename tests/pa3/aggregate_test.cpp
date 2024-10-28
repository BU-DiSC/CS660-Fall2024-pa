#include <db/Database.hpp>
#include <db/HeapFile.hpp>
#include <db/Query.hpp>
#include <gtest/gtest.h>
#include <random>

TEST(AggregateTest, Min) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT};
  std::vector<std::string> names2{"result"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  std::vector<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 1000) {
    values.push_back(dis(gen));
  }

  for (const auto &i : values) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::aggregate(in, out, {std::nullopt, db::AggregateOp::MIN, "id"});
  auto it = out.begin();
  EXPECT_NE(it, out.end());
  auto t = *it;
  db::field_t expected = *std::min_element(values.begin(), values.end());
  EXPECT_EQ(t.get_field(0), expected);
  ++it;
  EXPECT_EQ(it, out.end());
}

TEST(AggregateTest, Max) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT};
  std::vector<std::string> names2{"result"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  std::vector<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 1000) {
    values.push_back(dis(gen));
  }

  for (const auto &i : values) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::aggregate(in, out, {std::nullopt, db::AggregateOp::MAX, "id"});
  auto it = out.begin();
  EXPECT_NE(it, out.end());
  auto t = *it;
  db::field_t expected = *std::max_element(values.begin(), values.end());
  EXPECT_EQ(t.get_field(0), expected);
  ++it;
  EXPECT_EQ(it, out.end());
}

TEST(AggregateTest, Count) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT};
  std::vector<std::string> names2{"result"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  std::vector<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 1000) {
    values.push_back(dis(gen));
  }

  for (const auto &i : values) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::aggregate(in, out, {std::nullopt, db::AggregateOp::COUNT, "id"});
  auto it = out.begin();
  EXPECT_NE(it, out.end());
  auto t = *it;
  db::field_t expected = int(values.size());
  EXPECT_EQ(t.get_field(0), expected);
  ++it;
  EXPECT_EQ(it, out.end());
}

TEST(AggregateTest, Sum) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT};
  std::vector<std::string> names2{"result"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  std::vector<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 1000) {
    values.push_back(dis(gen));
  }

  for (const auto &i : values) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::aggregate(in, out, {std::nullopt, db::AggregateOp::SUM, "id"});
  auto it = out.begin();
  EXPECT_NE(it, out.end());
  auto t = *it;
  db::field_t expected = std::accumulate(values.begin(), values.end(), 0);
  EXPECT_EQ(t.get_field(0), expected);
  ++it;
  EXPECT_EQ(it, out.end());
}

TEST(AggregateTest, Average) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::DOUBLE};
  std::vector<std::string> names2{"result"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  std::vector<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 1000) {
    values.push_back(dis(gen));
  }

  for (const auto &i : values) {
    in.insertTuple({{i, "Hello", 3.14}});
  }

  db::aggregate(in, out, {std::nullopt, db::AggregateOp::AVG, "id"});
  auto it = out.begin();
  EXPECT_NE(it, out.end());
  auto t = *it;
  db::field_t expected = std::accumulate(values.begin(), values.end(), 0) / double(values.size());
  EXPECT_EQ(t.get_field(0), expected);
  ++it;
  EXPECT_EQ(it, out.end());
}

TEST(AggregateTest, Grouped) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name", "price"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names2{"name", "result"};
  db::TupleDesc td2(types2, names2);

  const char *in_name = "heapfile.in";
  const char *out_name = "heapfile.out";
  std::remove(in_name);
  std::remove(out_name);
  db::getDatabase().add(std::make_unique<db::HeapFile>(in_name, td1));
  db::getDatabase().add(std::make_unique<db::HeapFile>(out_name, td2));
  auto &in = db::getDatabase().get(in_name);
  auto &out = db::getDatabase().get(out_name);

  std::vector<int> values;
  std::mt19937 gen(1234);
  std::uniform_int_distribution<> dis(-100000, 100000);
  while (values.size() < 1000) {
    values.push_back(dis(gen));
  }

  int even = 0;
  int even_cnt = 0;
  int odd = 0;
  int odd_cnt = 0;
  for (const auto &i : values) {
    std::string group = i % 2 == 0 ? "even" : "odd";
    if (i % 2 == 0) {
      even += i;
      ++even_cnt;
    } else {
      odd += i;
      ++odd_cnt;
    }
    in.insertTuple({{i, group, 3.14}});
  }

  db::aggregate(in, out, {"name", db::AggregateOp::AVG, "id"});
  auto it = out.begin();
  EXPECT_NE(it, out.end());
  auto t = *it;
  db::field_t even_key = "even";
  db::field_t odd_key = "odd";
  if (t.get_field(0) == even_key) {
    db::field_t expected = even / double(even_cnt);
    EXPECT_EQ(t.get_field(1), expected);
    ++it;
    EXPECT_NE(it, out.end());
    t = *it;
    EXPECT_EQ(t.get_field(0), odd_key);
    expected = odd / double(odd_cnt);
    EXPECT_EQ(t.get_field(1), expected);
  } else if (t.get_field(0) == odd_key) {
    db::field_t expected = odd / double(odd_cnt);
    EXPECT_EQ(t.get_field(1), expected);
    ++it;
    EXPECT_NE(it, out.end());
    t = *it;
    EXPECT_EQ(t.get_field(0), even_key);
    expected = even / double(even_cnt);
    EXPECT_EQ(t.get_field(1), expected);
  } else {
    EXPECT_FALSE(true);
  }
  ++it;
  EXPECT_EQ(it, out.end());
}
