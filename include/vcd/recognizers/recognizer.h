#pragma once

#include <memory>
#include <optional>
#include <string_view>

namespace vcd {

// Forward declarations
class recognizer_base;

class recognizer {
public:
  // Helper types
  using match = std::optional<std::string_view>;

  // Functions
  recognizer (std::unique_ptr<recognizer_base> impl);
  recognizer (recognizer const & other);
  recognizer (recognizer && other);
  recognizer & operator=(recognizer const & other);
  recognizer & operator=(recognizer && other);
  ~recognizer ();

  match matches (std::string_view const content) const;

  recognizer_base const & impl () const;
private:
  // Members
  std::unique_ptr<recognizer_base> impl_;
};

} // ns vcd
