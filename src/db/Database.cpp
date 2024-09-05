#include <db/Database.hpp>

using namespace db;

BufferPool &Database::getBufferPool() { return bufferPool; }

Database &db::getDatabase() {
  static Database instance;
  return instance;
}

void Database::add(std::unique_ptr<DbFile> file) {
  // TODO pa1: add the file to the catalog. Note that the file must not exist.
}

std::unique_ptr<DbFile> Database::remove(const std::string &name) {
  // TODO pa1: remove the file from the catalog. Note that the file must exist.
}

DbFile &Database::get(const std::string &name) const {
  // TODO pa1: get the file from the catalog. Note that the file must exist.
}
