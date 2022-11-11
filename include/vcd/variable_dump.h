#pragma once

#include <vcd/types.h>

#include <variant>
#include <string>
#include <vector>
#include <map>

namespace vcd {

class variable_dump {
public:
  variable_dump ();
  variable_dump (unsigned num_bits);

  unsigned num_bits () const;

  void set_reference (std::string reference);
  void set_initial_value (types::value init);

  void add_change (types::timestamp time, types::value new_value);

  std::size_t num_changes () const;

  using event = std::pair<types::timestamp, types::value>;
  std::vector<event> events () const;
private:
  std::string identifier_;
  std::string reference_;

  types::value initial_value_;
  std::map<types::timestamp, types::value> value_changes_;
};

} // ns vcd
