#include <db/Tuple.hpp>
#include <gtest/gtest.h>

TEST(TupleTest, Constructor) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};

  EXPECT_NO_THROW(db::TupleDesc td(types, {"id", "name", "price"}));  // Ok
  EXPECT_ANY_THROW(db::TupleDesc td(types, {"id", "id", "price"}));  // Non-unique names
  EXPECT_ANY_THROW(db::TupleDesc td(types, {"id", "price"}));  // Missing name
  EXPECT_ANY_THROW(db::TupleDesc td(types, {"id", "name", "price", "field"}));  // Extra name
}

TEST(TupleTest, OffsetOf) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names{"id", "name", "price"};
  db::TupleDesc td(types, names);

  EXPECT_EQ(td.size(), 3);
  EXPECT_EQ(td.offset_of(0), 0);
  EXPECT_EQ(td.offset_of(1), db::INT_SIZE);
  EXPECT_EQ(td.offset_of(2), db::INT_SIZE + db::CHAR_SIZE);
  EXPECT_ANY_THROW(td.offset_of(3));
}

TEST(TupleTest, IndexOf) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names{"id", "name", "price"};
  db::TupleDesc td(types, names);

  EXPECT_EQ(td.size(), 3);
  EXPECT_EQ(td.index_of("id"), 0);
  EXPECT_EQ(td.index_of("name"), 1);
  EXPECT_EQ(td.index_of("price"), 2);
  EXPECT_ANY_THROW(td.index_of("invalid"));
}

TEST(TupleTest, Compatible) {
  std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names{"id", "name", "price"};
  db::TupleDesc td(types, names);

  EXPECT_TRUE(td.compatible(db::Tuple({123, "Hello", 3.14})));  // Compatible
  EXPECT_FALSE(td.compatible(db::Tuple({123, "Hello"})));  // Missing field
  EXPECT_FALSE(td.compatible(db::Tuple({123, "Hello", 3.14, "extra"})));  // Extra field
  EXPECT_FALSE(td.compatible(db::Tuple({123, 3.14, "Hello"})));  // Wrong type
}

TEST(TupleTest, Length) {
  {
    std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
    std::vector<std::string> names{"id", "name", "price"};
    db::TupleDesc td(types, names);

    EXPECT_EQ(td.length(), db::INT_SIZE + db::CHAR_SIZE + db::DOUBLE_SIZE);
  }
  {
    std::vector<db::type_t> types{db::type_t::CHAR, db::type_t::DOUBLE};
    std::vector<std::string> names{"name", "price"};
    db::TupleDesc td(types, names);

    EXPECT_EQ(td.length(), db::CHAR_SIZE + db::DOUBLE_SIZE);
  }
  {
    std::vector<db::type_t> types{db::type_t::INT, db::type_t::CHAR, db::type_t::INT};
    std::vector<std::string> names{"id", "name", "price"};
    db::TupleDesc td(types, names);

    EXPECT_EQ(td.length(), db::INT_SIZE + db::CHAR_SIZE + db::INT_SIZE);
  }
}

TEST(TupleTest, Merge) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id1", "name1", "price1"};
  db::TupleDesc td1(types1, names1);

  std::vector<db::type_t> types2{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names2{"id2", "name2", "price2"};
  db::TupleDesc td2(types2, names2);

  db::TupleDesc td = db::TupleDesc::merge(td1, td2);

  EXPECT_EQ(td.size(), 6);
  EXPECT_EQ(td.offset_of(0), 0);
  EXPECT_EQ(td.offset_of(1), db::INT_SIZE);
  EXPECT_EQ(td.offset_of(2), db::INT_SIZE + db::CHAR_SIZE);
  EXPECT_EQ(td.offset_of(3), db::INT_SIZE + db::CHAR_SIZE + db::DOUBLE_SIZE);
  EXPECT_EQ(td.offset_of(4), db::INT_SIZE + db::CHAR_SIZE + db::DOUBLE_SIZE + db::INT_SIZE);
  EXPECT_EQ(td.offset_of(5), db::INT_SIZE + db::CHAR_SIZE + db::DOUBLE_SIZE + db::INT_SIZE + db::CHAR_SIZE);
  EXPECT_EQ(td.index_of("id1"), 0);
  EXPECT_EQ(td.index_of("name1"), 1);
  EXPECT_EQ(td.index_of("price1"), 2);
  EXPECT_EQ(td.index_of("id2"), 3);
  EXPECT_EQ(td.index_of("name2"), 4);
  EXPECT_EQ(td.index_of("price2"), 5);
}

TEST(TupleTest, MergeConflict) {
  std::vector<db::type_t> types1{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names1{"id", "name1", "price1"};
  db::TupleDesc td1(types1, names1);
  std::vector<db::type_t> types2{db::type_t::INT, db::type_t::CHAR, db::type_t::DOUBLE};
  std::vector<std::string> names2{"id", "name2", "price2"};
  db::TupleDesc td2(types2, names2);

  EXPECT_ANY_THROW(db::TupleDesc::merge(td1, td2));  // Non-unique names
}
