#pragma once

#include <vcd/variable_dump.h>

#include <map>
#include <string>
#include <vector>

namespace vcd {

struct value_change_dump {
  std::map<std::string, variable_dump> variables;
};

} // ns vcd
