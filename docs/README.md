# Introduction

## Database Architecture and Implementation Guide

Our database consists of:

- A catalog that stores information about available files and their schemas.
- Database files that consist of data pages.
- A buffer pool that caches recently used pages in memory.
- Predicates and conditions that can be applied to tuples.
- A collection of operators (e.g., select, join, insert, delete, etc.) that process tuples;

## Logistics

You must submit your code (see below) **as well as** a short (1-2 pages) writeup describing your approach. This writeup
should:

- Describe any design decisions you made.
- Describe any missing or incomplete elements of your code.
- Answer the analytical questions.
- Describe how long you spent on the assignment, and whether there was anything you found particularly difficult or
  confusing.
- If you collaborate with someone else, you have to describe how you split the workload.

### Collaboration

This assignment should be manageable for a single person, but if you prefer to work with a partner, this is also OK
(which is the default setup in our class). Larger groups are not allowed. Please indicate clearly who you worked with,
if anyone, on your individual writeup.

### Submitting your assignment

Submit only the files that are provided with this assignment (only the [src](../src) directory is required).

### Submitting a bug

Please submit bug reports to the TF.

### Grading

80% of your grade will be based on whether your code passes the tests when we run it. These tests will be a superset
of the tests we have provided. Before handing in your code, you should make sure your code produces no errors.

For testing, we will use `ctest` with our version of the tests. This means you cannot change the format of binary files.
You should also not change our API. You should test that your code compiles the unmodified tests. Try to add code only
where we have indicated.

An additional 20% of your grade will be based on the quality of your writeup and our subjective evaluation of your code.

We hope you enjoy hacking on this assignment!
