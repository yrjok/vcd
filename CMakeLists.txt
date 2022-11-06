cmake_minimum_required(VERSION 3.20)

project(
  vcd
  VERSION 0.0.1
  DESCRIPTION "A VCD file parser."
  LANGUAGES CXX
)

option(BUILD_TESTS "Also build all the tests" OFF)
option(CLANG_TIDY "RUn clang-tidy on source files" OFF)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_subdirectory(src)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

if(CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy --config-file .clang-tidy-config)
endif()
