include(FetchContent)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest
  GIT_TAG v2.4.9
)

FetchContent_MakeAvailable(doctest)

add_executable(test_runner)
target_include_directories(test_runner PRIVATE ${doctest_SOURCE_DIR})
target_sources(test_runner
  PRIVATE
    test_runner.cpp
    unit-parser.cpp
    unit-variable_dump.cpp
    unit-recognizers.cpp
    unit-combinators.cpp
    unit-recognizer_factory.cpp
)
target_link_libraries(test_runner PRIVATE vcd)

add_test(testsuite test_runner)
