#pragma once

#include <variant>
#include <string>
#include <vector>

namespace vcd {

class variable_dump {
public:
  // TODO: add other types
  using timestamp = unsigned long;
  using value = std::variant<bool, unsigned>;

  variable_dump ();
  variable_dump (std::string reference);

  variable_dump & with_initial_value (value init);
  variable_dump & with_change_at (timestamp time, value new_value);

  std::size_t num_changes () const;
private:

  struct value_change {
    value_change (timestamp t, value v);

    timestamp time;
    value new_value;

    std::strong_ordering operator<=> (value_change const & other) const;
  };

  std::string identifier_;
  std::string reference_;

  value initial_value_;
  std::vector<value_change> value_changes_;
};

} // ns vcd
