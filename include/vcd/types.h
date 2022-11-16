#pragma once

#include <vector>
#include <string>

namespace vcd {

namespace types {

class timestamp {
public:
  timestamp ();
  timestamp (unsigned long long value);

  unsigned long long value () const;

  std::strong_ordering operator<=> (timestamp const & other) const;
  bool operator== (timestamp const & other) const;
  bool operator!= (timestamp const & other) const;

private:
  unsigned long long value_;
};

class bit {
public:
  bit ();

  static bit zero ();
  static bit one ();
  static bit z ();
  static bit x ();

  bool operator== (bit const & other) const;
  bool operator!= (bit const & other) const;

private:
  bit (std::uint8_t val);

  std::uint8_t val_;
};

class value {
public:
  value ();
  value (std::size_t num_bits);

  std::size_t num_bits () const;

  bit const & at (std::size_t index) const;
  bit       & at (std::size_t index);

private:
  std::vector<bit> bits_;
};

bool operator== (value const & a, value const & b);
bool operator!= (value const & a, value const & b);

struct value_change {
  timestamp t;
  value v;
};

} // ns types

} // ns vcd
