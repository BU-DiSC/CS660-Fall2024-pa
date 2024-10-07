# Programming Assignment 1

In this assignment, we will implement a file that can store and retrieve tuples from disk. Each page of the
file will store some metadata (header) and the data (tuples). Each `Tuple` is a collection of fixed length fields (i.e.
`INT`, `DOUBLE`, `CHAR(64)`).

## TupleDesc

The `TupleDesc` class represents the schema of a tuple. The schema of a tuple
is a list of field types and names.

### TupleDesc::TupleDesc

The constructor for `TupleDesc` creates a new schema with the given field types
and names. It is up to you to decide what other computations you want to do here.

### TupleDesc::compatible

This method checks if a tuple is compatible with the schema (i.e. all fields have
the expected type).

### TupleDesc::offset_of

This method returns the offset of a field in a tuple.

### TupleDesc::index_of

This method returns the index of a field in a tuple.

### TupleDesc::length

This method returns the length of a tuple (total number of bytes).

### TupleDesc::size

This method returns the number of fields in a tuple.

### TupleDesc::deserialize

This method deserializes a tuple from a byte array (read the contents of a byte array to create a Tuple object).

### TupleDesc::serialize

This method serializes a tuple to a byte array (write the contents of a Tuple object to a byte array).

### TupleDesc::merge

This method merges two `TupleDesc`. This will be useful later when we will need to join two tables.
The fields of the new `TupleDesc` will be the fields of the first `TupleDesc` followed by the fields of the second
`TupleDesc`.

## DbFile

This is the base class for all database files. It provides the basic
functionality for reading and writing pages to and from disk.

### DbFile::DbFile

The constructor for `DbFile` opens the file with the given file name. If the
file does not exist, it is created a new file with the given file name.
The constructor counts the number of pages in the file and stores the
number of pages in the `numPages` field. A file should always have one page
when it is created (even if it is empty).

### DbFile::~DbFile

The destructor for `DbFile` closes the file.

### DbFile::readPage

The `readPage` method reads the page with the given page number from the file.

### DbFile::writePage

The `writePage` method writes the page with the given page number to the file.

## HeapFile

A `HeapFile` stores tuples in no particular order. The file is divided into pages and each page stores a fixed number of
tuples along with a header that indicates which tuples are populated.

### HeapFile::insertTuple

The `insertTuple` method inserts a tuple into the last page of the file. If there is no space in the last page, a new
page is created.

### HeapFile::deleteTuple

The `deleteTuple` method deletes the tuple point to by the provided iterator by marking the tuple as deleted in the
header of the page. The tuple does not have to be overwritten but it is good practice to erase its data (remember that
the whole page will be written to disk so the overhead of erasing the tuple's data is minimal).

### HeapFile::getTuple

The `getTuple` method returns a tuple that corresponds to the provided iterator. The tuple is deserialized from the
page.

### HeapFile::next

The `next` method advances the iterator to the next populated tuple. This tuple may be in a subsequent page. If there
are no more tuples, the iterator is set to the end of the file.

### HeapFile::begin

The `begin` method returns an iterator to the first populated tuple in the file. This might not be in the first page of
the file.

### HeapFile::end

The `end` method returns an iterator to the end of the file.

## HeapPage

The `HeapPage` class represents a page in a `HeapFile`. It is a wrapper of the `Page` type, meaning that it does not
store any data itself, but it provides an interface to read and write to and from the page.
Specifically, it provides access to the header the data and the capacity of the page.

### HeapPage::HeapPage

The constructor for `HeapPage` initializes the capacity of the page and points the header and data to the correct
offsets in the page buffer.

Given a page of $P$ bytes and tuples of length $T$ bytes
a heap page can store $C$ tuples. For each tuple we will need one extra bit to indicate whether tuple is
used.

$C = \lfloor \frac{P * 8}{T * 8 + 1} \rfloor$

Let's have a look at an example: a page of $4096$ bytes ($4096 * 8$ bits) can store $31$ tuples of $132$
bytes ($132 * 8 + 1$ header bit). Which means that $4$ bytes will be left unused ($4096 - 31 * 132 = 4$).

The header bits will be grouped together in the first bytes of the page, followed by any unused bytes (padding).

The data will be stored in the remaining bytes of the page.

To summarize:

- The header points to the beginning of the page (zero offset in the page buffer).
- The header is followed by any unused bytes (padding).
- The data begin after the padding (at offset $P - T * C$).

### HeapPage::begin

The `begin` method returns the index to the first populated tuple in the page. This might not be in the first slot of
the page. If there are no populated tuples, return an index which indicates the end of the page.

### HeapPage::end

The `end` method returns the index to the end of the page (capacity).

### HeapPage::insertTuple

The `insertTuple` method inserts a tuple into the first available slot of the page.

### HeapPage::deleteTuple

The `deleteTuple` method deletes the tuple pointed to by the provided index by marking the tuple as deleted in the
header of the page.

### HeapPage::getTuple

The `getTuple` method returns a tuple that corresponds to the provided index.

### HeapPage::next

The `next` method advances the index to the next populated tuple. If there are no more tuples, the index is set to the
end of the page.

### HeapPage::empty

The `empty` method checks if the page is empty (i.e. no populated tuples).

### Questions

1. Deleting tuples from a `HeapFile` might lead to having empty pages. How can we avoid having empty pages in a file?
   What is the cost of the solution you propose?

2. In this assignment we have fixed size fields. How can we support variable size fields (e.g. `VARCHAR`)?

You will be submitting your answers to these questions <u>**directly on Gradescope**</u>.

### Grading

- <u>**60% of your grade**</u> will be based on whether your code passes the tests when we run it. These tests will be a
  superset
  of the tests we have provided. Before handing in your code, you should make sure your code produces no errors.

- For testing, we will use `ctest` with our version of the tests. This means you cannot change the format of binary
  files.
  You should also not change our API. You should test that your code compiles the unmodified tests. Try to add code only
  where we have indicated.

- <u>**30% of your grade**</u> will be based on your answer to the technical questions posted above. **You must follow
  instructions posted above to submit your answers.**

- <u>**10% of your grade</u>** will be based on the quality of your writeup and our subjective evaluation of your code.

We hope you enjoy hacking on this assignment!
