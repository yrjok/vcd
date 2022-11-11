#include <doctest/doctest.h>

#include <vcd/recognizers/recognizer_factory.h>

#include <string>
#include <vector>

using namespace vcd;

namespace {
  std::string const digits("0123456789");
  std::string const lowercase("abcdefghijklmnopqrstuvwxyz");
  std::string const uppercase("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  std::string const letter = lowercase + uppercase;
  std::string const whitespace(" \t\r\n");
  std::string const identifier_chars = [] {
    std::string result;
    for (char c = '!'; c <= '~'; ++c) { result += c; }  
    return result;
  }();
} // ns anonymous

TEST_SUITE("Recognizer factory") {
  TEST_CASE("digit recognizer") {
    auto recognizer = recognizer_factory::digit();

    SUBCASE("Matches digits") {
      for (char c : digits) {
        CHECK(recognizer.matches(std::string_view(&c, 1)));
      }
    }

    SUBCASE("Does not match others") {
      for (char c : letter + whitespace) {
        auto match = recognizer.matches(std::string_view(&c, 1));
        CHECK(not match.has_value());
      }
    }
  }

  TEST_CASE("lowercase recognizer") {
    auto recognizer = recognizer_factory::lowercase();

    SUBCASE("Matches lowercase letters") {
      for (char c : lowercase) {
        CHECK(recognizer.matches(std::string_view(&c, 1)));
      }
    }

    SUBCASE("Does not match others") {
      for (char c : digits + uppercase + whitespace) {
        CHECK(not recognizer.matches(std::string_view(&c, 1)));
      }
    }
  }

  TEST_CASE("uppercase recognizer") {
    auto recognizer = recognizer_factory::uppercase();

    SUBCASE("Matches uppercase letters") {
      for (char c : uppercase) {
        CHECK(recognizer.matches(std::string_view(&c, 1)));
      }
    }

    SUBCASE("Does not match others") {
      for (char c : digits + lowercase + whitespace) {
        CHECK(not recognizer.matches(std::string_view(&c, 1)));
      }
    }
  }

  TEST_CASE("identifier char recognizer") {
    auto recognizer = recognizer_factory::identifier_char();

    SUBCASE("Matches identifier characters") {
      for (char c : identifier_chars) {
        CHECK(recognizer.matches(std::string_view(&c, 1)));
      }
    }

    SUBCASE("Does not match others") {
      for (char c : whitespace) {
        CHECK(not recognizer.matches(std::string_view(&c, 1)));
      }
    }
  }

  TEST_CASE("identifier") {
    auto recognizer = recognizer_factory::identifier();

    SUBCASE("Matches entire identifiers") {
      std::string const content("!&~ 0b10");
      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK(match.value() == "!&~");
    }
  }

  TEST_CASE("word") {
    auto recognizer = recognizer_factory::word();

    SUBCASE("Matches entire words") {
      std::string const content("Hello, world!");
      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK(match.value() == "Hello");
    }

    SUBCASE("Matches underscores in words") {
      std::string const content("valid_word");
      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK(match.value() == content);
    }
  }

  TEST_CASE("literal") {
    std::string const literal("OMG");
    auto recognizer = recognizer_factory::literal(literal);

    SUBCASE("Matches the literal") {
      std::string const content("OMG");
      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK_EQ(match.value(), content);
    }

    SUBCASE("Does not match anything trailing it") {
      std::string const content("OMGWHATISTHIS");
      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK_EQ(match.value(), "OMG");
    }
  }

  TEST_CASE("VCD value") {
    auto recognizer = recognizer_factory::value();

    SUBCASE("Matches single bit values") {
      std::vector<std::string> values {
        "0", "1", "X", "x", "Z", "z"
      };
      for (auto const & e : values) {
        auto match = recognizer.matches(e);
        CHECK(match.has_value());
        CHECK_EQ(match.value(), e);
      }
    }

    SUBCASE("Matches bit vector values") {
      std::vector<std::string> values {
        "b001101",
        "B0101",
        "b0",
        "B1"
      };
      for (auto const & e : values) {
        auto match = recognizer.matches(e);
        CHECK(match.has_value());
        CHECK_EQ(match.value(), e);
      }
    }
  }

  TEST_CASE("VCD keyword") {
    auto recognizer = recognizer_factory::keyword();

    SUBCASE("Matches only the keyword") {
      std::string const content("$date some txt xd $end");
      auto match = recognizer.matches(content);
      CHECK(match.has_value());
      CHECK_EQ(match.value(), "$date");
    }
  }

  TEST_CASE("VCD keyword end") {
    auto recognizer = recognizer_factory::keyword_end();

    SUBCASE("Matches only the keyword") {
      std::string const content("$end\n$date");
      auto match = recognizer.matches(content);
      CHECK(match.has_value());
      CHECK_EQ(match.value(), "$end");
    }

    SUBCASE("Does not match when $end is only part of the keyword") {
      std::string const content("$endsomething");
      auto match = recognizer.matches(content);
      CHECK(not match.has_value());
    }
  }

  TEST_CASE("VCD definition section") {
    auto recognizer = recognizer_factory::section();

    SUBCASE("Matches the entire section") {
      std::string const content("$var reg 1 ^ data $end");
      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK_EQ(match.value(), content);
    }

    SUBCASE("Does not match past a section") {
      std::string const content("$var reg 1 ^ data $end $end");
      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK_EQ(match.value(), "$var reg 1 ^ data $end");
    }
  }

  TEST_CASE("VCD timestamps") {
    auto recognizer = recognizer_factory::timestamp();

    SUBCASE("Matches the timestamp") {
      std::string const content("#5420");
      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK_EQ(match.value(), content);
    }
  }

  TEST_CASE("Value change") {
    auto recognizer = recognizer_factory::value_change();

    SUBCASE("Matches single bit value changes") {
      std::string const content("1*");

      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK_EQ(match.value(), content);
    }

    SUBCASE("Matches single bit vector value changes") {
      std::string const content("b0 !");

      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK_EQ(match.value(), content);
    }

    SUBCASE("Matches mutliple bit vector value changes") {
      std::string const content("b100001 #");

      auto match = recognizer.matches(content);
      REQUIRE(match.has_value());
      CHECK_EQ(match.value(), content);
    }
  }
}
