#include <vcd/variable_dump.h>

#include <string>
#include <stdexcept>

namespace vcd {

variable_dump::variable_dump ()
  : variable_dump("")
{ }

variable_dump::variable_dump (std::string reference)
  : reference_(std::move(reference))
{ }

variable_dump & variable_dump::with_initial_value (value init) {
  initial_value_ = init;
  return *this;
}
variable_dump & variable_dump::with_change_at (timestamp time,
    value new_value) {
  if (not value_changes_.empty() and
      time <= value_changes_.back().time) {
    throw std::runtime_error("Can not go backwards in time.");
  }

  value_changes_.emplace_back(time, new_value);
  return *this;
}

variable_dump::value_change::value_change (timestamp t, value v)
  : time(t)
  , new_value(v)
{ }

std::strong_ordering variable_dump::value_change::operator<=> (
    value_change const & other) const {
  return time <=> other.time;
}

std::size_t variable_dump::num_changes () const {
  return value_changes_.size();
}

} // ns vcd
