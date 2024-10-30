#pragma once

#include <db/DbFile.hpp>
#include <optional>
#include <vector>

namespace db {

/**
 * @brief The operation of a predicate.
 * @details The supported numeric comparison operations are:
 *   EQ (equal),
 *   NE (not equal),
 *   LT (less than),
 *   LE (less than or equal),
 *   GT (greater than),
 *   GE (greater than or equal).
 */
enum class PredicateOp { EQ, NE, LT, LE, GT, GE };

/**
 * @brief A predicate to filter rows.
 * @details A predicate is a comparison between a field and a value.
 *   The field is specified by the field_name.
 *   The op is the operation to perform.
 *   The value is the value to compare the field with.
 */
struct FilterPredicate {
  std::string field_name;
  PredicateOp op;
  field_t value;
};

/**
 * @brief A predicate to join two tables.
 * @details A join predicate is a comparison between a field of the left table and a field of the right table.
 *   The left field is specified by the left field name.
 *   The op is the operation to perform.
 *   The right field is specified by the right field name.
 */
struct JoinPredicate {
  std::string left;
  PredicateOp op;
  std::string right;
};

/**
 * @brief The operation of an aggregate.
 * @details The supported aggregate operations are:
 *   sum, average, minimum, maximum, and count.
 */
enum class AggregateOp { SUM, AVG, MIN, MAX, COUNT };

/**
 * @brief An aggregate operation to group and summarize rows.
 * @details An aggregate operation groups rows by a field and summarizes the values of another field.
 *   The group is the field to group by.
 *   The op is the operation to perform.
 *   The field is the field to summarize.
 */
struct Aggregate {
  std::optional<std::string> group;
  AggregateOp op;
  std::string field;
};

/**
 * @brief Perform a projection operation.
 * @details A projection operation selects a subset of fields from the input table.
 *   The field_names specify the fields to keep, in the order they should appear.
 *   The output table is stored in the out table.
 * @param in The input table.
 * @param out The output table.
 * @param field_names The fields to keep.
 */
void projection(const DbFile &in, DbFile &out, const std::vector<std::string> &field_names);

/**
 * @brief Perform a filter operation.
 * @details A filter operation selects rows that satisfy a set of predicates.
 *   The predicates are combined with a logical AND.
 *   The output table is stored in the out table.
 * @param in The input table.
 * @param out The output table.
 * @param pred The predicates to filter rows.
 */
void filter(const DbFile &in, DbFile &out, const std::vector<FilterPredicate> &pred);

/**
 * @brief Perform a join operation.
 * @details A join operation combines rows from two tables that satisfy the join predicates.
 *   The output table is stored in the out table.
 * @param left The left table.
 * @param right The right table.
 * @param out The output table.
 * @param pred The join predicates.
 * @note When performing an equality join do not keep the join field of the right table in the output.
 * @note Keep in mind that the bufferpool has a limited size.
 */
void join(const DbFile &left, const DbFile &right, DbFile &out, const JoinPredicate &pred);

/**
 * @brief Perform an aggregate operation.
 * @details An aggregate operation groups rows by a field and summarizes the values of another field.
 *   The output table is stored in the out table.
 *   If the group field is not specified, the aggregate is performed on all rows and returns a single tuple with one field.
 *   Otherwise, the aggregate is performed on each unique group and returns one tuple per group.
 * @param in The input table.
 * @param out The output table.
 * @param agg The aggregate operation.
 * @note The computed value should have the same type as the field being aggregated with the exception of AVG which should return a double.
 */
void aggregate(const DbFile &in, DbFile &out, const Aggregate &agg);

} // namespace db
