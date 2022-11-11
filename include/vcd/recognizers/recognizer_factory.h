#pragma once

#include <vcd/recognizers/recognizer.h>

namespace vcd {

class recognizer_factory {
public:
  static recognizer digit ();
  static recognizer lowercase ();
  static recognizer uppercase ();
  static recognizer letter ();
  // Matches a single whitespace character
  static recognizer whitespace ();
  static recognizer identifier_char ();

  // Any amount of identifier characters
  static recognizer identifier ();

  // Any amount of alfanumeric characters
  static recognizer word ();

  // Any amount of digits
  static recognizer number ();

  // Matches exactly the given value
  static recognizer literal (std::string const & value);

  static recognizer keyword ();
  static recognizer keyword_end ();

  // Recognizes a single VCD value bit (i.e '1', '0', 'x' or 'z')
  static recognizer value_bit ();
  static recognizer value_vector ();

  // Recognizes a VCD variable value
  static recognizer value ();

  // Recognizes a VCD definition section value
  static recognizer section ();

  // Recognizes a VCD timestamp
  static recognizer timestamp ();

  // Recognizes a VCD named variable value change
  static recognizer value_change ();
};

} // ns vcd
