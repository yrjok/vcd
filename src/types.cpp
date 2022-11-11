#include <vcd/types.h>

#include <string>
#include <stdexcept>

namespace vcd {

namespace types {

timestamp::timestamp ()
  : timestamp(0)
{ }

timestamp::timestamp (unsigned long long value)
  : value_(value)
{ }

unsigned long long timestamp::value () const {
  return value_;
}

std::strong_ordering timestamp::operator<=> (timestamp const & other) const {
  return value_ <=> other.value_;
}

enum class bit_value : std::uint8_t {
  zero = 0,
  one = 1,
  high_z = 2,
  undefined = 3,
};

bit::bit ()
  : bit(static_cast<std::uint8_t>(bit_value::undefined))
{ }

bit::bit (std::uint8_t val)
  : val_(val)
{ }

bit bit::zero () {
  return bit(static_cast<std::uint8_t>(bit_value::zero));
}

bit bit::one () {
  return bit(static_cast<std::uint8_t>(bit_value::one));
}

bit bit::z () {
  return bit(static_cast<std::uint8_t>(bit_value::high_z));
}

bit bit::x () {
  return bit(static_cast<std::uint8_t>(bit_value::undefined));
}

bool bit::operator== (bit const & other) const {
  return other.val_ == val_;
}

bool bit::operator!= (bit const & other) const {
  return not operator==(other);
}

value::value ()
  : value(1)
{ }

value::value (std::size_t size)
  : bits_(size, bit::x())
{ }

std::size_t value::num_bits () const {
  return bits_.size();
}

bit const & value::at (std::size_t index) const {
  return bits_.at(index);
}

bit & value::at (std::size_t index) {
  return bits_.at(index);
}

} // ns types

} // ns vcd
