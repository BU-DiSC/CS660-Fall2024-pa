# Programming Assignment 3

In this assignment, you will write a set of operators to implement table projections, selections, joins, and aggregates.
These will provide you with a database system that can perform simple queries over multiple tables.

## Projection

The `Project` operator is projecting a subset of fields from the tuples of a file.
Given an input `DbFile`, an output `DbFile`, and a mapping between the fields of the tuple descriptors,
the `Project` operator will insert into the output file all the tuples from the input file, but with only the
specified fields.

## FilterPredicate

A `FilterPredicate` describes a comparison between two numeric fields. The supported operators are `EQUALS`, `NOT_EQUALS`,
`GREATER_THAN`, `LESS_THAN`, `LESS_THAN_OR_EQ`, and `GREATER_THAN_OR_EQ`.

## Filter

The `filter` operation is used to filter out tuples that do not satisfy a given a collection of `FilterPredicate`
operators. This operator will take in an input `DbFile` and an output `DbFile`, and will insert into the output
file all the tuples from the input file that satisfy all the predicates.

## Aggregate

The `aggregate` operations implement basic SQL aggregates with a GROUP BY clause. You should implement the five SQL
aggregates (`COUNT`, `SUM`, `AVG`, `MIN`, `MAX`) and support grouping. You only need to support aggregates over a single
field, and grouping by a single field. If there is no grouping, the aggregate should return a single tuple with one field.

## JoinPredicate

A `JoinPredicate` describes a comparison between two fields of two tuples.

## Join

The `join` operation is used for joining tuples from two input files. The join operation will take in two input `DbFile`
objects and an output `DbFile` object. The join operation will insert into the output file all the tuples that are the
result of joining the tuples from the two input files that satisfy the join predicate. If the join predicate operator is
`EQ`, the operation will perform ignore the joining field of the second file (to avoid repetition of equal values). 

## Questions

1. An ambitious student tries to implement an OR clause by executing two `filter` operations with the same input and
   output files, but with different predicates. Discuss what is the problem with this approach and how you could fix it.
2. The `aggregate` operation supports grouping by a single field. Discuss how you could extend this to support grouping by
   multiple fields. How can you implement the HAVING clause?
3. What is the complexity of your join operation? How could you improve it?
