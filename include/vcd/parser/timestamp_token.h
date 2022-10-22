#pragma once

#include <vcd/parser/token.h>

namespace vcd {

class timestamp_token : public token {
public:
  timestamp_token (unsigned value);

  unsigned value () const;

private:
  unsigned value_;
};

} // ns vcd
