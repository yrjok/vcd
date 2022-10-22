#include <vcd/parser/value_change_token.h>

#include <string>

namespace vcd {

value_change_token::value_change_token (std::string identifier,
    std::string value)
  : identifier_(std::move(identifier))
  , value_(std::move(value))
{ }

std::string const & value_change_token::identifier () const { return identifier_; }
std::string const & value_change_token::value () const { return value_; }

} // ns vcd
