#pragma once

#include <db/types.hpp>
#include <unordered_map>
#include <vector>

namespace db {
class TupleDesc;

class Tuple {
  std::vector<field_t> fields;

public:
  Tuple(const std::vector<field_t> &fields);
  type_t field_type(size_t i) const;
  size_t size() const;
  const field_t &get_field(size_t i) const;
};

class TupleDesc {
  std::vector<type_t> types;
  std::vector<size_t> offsets;
  std::unordered_map<std::string, size_t> name_to_index;

public:
  TupleDesc() = default;
  /**
   * @brief Construct a new Tuple Desc object
   * @details Construct a new TupleDesc object with the provided types and names
   * @param names the names of the fields
   * @param types the types of the fields
   * @throws std::logic_error if types and names have different lengths
   * @throws std::logic_error if names are not unique
   */
  TupleDesc(const std::vector<type_t> &types, const std::vector<std::string> &names);

  /**
   * @brief Check if the provided Tuple is compatible with this TupleDesc
   * @details A Tuple is compatible with a TupleDesc if the Tuple has the same number of fields and each field is of the
   * same type as the corresponding field in the TupleDesc
   * @param tuple the Tuple to check
   * @return true if the Tuple is compatible, false otherwise
   */
  bool compatible(const Tuple &tuple) const;

  /**
   * @brief Get offset of the field
   * @details The offset of the field is the number of bytes from the start of the Tuple to the start of the field
   * @param index the index of the field
   * @return the offset of the field
   */
  size_t offset_of(const size_t &index) const;

  /**
   * @brief Get the index of the field
   * @details The index of the field is the position of the field in the Tuple
   * @param name the name of the field
   * @return the index of the field
   */
  size_t index_of(const std::string &name) const;

  /**
   * @brief Get the number of fields in the TupleDesc
   * @return the number of fields in the TupleDesc
   */
  size_t size() const;

  /**
   * @brief Get the length of the TupleDesc
   * @return the number of bytes needed to serialize a Tuple with this TupleDesc
   */
  size_t length() const;

  /**
   * @brief Serialize a Tuple
   * @param data the buffer to serialize the Tuple into
   * @param t the Tuple to serialize
   */
  void serialize(uint8_t *data, const Tuple &t) const;

  /**
   * @brief Deserialize a Tuple
   * @param data the buffer to deserialize the Tuple from
   * @return the deserialized Tuple
   */
  Tuple deserialize(const uint8_t *data) const;

  /**
   * @brief Merge two TupleDescs
   * @details The merged TupleDesc has all the fields of the two TupleDescs
   * @param td1 the first TupleDesc
   * @param td2 the second TupleDesc
   * @return the merged TupleDesc
   */
  static db::TupleDesc merge(const TupleDesc &td1, const TupleDesc &td2);
};
} // namespace db
