cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

include(../../tests.cmake)

project(karabiner_test)

add_executable(
  karabiner_test
  src/test.cpp
)
