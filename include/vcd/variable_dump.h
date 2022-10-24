#pragma once

#include <vcd/types.h>

#include <variant>
#include <string>
#include <vector>
#include <map>

namespace vcd {

class variable_dump {
public:
  // TODO: add other types
  using timestamp = unsigned long;

  variable_dump ();
  variable_dump (unsigned size);

  unsigned size ();

  void set_reference (std::string reference);
  void set_initial_value (types::value init);

  void add_change (timestamp time, types::value new_value);

  std::size_t num_changes () const;
private:
  unsigned size_;
  std::string identifier_;
  std::string reference_;

  types::value initial_value_;
  std::map<timestamp, types::value> value_changes_;
};

} // ns vcd
