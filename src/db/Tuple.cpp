#include <cstring>
#include <db/Tuple.hpp>
#include <stdexcept>

using namespace db;

Tuple::Tuple(const std::vector<field_t> &fields) : fields(fields) {}

type_t Tuple::field_type(size_t i) const {
  const field_t &field = fields.at(i);
  if (std::holds_alternative<int>(field)) {
    return type_t::INT;
  }
  if (std::holds_alternative<double>(field)) {
    return type_t::DOUBLE;
  }
  if (std::holds_alternative<std::string>(field)) {
    return type_t::CHAR;
  }
  throw std::logic_error("Unknown field type");
}

size_t Tuple::size() const { return fields.size(); }

const field_t &Tuple::get_field(size_t i) const { return fields.at(i); }

TupleDesc::TupleDesc(const std::vector<type_t> &types, const std::vector<std::string> &names) : types(types) {
  if (types.size() != names.size()) {
    throw std::logic_error("Types and names sizes do not match");
  }
  size_t offset = 0;
  for (size_t i = 0; i < types.size(); i++) {
    offsets.push_back(offset);
    name_to_index[names[i]] = i;
    switch (types[i]) {
    case type_t::INT:
      offset += INT_SIZE;
      break;
    case type_t::DOUBLE:
      offset += DOUBLE_SIZE;
      break;
    case type_t::CHAR:
      offset += CHAR_SIZE;
      break;
    }
  }
  if (name_to_index.size() != names.size()) {
    throw std::logic_error("Duplicate name");
  }
}

bool TupleDesc::compatible(const Tuple &tuple) const {
  if (tuple.size() != types.size()) {
    return false;
  }

  for (size_t i = 0; i < tuple.size(); i++) {
    if (tuple.field_type(i) != types[i]) {
      return false;
    }
  }

  return true;
}

size_t TupleDesc::offset_of(const size_t &index) const { return offsets.at(index); }

size_t TupleDesc::index_of(const std::string &name) const { return name_to_index.at(name); }

size_t TupleDesc::length() const {
  size_t length = 0;
  for (type_t type : types) {
    switch (type) {
    case type_t::INT:
      length += INT_SIZE;
      break;
    case type_t::DOUBLE:
      length += DOUBLE_SIZE;
      break;
    case type_t::CHAR:
      length += CHAR_SIZE;
      break;
    }
  }
  return length;
}

size_t TupleDesc::size() const { return types.size(); }

Tuple TupleDesc::deserialize(const uint8_t *data) const {
  std::vector<field_t> fields;
  fields.reserve(types.size());
  for (const type_t &type : types) {
    switch (type) {
    case type_t::INT:
      fields.emplace_back(*reinterpret_cast<const int *>(data));
      data += INT_SIZE;
      break;
    case type_t::DOUBLE:
      fields.emplace_back(*reinterpret_cast<const double *>(data));
      data += DOUBLE_SIZE;
      break;
    case type_t::CHAR:
      fields.emplace_back(std::string(reinterpret_cast<const char *>(data)));
      data += CHAR_SIZE;
      break;
    }
  }
  return {fields};
}

void TupleDesc::serialize(uint8_t *data, const Tuple &t) const {
  for (size_t i = 0; i < types.size(); i++) {
    const type_t &type = types[i];
    const field_t &field = t.get_field(i);
    switch (type) {
    case type_t::INT:
      *reinterpret_cast<int *>(data) = std::get<int>(field);
      data += INT_SIZE;
      break;
    case type_t::DOUBLE:
      *reinterpret_cast<double *>(data) = std::get<double>(field);
      data += DOUBLE_SIZE;
      break;
    case type_t::CHAR:
      strncpy(reinterpret_cast<char *>(data), std::get<std::string>(field).c_str(), CHAR_SIZE);
      data += CHAR_SIZE;
      break;
    }
  }
}

db::TupleDesc TupleDesc::merge(const TupleDesc &td1, const TupleDesc &td2) {
  std::vector<type_t> types(td1.types);
  types.insert(types.end(), td2.types.begin(), td2.types.end());
  std::vector<std::string> names(types.size());
  for (const auto &[name, index] : td1.name_to_index) {
    names[index] = name;
  }
  for (const auto &[name, index] : td2.name_to_index) {
    names[td1.size() + index] = name;
  }
  return {types, names};
}
