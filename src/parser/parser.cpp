#include <vcd/parser/parser.h>

#include <vcd/recognizers/recognizer_factory.h>

#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <utility>

namespace vcd {

namespace {

std::string_view strip_whitespace (std::string_view text) {
  auto recognizer = recognizer_factory::whitespace();
  while (recognizer.matches(text)) {
    text.remove_prefix(1);
  }
  return text;
}

std::string pop_word (std::string_view text) {
  auto recognizer = recognizer_factory::identifier();
  return std::string(recognizer.matches(text).value_or(""));
}

inline constexpr auto identity = [](auto x) { return x; };

} // ns anonymous

parser::parser ()
{ }

auto parser::try_parse_section (std::string_view text) const -> std::optional<variable_proto> {
  auto variable_section = recognizer_factory::literal("$var").matches(text);

  if (variable_section.has_value()) {
    text.remove_prefix(variable_section.value().size());
    text = strip_whitespace(text);

    auto type = pop_word(text);
    if (not (type == "wire" or type == "reg")) {
      // Ignore other types for now
      return std::nullopt;
    }

    text.remove_prefix(type.size());
    text = strip_whitespace(text);

    auto size_str = pop_word(text);
    text.remove_prefix(size_str.size());
    text = strip_whitespace(text);
    unsigned size = std::stoul(size_str);

    auto id = pop_word(text);
    text.remove_prefix(id.size());
    text = strip_whitespace(text);

    auto reference = pop_word(text);

    return variable_proto {
      .id = id,
      .reference = reference,
      .num_bits = size
    };
  }
  return std::nullopt;
}

auto parser::try_parse_timestamp (std::string_view text) const -> std::optional<types::timestamp> {
  return types::timestamp(std::stoull(std::string(text.substr(1))));
}

auto parser::try_parse_value_change (std::string_view text) const
    -> std::optional<value_change_proto> {
  auto vec = recognizer_factory::value_vector().matches(text);

  std::string bits;
  if (vec) {
    auto w = pop_word(text);
    assert(not w.empty());
    if (w.front() != 'b' and w.front() != 'B') {
      throw std::runtime_error("Only binary values are supported.");
    }
    bits = w.substr(1);
    text.remove_prefix(w.size());
    text = strip_whitespace(text);
  } else {
    bits = text.substr(0, 1);
    text.remove_prefix(1);
  }

  auto new_value = types::value(bits.size());
  for (std::size_t i = 0; i < bits.size(); ++i) {
    switch (bits.at(i)) {
    case '0':
      new_value.at(i) = types::bit::zero();
      break;
    case '1':
      new_value.at(i) = types::bit::one();
      break;
    case 'x':
      [[ fallthrough ]];
    case 'X':
      new_value.at(i) = types::bit::x();
      break;
    case 'z':
      [[ fallthrough ]];
    case 'Z':
      new_value.at(i) = types::bit::z();
      break;
    default:
      throw std::runtime_error("Unexpected bit value");
    }
  }

  std::string id(text);
  value_change_proto  result;
  result.id = text;
  result.new_value = new_value;
  return result;
}

value_change_dump parser::parse (std::string_view content) const {
  auto section = recognizer_factory::section();
  auto timestamp = recognizer_factory::timestamp();
  auto value_change = recognizer_factory::value_change();
  auto whitespace = recognizer_factory::whitespace();

  vcd_builder builder;
  while (not content.empty()) {
    std::optional<std::string_view> match;
    if (match = section.matches(content)) {
      if (auto proto = try_parse_section(match.value())) {
        builder.dump.variables[proto->id] = variable_dump(proto->num_bits);
      }
    } else if (match = timestamp.matches(content)) {
      if (auto timestamp = try_parse_timestamp(match.value())) {
        builder.current_t = timestamp.value();
      }
    } else if (match = value_change.matches(content)) {
      auto proto = try_parse_value_change(match.value());
      if (proto.has_value()) {
        builder.dump.variables.at(proto->id)
          .add_change(builder.current_t, proto->new_value);
      }
    } else if (match = whitespace.matches(content)) {
      // Just skip whitespace
    } else {
      throw std::runtime_error("parse error");
    }
    assert(match.has_value());
    content.remove_prefix(match.value().size());
  }
  return builder.dump;
}

} // ns vcd
