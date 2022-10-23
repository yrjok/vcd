#pragma once

#include <vcd/value_change_dump.h>
#include <vcd/parser/token.h>
#include <vcd/parser/value_change_token.h>
#include <vcd/parser/section_token.h>
#include <vcd/parser/timestamp_token.h>

#include <string>
#include <vector>
#include <memory>

namespace vcd {

class parser {
public:
  parser ();

  void consume (std::unique_ptr<token> next_token);

  [[nodiscard]] value_change_dump finalize ();

  void parse_section (section_token & section);
  void parse_timestamp (timestamp_token & timestamp);
  void parse_value_change (value_change_token value_change);

private:

  bool header_done_;

  value_change_dump temp_;
};

} // ns vcd
