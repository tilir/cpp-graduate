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
find_package(FLEX REQUIRED)

if (NOT DEFINED GRAMMAR)
  set(GRAMMAR "numgrammar-sr.y" CACHE STRING "file with grammar" FORCE)
endif()

flex_target(scanner
  numgrammar.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

bison_target(parser
  ${GRAMMAR}
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/numgrammar.tab.hh")

add_flex_bison_dependency(scanner parser)

add_executable(${PROJECT_NAME} 
  driver.cc  
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

