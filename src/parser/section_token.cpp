#include <vcd/parser/section_token.h>

#include <string>

namespace vcd {

section_token::section_token (std::string type, std::string body)
  : type_(std::move(type))
  , body_(std::move(body))
{ }

std::string const & section_token::type () const { return type_; }
std::string const & section_token::body () const { return body_; }

} // ns vcd
