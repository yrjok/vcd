include(FetchContent)

FetchContent_Declare(
  result
  GIT_REPOSITORY https://github.com/yrjok/result
  GIT_TAG main
)

FetchContent_MakeAvailable(result)

add_library(vcd STATIC)

target_include_directories(vcd PRIVATE ${result_SOURCE_DIR})

target_sources(vcd
  PRIVATE
    parser/parser.cpp
    recognizers/recognizer.cpp
    recognizers/fundamental.cpp
    recognizers/combinators.cpp
    recognizers/recognizer_factory.cpp
    value_change_dump.cpp
    variable_dump.cpp
    types.cpp
)
