#pragma once

#include <db/Tuple.hpp>

namespace db {
class DbFile;

struct Iterator {
  const DbFile &file;
  size_t page;
  size_t slot;

public:
  Iterator(const DbFile &file, const size_t &page, size_t slot);
  ~Iterator() = default;
  Iterator(const Iterator &) = default;
  Iterator(Iterator &&) = default;

  Tuple operator*() const;

  Iterator &operator++();

  bool operator==(const Iterator &other) const { return page == other.page && slot == other.slot; }
  bool operator!=(const Iterator &) const = default;
};
} // namespace db