#pragma once

#include <vcd/parser/token.h>

#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace vcd {

class tokenizer {
public:
  class error : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  static std::vector<std::unique_ptr<token>> tokenize (std::string_view content);
};

} // ns vcd
