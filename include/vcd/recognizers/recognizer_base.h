#pragma once

#include <memory>
#include <optional>
#include <string_view>

namespace vcd {

class recognizer_base {
public:
  using match = std::optional<std::string_view>;
  using pointer = std::unique_ptr<recognizer_base>;

  virtual ~recognizer_base () = default;

  virtual match matches (std::string_view content) const = 0;

  virtual pointer clone () const = 0;
};

} // ns vcd
