#pragma once

#include <vcd/parser/token.h>

#include <string>

namespace vcd {

class value_change_token : public token {
public:
  value_change_token (std::string identifier, std::string value);

  std::string const & identifier () const;
  std::string const & value () const;

private:
  std::string identifier_;
  std::string value_;
};

} // ns vcd
