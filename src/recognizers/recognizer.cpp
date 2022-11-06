#include <vcd/recognizers/recognizer.h>

#include <vcd/recognizers/recognizer_base.h>

#include <utility>

namespace vcd {

recognizer::recognizer (std::unique_ptr<recognizer_base> impl)
  : impl_(std::move(impl))
{ }

recognizer::recognizer (recognizer_base const & impl)
  : recognizer(impl.clone())
{ }

recognizer::recognizer (recognizer const & other)
  : impl_(other.impl_->clone())
{ }

recognizer::recognizer (recognizer && other)
  : impl_(std::move(other.impl_))
{ }

recognizer & recognizer::operator=(recognizer const & other) {
  impl_ = other.impl_->clone();
  return *this;
}

recognizer & recognizer::operator=(recognizer && other) {
  using std::swap;
  swap(impl_, other.impl_);
  return *this;
}

recognizer::~recognizer () { }

auto recognizer::matches (std::string_view const content) const -> match {
  return impl_->matches(content);
}

recognizer_base const & recognizer::impl () const {
  return *impl_;
}

} // ns vcd
