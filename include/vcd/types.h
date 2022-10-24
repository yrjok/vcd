#pragma once

#include <vector>
#include <string>

namespace vcd {

namespace types {

enum class bit { one, zero, high_z, undefined };

bit parse_bit (char c);

using value = std::vector<bit>;

value parse_value (std::string const s);

}

} // ns vcd
