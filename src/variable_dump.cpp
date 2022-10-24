#include <vcd/variable_dump.h>

#include <cassert>
#include <string>
#include <stdexcept>

namespace vcd {

variable_dump::variable_dump ()
  : variable_dump(1)
{ }

variable_dump::variable_dump (unsigned size)
  : size_(size)
{ }

void variable_dump::set_initial_value (types::value init) {
  assert(init.size() == size_);
  initial_value_ = init;
}
void variable_dump::add_change (timestamp time,
    types::value new_value) {
  assert(new_value.size() == size_);

  value_changes_.emplace(time, new_value);
}

std::size_t variable_dump::num_changes () const {
  return value_changes_.size();
}

} // ns vcd
