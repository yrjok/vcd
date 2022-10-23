add_library(vcd STATIC)

target_sources(vcd
  PRIVATE
    parser/tokenizer.cpp
    parser/section_token.cpp
    parser/value_change_token.cpp
    parser/timestamp_token.cpp
    parser/parser.cpp
    value_change_dump.cpp
    variable_dump.cpp
)
