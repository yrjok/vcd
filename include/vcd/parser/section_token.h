#pragma once

#include <vcd/parser/token.h>

#include <string>

namespace vcd {

class section_token : public token {
public:
  section_token (std::string type, std::string content);

  std::string const & type () const;
  std::string const & body () const;

private:
  std::string type_;
  std::string body_;
};

} // ns vcd
