#include <vcd/parser/parser.h>
#include <vcd/parser/tokenizer.h>

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
    {
      section_token section("var", "wire 1 ! clk");
      p.parse_section(section);
    }
    {
      section_token section("var", "wire 1 ~ clk");
      p.parse_section(section);
    }

    {
      timestamp_token timestamp(1);
      p.parse_timestamp(timestamp);
    }

    p.parse_value_change(value_change_token("!", "0"));

    {
      timestamp_token timestamp(7);
      p.parse_timestamp(timestamp);
    }

    p.parse_value_change(value_change_token("!", "1"));
    p.parse_value_change(value_change_token("~", "1"));

    auto vcd = p.finalize();

    CHECK_EQ(vcd.variables["!"].num_changes(), 2);
    CHECK_EQ(vcd.variables["~"].num_changes(), 1);
  }

  TEST_CASE("Example file") {
    std::string content = R"($date
   Date text. For example: November 11, 2009.
$end
$version
   VCD generator tool version info text.
$end
$comment
   Any comment text.
$end
$timescale 1ps $end
$scope module logic $end
$var wire 8 # data $end
$var wire 1 $ data_valid $end
$var wire 1 % en $end
$var wire 1 & rx_en $end
$var wire 1 ' tx_en $end
$var wire 1 ( empty $end
$var wire 1 ) underrun $end
$upscope $end
$enddefinitions $end
$dumpvars
bxxxxxxxx #
x$
0%
x&
x'
1(
0)
$end
#0
b10000001 #
0$
1%
0&
1'
0(
0)
#2211
0'
#2296
1$
#2302
0$
#2303)";
    tokenizer t;
    parser p;

    for (auto & e : t.tokenize(content)) { p.consume(std::move(e)); }
    auto vcd = p.finalize();

    CHECK(vcd.variables.count("#"));
    CHECK(vcd.variables.count("$"));
    CHECK(vcd.variables.count("%"));
    CHECK(vcd.variables.count("&"));
    CHECK(vcd.variables.count("'"));
    CHECK(vcd.variables.count("("));
    CHECK(vcd.variables.count(")"));
    CHECK_EQ(vcd.variables["$"].num_changes(), 3);
  }
}
