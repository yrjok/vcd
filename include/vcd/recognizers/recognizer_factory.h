#pragma once

#include <vcd/recognizers/recognizer.h>

namespace vcd {

class recognizer_factory {
public:
  static recognizer digit ();
  static recognizer lowercase ();
  static recognizer uppercase ();
  static recognizer identifier_char ();

  static recognizer identifier ();
};

} // ns vcd
