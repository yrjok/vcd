#pragma once

#include <vcd/value_change_dump.h>
#include <vcd/types.h>

#include <optional>
#include <vector>
#include <memory>
#include <functional>

namespace vcd {

class parser {
public:
  parser ();

  value_change_dump parse (std::string_view content) const;

private:
  struct vcd_builder {
    types::timestamp current_t;
    value_change_dump dump;
  };

  struct variable_proto {
    std::string id;
    std::string reference;
    unsigned num_bits;
  };

  struct value_change_proto {
    std::string id;
    types::value new_value;
  };

  std::optional<variable_proto> try_parse_section (std::string_view text) const;
  std::optional<types::timestamp> try_parse_timestamp (std::string_view text) const;
  std::optional<value_change_proto> try_parse_value_change (std::string_view text) const;
};

} // ns vcd
