#include <vcd/variable_dump.h>

#include <vcd/types.h>

#include <algorithm>
#include <stdexcept>
#include <string>

namespace vcd {

variable_dump::variable_dump ()
  : variable_dump(1)
{ }

variable_dump::variable_dump (unsigned num_bits)
  : initial_value_(num_bits)
{ }

unsigned variable_dump::num_changes () const {
  return value_changes_.size();
}

unsigned variable_dump::num_bits () const {
  return initial_value_.num_bits();
}

types::value variable_dump::initial_value () const {
  return initial_value_;
}

types::value variable_dump::at (types::timestamp time) const {
  auto it = std::find_if(value_changes_.begin(), value_changes_.end(),
    [=](auto const & e) { return e.t > time; });
  if (it == value_changes_.begin()) {
    return initial_value_;
  }
  if (it == value_changes_.end()) {
    return value_changes_.back().v;
  }
  return std::prev(it)->v;
}

const_value_change_iterator variable_dump::begin () const {
  return const_value_change_iterator(value_changes_, 0);
}

const_value_change_iterator variable_dump::end () const {
  return const_value_change_iterator(value_changes_, value_changes_.size());
}

void variable_dump::set_initial_value (types::value init) {
  if (init.num_bits() != initial_value_.num_bits()) {
    throw std::runtime_error(std::string("Got ") +
        std::to_string(init.num_bits()) + " bit value but expected " +
        std::to_string(initial_value_.num_bits()));
  }
  initial_value_ = init;
}

void variable_dump::add_change (types::timestamp time,
    types::value new_value) {
  if (new_value.num_bits() != initial_value_.num_bits()) {
    throw std::runtime_error(std::string("Got ") +
        std::to_string(new_value.num_bits()) + " bit value but expected " +
        std::to_string(initial_value_.num_bits()));
  }

  if (not value_changes_.empty()) {
    if (time <= value_changes_.back().t) {
      throw std::runtime_error("Can not add value changes in the past");
    }
  }

  value_changes_.push_back({time, new_value});
}

void variable_dump::add_change (types::value_change change) {
  add_change(change.t, change.v);
}

} // ns vcd
