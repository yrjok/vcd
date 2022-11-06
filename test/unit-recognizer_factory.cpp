#include <doctest/doctest.h>

#include <vcd/recognizers/recognizer_factory.h>

#include <string>
#include <iostream>

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
}
