#------------------------------------------------------------------------------
#
# Source code for MIPT ILab
# Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
#  common cmake for different grammars
#  default: cmake -DCMAKE_BUILD_TYPE=Release -DGRAMMAR="simplest-lr.yy" ..
#  try:     cmake -DCMAKE_BUILD_TYPE=Release -DGRAMMAR="simplest-nlr.yy" ..
#  try:     cmake -DCMAKE_BUILD_TYPE=Release -DGRAMMAR="simplest-amb.yy" ..
#
#-----------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.13.4)
project(Simplest)

find_package(BISON REQUIRED)
include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/FlexLexer.cmake")

if (NOT DEFINED GRAMMAR)
  set(GRAMMAR "simplest-lr.yy")
endif()

flex_target(scanner
  simplest.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

bison_target(parser
  ${GRAMMAR}
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/simplest.tab.hh")

add_flex_bison_dependency(scanner parser)

# simplest.cc pulls the generated header via driver.hpp
set_source_files_properties(simplest.cc PROPERTIES
  OBJECT_DEPENDS "${BISON_parser_OUTPUT_HEADER}")

add_executable(${PROJECT_NAME} 
  simplest.cc  
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)

set(TARGETS
  ${PROJECT_NAME}
)

foreach(TNAME ${TARGETS})
  target_compile_features(${TNAME} PRIVATE cxx_std_20)
  target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${FLEX_INCLUDE_DIRS})
endforeach()

