#include <vcd/recognizers/recognizer_factory.h>
#include <vcd/recognizers/fundamental.h>
#include <vcd/recognizers/combinators.h>
#include <vcd/recognizers/recognizer.h>

#include <memory>
#include <utility>
#include <string_view>

namespace vcd {

namespace {
  template <class T, class ...Args>
  recognizer create (Args ...args) {
    return recognizer(std::make_unique<T>(std::forward<Args>(args)...));
  }
} // ns anonymous

using namespace recognizers;

recognizer recognizer_factory::digit () {
  auto func = [](std::string_view view) { 
    return view.size() == 1 and view[0] >= '0' and view[0] <= '9';
  };
  return create<satisfying>(any(), func);
}

recognizer recognizer_factory::lowercase () {
  auto func = [](std::string_view view) { 
    return view.size() == 1 and view[0] >= 'a' and view[0] <= 'z';
  };
  return create<satisfying>(any(), func);
}

recognizer recognizer_factory::uppercase () {
  auto func = [](std::string_view view) { 
    return view.size() == 1 and view[0] >= 'A' and view[0] <= 'Z';
  };
  return create<satisfying>(any(), func);
}

recognizer recognizer_factory::identifier_char () {
  auto func = [](std::string_view view) { 
    return view.size() == 1 and view[0] >= '!' and view[0] <= '~';
  };
  return create<satisfying>(any(), func);
}

recognizer recognizer_factory::identifier () {
  return create<many>(identifier_char().impl().clone());
}

} // ns vcd
