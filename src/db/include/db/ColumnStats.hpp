#pragma once

#include <db/Query.hpp>

namespace db {

/**
 * A class to represent a fixed-width histogram over a single integer-based field.
 */
class ColumnStats {
  // TODO pa4: add private members

public:
  /**
   * Create a new ColumnStats.
   *
   * This ColumnStats should maintain a histogram of integer values that it receives.
   * It should split the histogram into buckets.
   *
   * The values that are being processed will be provided one-at-a-time through the "addValue()" function.
   *
   * Your implementation should use constant space and have constant execution time with respect to
   * the number of values being added (i.e., you store the values that you add in the histogram).
   *
   * @param buckets The number of buckets to split the input range.
   * @param min The minimum integer value that this instance will process
   * @param max The maximum integer value that this instance will process
   */
  ColumnStats(unsigned buckets, int min, int max);

  /**
   * Add a value to the histogram.
   * @param v Value to add to the histogram
   */
  void addValue(int v);

  /**
   * Estimate the selectivity of a particular predicate and operand on this table.
   *
   * For example, if "op" is "GT" and "v" is 5,
   * return your estimate of the fraction of elements that are greater than 5.
   *
   * @param op Operator
   * @param v Value
   * @return Predicted selectivity of this particular operator and value
   */
  size_t estimateCardinality(PredicateOp op, int v) const;
};
} // namespace db
