#include <doctest/doctest.h>

#include <vcd/types.h>
#include <vcd/variable_dump.h>

using namespace vcd;

namespace {
  types::value value_from_str (std::string const & s) {
    types::value result(s.size());
    for (unsigned i = 0; i < s.size(); ++i) {
      char c = s.at(i);
      if (c == '0') {
        result.at(i) = types::bit::zero();
      } else if (c == '1') {
        result.at(i) = types::bit::one();
      } else if (c == 'x') {
        result.at(i) = types::bit::x();
      } else if (c == 'z') {
        result.at(i) = types::bit::z();
      }
    }
    return result;
  }

  types::value initial_value () {
   return value_from_str("xxxx");
  }

  std::vector<types::timestamp> const & timestamps () {
    static std::vector<types::timestamp> const c_times {
      types::timestamp(10),
      types::timestamp(14),
      types::timestamp(18),
      types::timestamp(22),
      types::timestamp(30)
    };
    return c_times;
  }

  std::vector<types::value> const & values () {
    static std::vector<types::value> const c_values {
      value_from_str("0001"),
      value_from_str("0010"),
      value_from_str("0011"),
      value_from_str("0100"),
      value_from_str("0101")
    };
    return c_values;
  }

  variable_dump get_test_dump () {
    variable_dump dump(4);
    dump.set_initial_value(initial_value());

    for (std::size_t i = 0; i < values().size(); ++i) {
      dump.add_change({ timestamps().at(i), values().at(i) });
    }
    return dump;
  }
} // ns anonymous

TEST_SUITE("Owning variable dumps") {
  auto dump = get_test_dump();

  TEST_CASE("Indexing with timestamps") {
    SUBCASE("before first change yield initial value") {
      CHECK_EQ(dump.at(types::timestamp(2)), initial_value());
    }

    SUBCASE("between changes yields most recent value") {
      CHECK_EQ(dump.at(types::timestamp(20)), value_from_str("0011"));
    }

    SUBCASE("after last change yields last value") {
      CHECK_EQ(dump.at(types::timestamp(3000)), value_from_str("0101"));
    }
  }

  TEST_CASE("Iterating through changes") {
    unsigned index = 0;
    for (auto it = dump.begin(); it != dump.end(); ++it) {
      CHECK_EQ(it->t, timestamps().at(index));
      CHECK_EQ(it->v, values().at(index));
      ++index;
    }

  }
}

TEST_SUITE("Non-owning variable dump views") {
}
