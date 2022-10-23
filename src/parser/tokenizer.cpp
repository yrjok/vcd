#include <vcd/parser/tokenizer.h>

#include <vcd/parser/section_token.h>
#include <vcd/parser/timestamp_token.h>
#include <vcd/parser/token.h>
#include <vcd/parser/value_change_token.h>

#include <iostream>
#include <cassert>
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

  std::unique_ptr<section_token> pop_section_token ();
  std::unique_ptr<timestamp_token> pop_timestamp_token ();
  std::unique_ptr<value_change_token> pop_value_token ();

  std::string_view content_;
};

namespace {
  inline constexpr std::string c_whitespace(" \t\n\r");
  inline constexpr std::string c_end_tag("$end");
} // ns anonymous

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
  unsigned num_skipped = content_.find_first_not_of(c_whitespace);
  if (num_skipped != std::string_view::npos) {
    content_.remove_prefix(num_skipped);
  } else {
    content_ = std::string_view();
  }
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

  std::cout << type << std::endl;
  std::cout << "'" << body << "'" << std::endl;
  content_.remove_prefix(end_tag_pos + c_end_tag.size());
  return std::make_unique<section_token>(std::move(type), std::move(body));
}

std::unique_ptr<timestamp_token> tokenizer_impl::pop_timestamp_token () {
  auto word = pop_word();
  auto body = word.substr(1);
  return std::make_unique<timestamp_token>(std::stoull(body));
}

std::unique_ptr<value_change_token> tokenizer_impl::pop_value_token () {
  std::string value = pop_word();
  std::string identifier;
  if (value.front() == 'r' or value.front() == 'b') {
    skip_whitespace();
    identifier = pop_word();
  } else if (value.front() == 'x' or value.front() == 'z' or
      value.front() == '1' or value.front() == '0') {
    assert(value.size() == 2);
    identifier = value.substr(1);
    value = value.substr(0, 1);
  } else {
    throw tokenizer::error(std::string("Unexpected token: ") + value);
  }

  return std::make_unique<value_change_token>(std::move(value),
      std::move(identifier));
}

std::string tokenizer_impl::pop_word () {
  assert(not content_.empty());
  assert(c_whitespace.find(content_.front()) == std::string::npos);

  auto end_of_word = content_.find_first_of(c_whitespace);
  std::string result(content_.substr(0, end_of_word));
  content_.remove_prefix(result.size());
  return result;
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
