#include <vcd/types.h>

#include <string>
#include <stdexcept>

namespace vcd {

namespace types {

bit parse_bit (char c) {
  switch (c) {
    case '0':
      return bit::zero;
    case '1':
      return bit::one;
    case 'x':
      return bit::undefined;
    case 'z':
      return bit::high_z;
    default:
      throw std::invalid_argument("Invalid bit character.");
  }
}

value parse_value (std::string const s) {
  std::vector<bit> result;
  if (s.size() == 1) {
    result.push_back(parse_bit(s.front()));
  } else {
    for (auto it = std::next(s.begin()); it != s.end(); ++it) {
      result.push_back(parse_bit(*it));
    }
  }
  return result;
}

} // ns types

} // ns vcd
