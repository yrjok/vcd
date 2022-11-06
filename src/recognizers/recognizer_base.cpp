#include <vcd/recognizer/recognizer_base.h>

#include <string_view>

namespace vcd {

namespace recognizer {

recognizer::recognizer (std::unique_ptr<recognizer_base> impl)
  : impl_(std::move(impl)
{ }

auto recognizer::matches (std::string_view const content) const {
  return impl_->matches(content);
}

} // ns recognizer

} // ns vcd
