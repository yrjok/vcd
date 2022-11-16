#include <vcd/parser/parser.h>

#include <doctest/doctest.h>

using namespace vcd;

TEST_SUITE("Parser") {
  parser p;

  TEST_CASE("parses variable sections") {
    std::string section("$var wire 8 # data $end");
    auto result = p.parse(section);
    REQUIRE_EQ(result.variables.size(), 1);
    CHECK_EQ(result.variables.begin()->first, "#");
    CHECK_EQ(result.variables.begin()->second.num_bits(), 8);
  }

  TEST_CASE("parses value changes") {
    std::string content("$var wire 4 ! data $end\n#1023\nb011x !\n");
    auto result = p.parse(content);
    REQUIRE_EQ(result.variables.size(), 1);
    CHECK_EQ(result.variables.begin()->first, "!");
    auto const & variable = result.variables.begin()->second;
    CHECK_EQ(variable.num_bits(), 4);
    CHECK_EQ(variable.num_changes(), 1);
    CHECK_EQ(variable.begin()->t, types::timestamp(1023));
    auto val = variable.begin()->v; 
    CHECK_EQ(val.at(0), types::bit::zero());
    CHECK_EQ(val.at(1), types::bit::one());
    CHECK_EQ(val.at(2), types::bit::one());
    CHECK_EQ(val.at(3), types::bit::x());
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
    parser p;

    auto dump = p.parse(content);
    CHECK_EQ(dump.variables.size(), 7);
    CHECK(dump.variables.count("#"));
    CHECK_EQ(dump.variables.at("#").num_changes(), 1);
    CHECK_EQ(dump.variables.at("#").num_bits(), 8);
  }
}
