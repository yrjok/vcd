#pragma once

#include <vcd/types.h>
#include <vcd/variable_dump_view.h>

#include <variant>
#include <string>
#include <vector>
#include <map>

namespace vcd {

class variable_dump : public variable_dump_view {
public:
  variable_dump ();
  variable_dump (unsigned num_bits);

  unsigned num_changes () const override;
  unsigned num_bits () const override;

  types::value initial_value () const override;

  types::value at (types::timestamp time) const override;

  const_value_change_iterator begin () const override;
  const_value_change_iterator end () const override;

  void set_reference (std::string reference);
  void set_initial_value (types::value init);

  void add_change (types::timestamp time, types::value new_value);
  void add_change (types::value_change change);

private:
  types::value initial_value_;
  std::vector<types::value_change> value_changes_;
};

} // ns vcd
