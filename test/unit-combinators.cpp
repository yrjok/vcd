#include <doctest/doctest.h>

#include <vcd/recognizers/fundamental.h>
#include <vcd/recognizers/combinators.h>

#include <string>
#include <vector>

using namespace vcd;

TEST_SUITE("Combinators") {
  TEST_CASE("Either") {
    recognizers::any any;
    recognizers::satisfying first(any.clone(), [](auto v) { return v == "a"; });
    recognizers::satisfying second(any.clone(), [](auto v) { return v == "b"; });

    recognizers::either recognizer(first.clone(), second.clone());

    SUBCASE("Matches using the first recognizer") {
      std::string content("abcd");
      REQUIRE(first.matches(content));
      REQUIRE(not second.matches(content));
      CHECK(recognizer.matches(content));
    }

    SUBCASE("Matches using the second recognizer") {
      std::string content("bcd");
      REQUIRE(not first.matches(content));
      REQUIRE(second.matches(content));
      CHECK(recognizer.matches(content));
    }

    SUBCASE("Does not match anything else") {
      std::string content("cd");
      REQUIRE(not first.matches(content));
      REQUIRE(not second.matches(content));
      CHECK(not recognizer.matches(content));
    }
  }

  TEST_CASE("Sequence") {
    recognizers::any any;
    recognizers::satisfying first(any.clone(), [](auto v) { return v == "a"; });
    recognizers::satisfying second(any.clone(), [](auto v) { return v == "b"; });

    recognizers::sequence recognizer(first.clone(), second.clone());

    SUBCASE("Matches sequences") {
      std::string content("ab");
      REQUIRE(first.matches(content));
      REQUIRE(second.matches(content.substr(1, 1)));
      CHECK(recognizer.matches(content));
    }

    SUBCASE("Does not match part of the sequence") {
      std::string content("ac");
      REQUIRE(first.matches(content));
      REQUIRE(not second.matches(content.substr(1, 1)));
      CHECK(not recognizer.matches(content));
    }

    SUBCASE("Does not match anything else") {
      std::string content("def");
      REQUIRE(not first.matches(content));
      REQUIRE(not second.matches(content.substr(1, 1)));
      CHECK(not recognizer.matches(content));
    }
  }

  TEST_CASE("Many") {
    recognizers::any any;
    recognizers::satisfying first(any.clone(), [](auto v) { return v == "y"; });
    recognizers::many recognizer(first.clone());

    SUBCASE("Matches a sequences of as many matching parts as possible") {
      std::string content("yyyyyba");
      REQUIRE(first.matches(content));
      auto match = recognizer.matches(content);
      CHECK(match.has_value());
      CHECK(match.value().size() == 5);
    }

    SUBCASE("Matches only the first uninterrupted sequence.") {
      std::string content("yyy yyyyyba");
      REQUIRE(first.matches(content));
      auto match = recognizer.matches(content);
      CHECK(match.has_value());
      CHECK(match.value().size() == 3);
    }
  }

  TEST_CASE("Negate") {
    recognizers::any any;
    recognizers::satisfying negated(any.clone(), [](auto v) { return v == "a"; });
    recognizers::negate recognizer(negated);

    SUBCASE("Returns size one match if the negated recognizer fails.") {
      std::string content("sheep");
      REQUIRE(not negated.matches(content));
      auto match = recognizer.matches(content);
      CHECK(match.has_value());
      CHECK(match.value().size() == 1);
    }

    SUBCASE("Does not match if the negated recognizer succeeds.") {
      std::string content("abba");
      REQUIRE(negated.matches(content));
      auto match = recognizer.matches(content);
      CHECK(not match.has_value());
    }
  }
}
