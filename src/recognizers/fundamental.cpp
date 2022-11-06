#include <vcd/recognizers/fundamental.h>

#include <optional>
#include <functional>
#include <utility>

namespace vcd {

namespace recognizers {

auto empty::matches (std::string_view const content) const -> match {
  return content.empty() ? match(content) : std::nullopt;
}

std::unique_ptr<recognizer_base> empty::clone () const {
  return std::make_unique<empty>();
}

auto any::matches (std::string_view const content) const -> match {
  return content.empty() ? std::nullopt : match(content.substr(0, 1));
}

std::unique_ptr<recognizer_base> any::clone () const {
  return std::make_unique<any>();
}

satisfying::satisfying (std::unique_ptr<recognizer_base> other, predicate pred)
  : other_(std::move(other))
  , pred_(std::move(pred))
{ }

satisfying::satisfying (recognizer_base const & other, predicate pred)
  : other_(other.clone())
  , pred_(std::move(pred))
{ }

auto satisfying::matches (std::string_view const content) const -> match {
  auto result = other_->matches(content);
  if (result.has_value() and pred_(result.value())) {
    return result;
  }
  return std::nullopt;
}

std::unique_ptr<recognizer_base> satisfying::clone () const {
  return std::make_unique<satisfying>(other_->clone(), pred_);
}

} // ns recognizers

} // ns vcd

