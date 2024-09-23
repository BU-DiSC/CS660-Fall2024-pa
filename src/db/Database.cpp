#include <db/Database.hpp>

using namespace db;

BufferPool &Database::getBufferPool() { return bufferPool; }

Database &db::getDatabase() {
  static Database instance;
  return instance;
}

void Database::add(std::unique_ptr<DbFile> file) {
  const std::string &name = file->getName();
  if (files.contains(name)) {
    throw std::logic_error("File already exists");
  }
  files[name] = std::move(file);
}

std::unique_ptr<DbFile> Database::remove(const std::string &name) {
  auto nh = files.extract(name);
  if (nh.empty()) {
    throw std::logic_error("File does not exist");
  }
  Database::getBufferPool().flushFile(nh.key());
  return std::move(nh.mapped());
}

DbFile &Database::get(const std::string &name) const { return *files.at(name); }
