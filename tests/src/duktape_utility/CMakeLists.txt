cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

include(../../tests.cmake)

project(karabiner_test)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../vendor/duktape-2.7.0/src)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../vendor/duktape-2.7.0/extras/console)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../vendor/duktape-2.7.0/extras/module-node)

# Suppress duktape warnings
add_compile_options(-Wno-deprecated-declarations)
add_compile_options(-Wno-unused-but-set-variable)

add_executable(
  karabiner_test
  src/test.cpp
  ../../../vendor/duktape-2.7.0/src/duktape.c
  ../../../vendor/duktape-2.7.0/extras/console/duk_console.c
  ../../../vendor/duktape-2.7.0/extras/module-node/duk_module_node.c
)
