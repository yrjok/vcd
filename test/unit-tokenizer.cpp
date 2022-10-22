#include <vcd/parser/section_token.h>
#include <vcd/parser/timestamp_token.h>
#include <vcd/parser/tokenizer.h>
#include <vcd/parser/value_change_token.h>

#include <doctest/doctest.h>

using namespace vcd;

TEST_SUITE("Tokenizer") {
  tokenizer t;

  TEST_CASE("Skips leading whitespace") {
    std::string const line("   \n  \t   $timescale 100 ns $end");
    auto tokens = t.tokenize(line);
    CHECK_EQ(tokens.size(), 1);
  }

  TEST_CASE("Throws in case of unexpected tokens") {
    std::string const line("$timescale 100 ns $end\n"
      "unexpected\n"
      "$comment comment $end");
    CHECK_THROWS_AS(t.tokenize(line), tokenizer::error);
  }


  TEST_CASE("Section tokens") {
    SUBCASE("Throws when section has invalid body") {
      std::string const line("$timescale$end");
      CHECK_THROWS_AS(t.tokenize(line), tokenizer::error);
    }

    SUBCASE("Timescale") {
      std::string const line("$timescale 100 ns $end");
      auto tokens = t.tokenize(line);
      CHECK_EQ(tokens.size(), 1);
      CHECK(tokens.front()->is<section_token>());
    }
  }

  TEST_CASE("Timestamp tokens") {
    std::string const line("#4321");
    auto tokens = t.tokenize(line);
    CHECK_EQ(tokens.size(), 1);
    CHECK(tokens.front()->is<timestamp_token>());
  }

  TEST_CASE("Value change tokens") {
    SUBCASE("Simple values") {
      std::string const line("0~");
      auto tokens = t.tokenize(line);
      CHECK_EQ(tokens.size(), 1);
      CHECK(tokens.front()->is<value_change_token>());
    }

    SUBCASE("Vectors") {
      std::string const line("b00101 ~");
      auto tokens = t.tokenize(line);
      CHECK_EQ(tokens.size(), 1);
      CHECK(tokens.front()->is<value_change_token>());
    }
  }
}
