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
- Label the file as **writeup.md** or **writeup.txt** (we require the writeup to either be a markdown or txt file <u>**only**</u>).

### Collaboration

This assignment should be manageable for a single person, but if you prefer to work with a partner, this is also OK
(which is the default setup in our class). Larger groups are not allowed. Please indicate clearly who you worked with,
if anyone, on your individual writeup.

### Submitting your assignment

Submit only the files that are provided with this assignment (only the [src](../src) directory is required).

### Submitting a bug

Please submit bug reports to the TF.

### Grading

Your grade will be based on multiple components:

- Code
- Answer to technical questions
- Demo (<u>\*only for certain assignments</u>)
- Documentation

Please refer to the rubric attached with every assignment for the exact weightage for each component.

We hope you enjoy hacking on this assignment!
