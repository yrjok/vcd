#include <vcd/variable_dump.h>

#include <vcd/types.h>

#include <stdexcept>
#include <string>

namespace vcd {

variable_dump::variable_dump ()
  : variable_dump(1)
{ }

variable_dump::variable_dump (unsigned num_bits)
  : initial_value_(num_bits)
{ }

unsigned variable_dump::num_bits () const {
  return initial_value_.num_bits();
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

  value_changes_.emplace(time, new_value);
}

std::size_t variable_dump::num_changes () const {
  return value_changes_.size();
}

auto variable_dump::events () const -> std::vector<event> {
  std::vector<event> result;
  for (auto const & kv : value_changes_) {
    result.push_back(event(kv.first, kv.second));
  }
  return result;
}

} // ns vcd
