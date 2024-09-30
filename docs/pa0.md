# Programming Assignment 0

## Implementation Guide

### Database

The `Database` consists of a collection of the `DbFiles` that are currently in the database and a BufferPool (a
collection of pages currently in memory). This class should support adding a new file, as well as retrieve information
about stored files. The Database class is partially implemented.

You can get access to the global `Database` by calling the `db::getDatabase()` method.

### PageId and Page

The `PageId` class represents a unique identifier for a page in the database. It consists of a `file` and a `page`.
The `file` is a unique identifier for the file in the catalog, and the `page` is the number of the page in the file.

The `Page` class represents a single page of data in the database. It is a fixed-size block of memory that is read from
and written to disk. The page is identified by a `PageId`, which is a unique identifier for the page. The contents of
the page are stored in a `std::array` of `DEFAULT_PAGE_SIZE` bytes. The page is stored in a binary format, so it can be
read and written to disk using the `DbFile` interface.

### DbFile

The `DbFile` interface provides methods to read and write pages to and from disk. For this assignment, you do not need
to implement this interface. Instead, we have provided you an empty implementation for testing purposes. You will need
to use the `DbFile::readPage()` and `DbFile::writePage()` methods to implement the `BufferPool` methods.

### BufferPool

The `BufferPool` is responsible for caching pages in memory that have been recently read from disk. All operators read
and write pages from various files on disk through the buffer pool. It consists of a fixed number of pages, defined by
the `DEFAILT_NUM_PAGES` constant. You will implement an eviction policy and the `BufferPool::getPage()` method used by
as well as some helper methods. You will be required to implement a LRU (least recently used) eviction policy to pass
all the test cases.

The `Database` class provides a method, `Database::getBufferPool()`, that returns a reference to the buffer pool.

Here are some snippets of code that you may find useful:

```cpp
Database &db = getDatabase();

PageId pageId{file, page};

// get a page from the buffer pool (this will read the page from disk if it is not already in memory)
Page &page = db.getBufferPool().getPage(pageId);

// get a DbFile from the catalog
DbFile &file = db.get(pageId.file);

// read a page from the disk
file.readPage(page, pageId.page);

// write a page to the disk
file.writePage(page, pageId.page);
```

## Exercises

Add any private members needed and implement the methods as indicated by the `// TODO pa1.x` comments in the following
files:

### void Database::add(std::unique_ptr<DbFile> file)

Adds the `file` to the catalog. Associate the given `file` with its name.

### std::unique_ptr<DbFile> Database::remove(const std::string &name)

Remove the file associated with the given `name` from the catalog and return it. The buffer pool should flush any dirty
pages associated with the file before it is removed, otherwise the buffer pool will not be able to retrieve the file
when it eventually tries to evict them.

### DbFile &Database::get(const std::string &name) const

Return the file associated with the given `name`.

### BufferPool::BufferPool()

Initialize any private members necessary to keep track of the pages in the buffer pool. The pages are stored in a
`std::array` of size `DEFAULT_NUM_PAGES`. Initially, all the pages are available for use. As the application requests
pages, they are stored in available slots of this array. Some of these pages may be marked as dirty, indicating that
they have been modified and will need to be written back to disk when evicted.

### BufferPool::~BufferPool()

When the buffer pool is destroyed, it should ensure that all the dirty pages will be written back to the disk.

### Page &BufferPool::getPage(const PageId &pid)

This method should return a reference to the page with the given `pid`. If the page is not in the buffer pool, it should
be read from disk. If the buffer pool is full, a page should be evicted to accommodate this request. Specifically, the
LRU page should be evicted. To achieve this, you should keep track of the order in which pages are accessed. If the page
chosen for eviction is dirty, it should be written back to disk before being evicted.

Mark the requested page as the most recently used page.

### void BufferPool::markDirty(const PageId &pid)

Mark the page with the given `pid` as dirty.

### bool BufferPool::isDirty(const PageId &pid) const

Return `true` if the page with the given `pid` is dirty.

### bool BufferPool::contains(const PageId &pid) const

Return `true` if the page with the given `pid` is in the buffer pool.

### void BufferPool::discardPage(const PageId &pid)

Remove the page with the given `pid` from the buffer pool. Do not write the page back to disk.

### void BufferPool::flushPage(const PageId &pid)

Write the page with the given `pid` back to disk.

### void BufferPool::flushFile(const std::string &name)

Write any dirty pages associated with the file with the given `name` back to disk.

### Questions

1. Evicting a dirty page is more expensive than evicting a clean page. Propose a strategy to reduce the number of dirty pages that are evicted.

2. The `Database::remove` flushes any dirty pages associated with the file before it is removed. What are the implications of leaving the pages in the buffer pool when a file is removed? Can you identify a possible bug?

3. When would you need to discard a page from the buffer pool without writing it back to disk?

You will be submitting your answers to these questions <u>**directly on Gradescope**</u>. The link for submission will be released early next week.

### Grading

- <u>**60% of your grade**</u> will be based on whether your code passes the tests when we run it. These tests will be a superset
  of the tests we have provided. Before handing in your code, you should make sure your code produces no errors.

- For testing, we will use `ctest` with our version of the tests. This means you cannot change the format of binary files.
  You should also not change our API. You should test that your code compiles the unmodified tests. Try to add code only
  where we have indicated.

- <u>**30% of your grade**</u> will be based on your answer to the technical questions posted above. **You must follow instructions posted above to submit your answers.**

- <u>**10% of your grade</u>** will be based on the quality of your writeup and our subjective evaluation of your code.

We hope you enjoy hacking on this assignment!
