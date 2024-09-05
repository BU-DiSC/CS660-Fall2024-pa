#include <db/Database.hpp>
#include <db/DbFile.hpp>

using namespace db;

DbFile::DbFile(const std::string &name) : name(name) {}

const std::string &DbFile::getName() const { return name; }

void DbFile::readPage(Page &page, const size_t id) const { reads.push_back(id); }

void DbFile::writePage(const Page &page, const size_t id) const { writes.push_back(id); }

const std::vector<size_t> &DbFile::getReads() const { return reads; }

const std::vector<size_t> &DbFile::getWrites() const { return writes; }
