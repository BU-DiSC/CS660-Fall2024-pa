#include <gtest/gtest.h>

#include <db/Database.hpp>
#include <db/DbFile.hpp>

TEST(DatabaseTest, AddDbFile) {
  db::Database &db = db::getDatabase();
  std::string name = "test";
  db::TupleDesc td;
  auto file = std::make_unique<db::DbFile>(name, td);
  auto expected = file.get();
  db.add(std::move(file));
  db::DbFile &actual = db.get(name);
  EXPECT_EQ(expected, &actual);
}

TEST(DatabaseTest, AddDbFileTwice) {
  db::Database &db = db::getDatabase();
  std::string name = "test";
  db::TupleDesc td;
  db.add(std::make_unique<db::DbFile>(name, td));
  EXPECT_ANY_THROW(db.add(std::make_unique<db::DbFile>(name, td)));
}

TEST(DatabaseTest, AddMultipleDbFiles) {
  db::Database &db = db::getDatabase();
  std::string name1 = "test1";
  std::string name2 = "test2";
  db::TupleDesc td;
  auto file1 = std::make_unique<db::DbFile>(name1, td);
  auto file2 = std::make_unique<db::DbFile>(name2, td);
  auto expected1 = file1.get();
  auto expected2 = file2.get();
  db.add(std::move(file1));
  db.add(std::move(file2));
  db::DbFile &actual1 = db.get(name1);
  EXPECT_EQ(&actual1, expected1);
  db::DbFile &actual2 = db.get(name2);
  EXPECT_EQ(&actual2, expected2);
}

TEST(DatabaseTest, GetNonexistentDbFile) {
  db::Database &db = db::getDatabase();
  std::string name = "file2";
  db::TupleDesc td;
  EXPECT_ANY_THROW(db.get(name));
  db.add(std::make_unique<db::DbFile>("file1", td));
  EXPECT_ANY_THROW(db.get(name));
}

TEST(DatabaseTest, RemoveDbFile) {
  db::Database &db = db::getDatabase();
  std::string name = "test";
  db::TupleDesc td;
  auto file = std::make_unique<db::DbFile>(name, td);
  auto expected = file.get();
  db.add(std::move(file));
  auto actual = db.remove(name);
  EXPECT_EQ(expected, actual.get());
  EXPECT_ANY_THROW(db.get(name));
}

TEST(DatabaseTest, RemoveNonexistentDbFile) {
  db::Database &db = db::getDatabase();
  EXPECT_ANY_THROW(db.remove("test"));
}

TEST(DatabaseTest, MultipleOperations) {
  db::Database &db = db::getDatabase();
  db::TupleDesc td;
  constexpr size_t size = 10;
  std::array<db::DbFile *, size> files{};
  std::array<std::string, size> names;
  for (size_t i = 0; i < size; i++) {
    names[i] = std::to_string(i);
    auto file = std::make_unique<db::DbFile>(names[i], td);
    files[i] = file.get();
    db.add(std::move(file));
  }
  for (size_t i = 0; i < size; i++) {
    db::DbFile &actual = db.get(names[i]);
    db::DbFile *expected = files[i];
    EXPECT_EQ(expected, &actual);
  }
  for (size_t i = 0; i < size; i++) {
    std::string name = std::to_string(i);
    EXPECT_ANY_THROW(db.add(std::make_unique<db::DbFile>(name, td)));
  }

  size_t test1 = 3;
  size_t test2 = 8;
  std::string name1 = std::to_string(test1);
  std::string name2 = std::to_string(test2);
  db::DbFile *expected;

  {
    auto actual = db.remove(name1);
    expected = files[test1];
    EXPECT_EQ(expected, actual.get());
    EXPECT_ANY_THROW(db.get(name1));
  }
  {
    auto actual = db.remove(name2);
    expected = files[test2];
    EXPECT_EQ(expected, actual.get());
    EXPECT_ANY_THROW(db.get(name2));
  }

  auto file = std::make_unique<db::DbFile>(name1, td);
  expected = file.get();
  db.add(std::move(file));
  EXPECT_EQ(expected, &db.get(name1));

  file = std::make_unique<db::DbFile>(name2, td);
  expected = file.get();
  db.add(std::move(file));
  EXPECT_EQ(expected, &db.get(name2));
}
