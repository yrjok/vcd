#pragma once

#include <vcd/recognizers/recognizer_base.h>

#include <string_view>
#include <functional>

namespace vcd {

namespace recognizers {

class empty : public recognizer_base {
public:
  match matches (std::string_view const content) const override;

  std::unique_ptr<recognizer_base> clone () const override;
};

class any : public recognizer_base {
public:
  match matches (std::string_view const content) const override;

  std::unique_ptr<recognizer_base> clone () const override;
};

class satisfying : public recognizer_base {
public:
  using predicate = std::function<bool(std::string_view)>;

  satisfying (std::unique_ptr<recognizer_base> other, predicate pred);
  satisfying (recognizer_base const & other, predicate pred);

  match matches (std::string_view const content) const override;

  std::unique_ptr<recognizer_base> clone () const override;

private:
  std::unique_ptr<recognizer_base> other_;
  predicate pred_;
};

} // ns recognizers

} // ns vcd
