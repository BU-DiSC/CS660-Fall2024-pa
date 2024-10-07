#include <db/DbFile.hpp>
#include <stdexcept>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace db;

const TupleDesc &DbFile::getTupleDesc() const { return td; }

DbFile::DbFile(const std::string &name, const TupleDesc &td) : name(name), td(td) {
  fd = open(name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd == -1) {
    throw std::runtime_error("open");
  }
  struct stat st{};
  if (fstat(fd, &st) == -1) {
    throw std::runtime_error("fstat");
  }
  numPages = st.st_size / DEFAULT_PAGE_SIZE;
  if (numPages == 0) {
    numPages = 1;
  }
}

DbFile::~DbFile() {
  close(fd);
}

const std::string &DbFile::getName() const { return name; }

void DbFile::readPage(Page &page, const size_t id) const {
  reads.push_back(id);
  pread(fd, page.data(), DEFAULT_PAGE_SIZE, id * DEFAULT_PAGE_SIZE);
}

void DbFile::writePage(const Page &page, const size_t id) const {
  writes.push_back(id);
  pwrite(fd, page.data(), DEFAULT_PAGE_SIZE, id * DEFAULT_PAGE_SIZE);
}

const std::vector<size_t> &DbFile::getReads() const { return reads; }

const std::vector<size_t> &DbFile::getWrites() const { return writes; }

void DbFile::insertTuple(const Tuple &t) { throw std::runtime_error("Not implemented"); }

void DbFile::deleteTuple(const Iterator &it) { throw std::runtime_error("Not implemented"); }

Tuple DbFile::getTuple(const Iterator &it) const { throw std::runtime_error("Not implemented"); }

void DbFile::next(Iterator &it) const { throw std::runtime_error("Not implemented"); }

Iterator DbFile::begin() const { throw std::runtime_error("Not implemented"); }

Iterator DbFile::end() const { throw std::runtime_error("Not implemented"); }

size_t DbFile::getNumPages() const { return numPages; }
