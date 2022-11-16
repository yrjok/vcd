#pragma once

#include <vcd/types.h>
#include <vcd/value_change_iterator.h>

namespace vcd {

class variable_dump_view {
public:
  virtual ~variable_dump_view () = default;

  virtual unsigned num_changes () const = 0;
  virtual unsigned num_bits () const = 0;

  virtual types::value initial_value () const = 0;

  virtual types::value at (types::timestamp time) const = 0;

  virtual const_value_change_iterator begin () const = 0;
  virtual const_value_change_iterator end () const = 0;
};  

} // ns vcd
