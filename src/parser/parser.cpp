#include <vcd/parser/parser.h>

#include <vcd/parser/token.h>
#include <vcd/parser/section_token.h>
#include <vcd/parser/value_change_token.h>
#include <vcd/variable_dump.h>

#include <iostream>

#include <string>
#include <regex>
#include <vector>
#include <memory>
#include <utility>

namespace vcd {

namespace {
  inline constexpr std::string c_whitespace(" \t\r\n");

  std::string pop_word (std::string & s) {
    auto pos = s.find_first_of(c_whitespace);
    auto result = s.substr(0, pos);
    s = s.substr(pos);
    return result;
  }

  void trim_left (std::string & s) {
    auto pos = s.find_first_not_of(c_whitespace);
    s = s.substr(pos);
  }
}

parser::parser ()
  : header_done_(false)
{ }

value_change_dump parser::finalize () {
  return std::move(temp_);
}

void parser::consume (std::unique_ptr<token> next_token) {
  if (next_token->is<section_token>()) {
    if (header_done_) {
      throw std::runtime_error(
        "Received unexpected header section token.");
    }
    parse_section(next_token->as<section_token>());
  } else if (next_token->is<value_change_token>()) {
    header_done_ = true;
    parse_value_change(next_token->as<value_change_token>());

  } else if (next_token->is<timestamp_token>()) {
    parse_timestamp(next_token->as<timestamp_token>());
  } else {
    std::logic_error("Unexpected token type");
  }
}

void parser::parse_section (section_token & section) {
  if (section.type() == "var") {
    std::string body(section.body());
    auto var_type = pop_word(body);
    trim_left(body);
    auto var_width = pop_word(body);
    trim_left(body);
    auto identifier = pop_word(body);
    trim_left(body);
    auto reference = body;
    temp_.variables[identifier] = variable_dump(std::move(reference));
  } else {
    std::cout << "Skipping unknown section type "
      << section.type() << std::endl;
  }
}

void parser::parse_timestamp (timestamp_token & timestamp) {
  temp_.current_time = timestamp.value();
}

void parser::parse_value_change (value_change_token value_change) {
  if (temp_.variables.count(value_change.identifier()) == 0) {
    throw std::runtime_error("No variable registered with given id");
  }

  if (value_change.value().size() == 1) {
    bool new_value = value_change.value() == "1";
    temp_.variables.at(value_change.identifier())
      .with_change_at(temp_.current_time, new_value);
  } else {
    throw std::runtime_error("TODO");
  }
}


} // ns vcd
