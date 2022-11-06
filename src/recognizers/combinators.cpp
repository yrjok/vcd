#include <vcd/recognizers/combinators.h>

#include <cassert>
#include <functional>

namespace vcd {

namespace recognizers {

either::either (pointer first, pointer second)
  : first_(std::move(first))
  , second_(std::move(second))
{ }

auto either::matches (std::string_view const content) const -> match {
  return match_first(content)
    .or_else(std::bind(&either::match_second, this, content));
}

auto either::clone () const -> pointer {
  return std::make_unique<either>(first_->clone(), second_->clone());
}

auto either::match_first (std::string_view const content) const -> match {
  return first_->matches(content);
}

auto either::match_second (std::string_view const content) const -> match {
  return second_->matches(content);
}

sequence::sequence (pointer first, pointer second)
  : first_(std::move(first))
  , second_(std::move(second))
{ }

auto sequence::matches (std::string_view const content) const -> match {
  auto first_match = match_first(content);
  if (first_match) {
    auto remainder = content;
    remainder.remove_prefix(first_match->size());
    auto second_match = match_second(remainder);
    if (second_match) {
      return content.substr(0, first_match->size() + second_match->size());
    }
  }
  return std::nullopt;
}

auto sequence::clone () const -> pointer {
  return std::make_unique<sequence>(first_->clone(), second_->clone());
}

auto sequence::match_first (std::string_view content) const -> match {
  return first_->matches(content);
}

auto sequence::match_second (std::string_view content) const -> match {
  return second_->matches(content);
}

many::many (pointer elem)
  : elem_(std::move(elem))
{ }

auto many::matches (std::string_view const content) const -> match {
  std::size_t total_size = 0;
  auto remainder = content;
  while (auto matched_element = elem_->matches(remainder)) {
    assert(matched_element->size() > 0);
    total_size += matched_element->size();
    remainder.remove_prefix(matched_element->size());
  }
  if (total_size > 0) {
    return content.substr(0, total_size);
  } else {
    return std::nullopt;
  }
}

auto many::clone () const -> pointer {
  return std::make_unique<many>(elem_->clone());
}

} // ns recognizers

} // ns vcd
