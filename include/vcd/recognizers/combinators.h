#pragma once

#include <vcd/recognizers/recognizer_base.h>

#include <memory>
#include <string_view>

namespace vcd {

namespace recognizers {

class either : public recognizer_base {
public:
  either (pointer first, pointer second);

  match matches (std::string_view content) const override;

  pointer clone () const override;

private:
  match match_first (std::string_view content) const;
  match match_second (std::string_view content) const;

  pointer first_;
  pointer second_;
};

class sequence : public recognizer_base {
public:
  sequence (pointer first, pointer second);

  match matches (std::string_view content) const override;

  pointer clone () const override;

private:
  match match_first (std::string_view content) const;
  match match_second (std::string_view content) const;

  pointer first_;
  pointer second_;
};

class many : public recognizer_base {
public:
  many (pointer elem);

  match matches (std::string_view content) const override;

  pointer clone () const override;

private:
  pointer elem_;
};

} // ns recognizers

} // ns vcd
