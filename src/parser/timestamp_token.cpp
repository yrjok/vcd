#include <vcd/parser/timestamp_token.h>

namespace vcd {

timestamp_token::timestamp_token (unsigned value)
  : value_(value)
{ }

unsigned timestamp_token::value () const { return value_; }

} // ns vcd
