#pragma once

#include <vcd/vcd_header.h>
#include <vcd/variable_dump.h>

#include <map>
#include <string>
#include <vector>

namespace vcd {

struct value_change_dump {
public:
  vcd_header header;

  unsigned long long current_time;

  std::map<std::string, variable_dump> variables;
};

} // ns vcd
