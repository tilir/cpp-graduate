#------------------------------------------------------------------------------
#
# Source code for MIPT ILab
# Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
#  cmake for simplest grammar example
#  default: cmake -DCMAKE_BUILD_TYPE=Release -DGRAMMAR="numgrammar-sr.y" ..
#      try: cmake -DCMAKE_BUILD_TYPE=Release -DGRAMMAR="numgrammar-sr-fixed.y" ..
#      try: cmake -DCMAKE_BUILD_TYPE=Release -DGRAMMAR="numgrammar.y" ..
#
#-----------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.13.4)
project(numgrammar)

find_package(BISON REQUIRED)
include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/FlexLexer.cmake")

if (NOT DEFINED GRAMMAR)
  set(GRAMMAR "numgrammar-sr.y")
endif()

flex_target(scanner
  numgrammar.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

bison_target(parser
  ${GRAMMAR}
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/numgrammar.tab.hh")

add_flex_bison_dependency(scanner parser)

# driver.cc pulls the generated header via numdriver.hpp
set_source_files_properties(driver.cc PROPERTIES
  OBJECT_DEPENDS "${BISON_parser_OUTPUT_HEADER}")

# Generated scanner/parser code is owned by Flex/Bison, not the course.
set_source_files_properties(${FLEX_scanner_OUTPUTS} ${BISON_parser_OUTPUTS} PROPERTIES SKIP_LINTING ON)

add_executable(${PROJECT_NAME} 
  driver.cc  
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${FLEX_INCLUDE_DIRS})

