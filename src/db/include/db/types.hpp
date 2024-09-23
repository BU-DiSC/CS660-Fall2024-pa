#pragma once

#include <array>
#include <string>
#include <utility>
#include <variant>

namespace db {
constexpr size_t INT_SIZE = sizeof(int);
constexpr size_t DOUBLE_SIZE = sizeof(double);
constexpr size_t CHAR_SIZE = 64;

enum class type_t { INT, CHAR, DOUBLE };

using field_t = std::variant<int, double, std::string>;

struct PageId {
  std::string file;
  size_t page;

public:
  bool operator==(const PageId &) const = default;
};

constexpr size_t DEFAULT_PAGE_SIZE = 4096;

using Page = std::array<uint8_t, DEFAULT_PAGE_SIZE>;
} // namespace db

template <> struct std::hash<const db::PageId> {
  std::size_t operator()(const db::PageId &r) const {
    return std::hash<std::string>()(r.file) ^ std::hash<size_t>()(r.page);
  }
};
