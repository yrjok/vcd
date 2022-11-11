#include <vcd/recognizers/recognizer_factory.h>
#include <vcd/recognizers/fundamental.h>
#include <vcd/recognizers/combinators.h>
#include <vcd/recognizers/recognizer.h>

#include <cassert>
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

recognizer recognizer_factory::letter () {
  return create<either>(lowercase().impl().clone(), uppercase().impl().clone());
}

recognizer recognizer_factory::whitespace () {
  return create<either>(
    create<either>(literal(" "), literal("\t")),
    create<either>(literal("\r"), literal("\n"))
  );
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

recognizer recognizer_factory::word () {
  return create<many>(either(literal("_"), either(letter(), digit())));
}

recognizer recognizer_factory::number () {
  return create<many>(digit());
}

recognizer recognizer_factory::literal (std::string const & value) {
  auto literal_char = [](char c) {
    return [=](std::string_view v) { return v.size() == 1 and v[0] == c; };
  };

  assert(not value.empty());
  auto result = create<satisfying>(any(), literal_char(value.at(0)));

  for (auto i = 1; i < value.size(); ++i) {
    result = create<sequence>(
      result,
      create<satisfying>(any(), literal_char(value.at(i)))
    );
  }

  return result;
}

recognizer recognizer_factory::keyword () {
  return create<sequence>(literal("$"), word());
}

recognizer recognizer_factory::keyword_end () {
  // Don't use 'literal' recognizer here since it would also match
  // "$enddefinitions"
  auto func = [](std::string_view view) { return view == "$end"; };
  return create<satisfying>(keyword().impl().clone(), func);
}

recognizer recognizer_factory::value_bit () {
  return create<either>(
    create<either>(
      create<either>(literal("0"), literal("1")),
      create<either>(literal("x"), literal("X"))
    ),
    create<either>(literal("z"), literal("Z"))
  );
}

recognizer recognizer_factory::value_vector () {
  return create<sequence>(
    create<either>(literal("b"), literal("B")),
    create<many>(value_bit())
  );
}

recognizer recognizer_factory::value () {
  return create<either>(value_bit(), value_vector());
}

recognizer recognizer_factory::section () {
  return create<sequence>(
    keyword(),
    create<sequence>(
      create<many>(create<negate>(keyword_end())),
      keyword_end()
    )
  );
}

recognizer recognizer_factory::timestamp () {
  return create<sequence>(literal("#"), number());
}

recognizer recognizer_factory::value_change () {
  return create<either>(
    create<sequence>(value_vector(), create<sequence>(whitespace(), identifier())),
    create<sequence>(value_bit(), identifier())
  );
}

} // ns vcd
