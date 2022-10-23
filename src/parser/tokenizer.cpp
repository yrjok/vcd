#include <vcd/parser/tokenizer.h>

#include <vcd/parser/section_token.h>
#include <vcd/parser/timestamp_token.h>
#include <vcd/parser/token.h>
#include <vcd/parser/value_change_token.h>

#include <iostream>
#include <cassert>
#include <regex>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace vcd {

struct tokenizer_impl {
  tokenizer_impl (std::string_view content);

  std::unique_ptr<token> next_token ();

  void skip_whitespace ();
  std::string pop_word ();
  std::string_view pop_line ();

  std::unique_ptr<section_token> pop_section_token ();
  std::unique_ptr<timestamp_token> pop_timestamp_token ();
  std::unique_ptr<value_change_token> pop_value_token ();

  std::string_view content_;
};

namespace {
  inline constexpr std::string c_whitespace(" \t\n\r");
  inline constexpr std::string c_end_tag("$end");
} // ns anonymous
  //

tokenizer_impl::tokenizer_impl (std::string_view content)
  : content_(std::move(content))
{ }

std::unique_ptr<token> tokenizer_impl::next_token () {
  std::unique_ptr<token> result;


  while (not content_.empty() and result == nullptr) {
    skip_whitespace();
    switch (content_.front()) {
      case '$':
        result = pop_section_token();
        break;
      case '#':
        result = pop_timestamp_token();
        break;
      default:
        result = pop_value_token();
    }
  }
  return result;
}

void tokenizer_impl::skip_whitespace () {
  auto end_of_whitespace = content_.find_first_not_of(c_whitespace);
  content_ = content_.substr(end_of_whitespace);
}

std::unique_ptr<section_token> tokenizer_impl::pop_section_token () {
  auto end_tag_pos = content_.find(c_end_tag);
  if (end_tag_pos == std::string_view::npos) {
    throw tokenizer::error("Counld't find matching $end tag");
  }
  
  std::string type;
  std::string body;
  auto type_end = content_.find_first_of(c_whitespace);
  if (type_end > end_tag_pos or type_end == std::string_view::npos) {
    type = content_.substr(1, end_tag_pos);
  } else {
    type = content_.substr(1, type_end - 1);
    auto body_begin = type_end + 1;
    body = content_.substr(body_begin, end_tag_pos - body_begin - 1);
  }

  content_.remove_prefix(end_tag_pos + c_end_tag.size());
  return std::make_unique<section_token>(std::move(type), std::move(body));
}

std::unique_ptr<timestamp_token> tokenizer_impl::pop_timestamp_token () {
  auto word = pop_word();
  auto body = word.substr(1);
  return std::make_unique<timestamp_token>(std::stoull(body));
}

std::unique_ptr<value_change_token> tokenizer_impl::pop_value_token () {
  std::regex const simple_value_pattern("^[01zx][!-~]+");
  std::regex const vector_value_pattern("^[rbB][\\dzx]+\\s[!-~]+");

  std::string line(pop_line());

  std::smatch match;
  if (std::regex_match(line, match, simple_value_pattern)) {
    std::string value(line.begin(), std::next(line.begin()));
    std::string identifier(std::next(line.begin()), line.end());
    content_.remove_prefix(line.size());
    return std::make_unique<value_change_token>(std::move(value),
      std::move(identifier));
  } else if (std::regex_match(line, match, vector_value_pattern)) {
    auto separator_pos = line.find_first_of(c_whitespace);
    std::string value = line.substr(0, separator_pos);
    std::string identifier = line.substr(separator_pos + 1);
    content_.remove_prefix(line.size());
    return std::make_unique<value_change_token>(std::move(value),
      std::move(identifier));
  } else {
    throw tokenizer::error("Unexpected value change definition.");
  }
}

std::string tokenizer_impl::pop_word () {
  assert(not content_.empty());
  assert(c_whitespace.find(content_.front()) == std::string::npos);

  auto end_of_word = content_.find_first_of(c_whitespace);
  std::string result(content_.substr(0, end_of_word));
  content_.remove_prefix(result.size());
  return result;
}

std::string_view tokenizer_impl::pop_line () {
  auto end_of_line = content_.find('\n');      
  return content_.substr(0, end_of_line);
}

std::vector<std::unique_ptr<token>> tokenizer::tokenize (std::string_view content) {
  std::vector<std::unique_ptr<token>> tokens;
  tokenizer_impl impl(std::move(content));
  for (auto t = impl.next_token(); t != nullptr; t = impl.next_token()) {
    tokens.push_back(std::move(t));
  }
  return tokens;
}

} // ns vcd
