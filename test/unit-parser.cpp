#include <vcd/parser/parser.h>

#include <doctest/doctest.h>

using namespace vcd;

TEST_SUITE("Parser") {
  parser p;

  TEST_CASE("parses variable sections") {
    auto section = section_token("var", "wire 1 ! clk");
    p.parse_section(section);
    auto vcd = p.finalize();

    CHECK_EQ(vcd.variables.size(), 1);
    CHECK_EQ(vcd.variables.count("!"), 1);
  }

  TEST_CASE("parses timestamps") {
    constexpr unsigned c_time = 42;
    p.consume(std::make_unique<timestamp_token>(c_time));
    auto vcd = p.finalize();
    CHECK_EQ(vcd.current_time, c_time);
  }

  TEST_CASE("parses value changes") {
    auto section = section_token("var", "wire 1 ! clk");
    p.parse_section(section);
    auto timestamp = timestamp_token(1);
    p.parse_timestamp(timestamp);
    p.parse_value_change(value_change_token("!", "0"));
    auto vcd = p.finalize();

    CHECK_EQ(vcd.variables["!"].num_changes(), 1);
  }
}
