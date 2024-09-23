#include <db/DbFile.hpp>
#include <db/Iterator.hpp>

using namespace db;

Iterator::Iterator(const DbFile &file, const size_t &page, size_t slot) : file(file), page(page), slot(slot) {}

Tuple Iterator::operator*() const { return file.getTuple(*this); }

Iterator &Iterator::operator++() {
  file.next(*this);
  return *this;
}
