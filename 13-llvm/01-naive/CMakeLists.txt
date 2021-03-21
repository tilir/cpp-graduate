cmake_minimum_required(VERSION 3.13.4)
project(ParaCL)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(COMPILE_NAME ${PROJECT_NAME})
set(INTERP_NAME "${PROJECT_NAME}i")

message(STATUS "Interp: ${INTERP_NAME}")

flex_target(scanner
  scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)

bison_target(parser
  compiler.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/compiler.cpp.h")

add_flex_bison_dependency(scanner parser)

add_executable(${COMPILE_NAME} 
  driver.cc
  Node.cc
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)

# build for codegen
target_compile_definitions(${COMPILE_NAME} PRIVATE CODEGEN=1)

add_executable(${INTERP_NAME}
  driver.cc
  Node.cc
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)

# build as interpreter
target_compile_definitions(${INTERP_NAME} PRIVATE CODEGEN=0)

set(TARGETS
  ${COMPILE_NAME}
  ${INTERP_NAME}
)

# use llvm-config --components and some guessing to determine components
llvm_map_components_to_libnames(llvm_libs support core codegen irreader)

# common for all targets
foreach(TNAME ${TARGETS})
  target_compile_features(${TNAME} PRIVATE cxx_std_17)
  target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_include_directories(${TNAME} PRIVATE ${LLVM_INCLUDE_DIRS})
  target_link_libraries(${TNAME} ${llvm_libs})
endforeach()


target_compile_definitions(${COMPILE_NAME} PRIVATE
  $<$<CONFIG:Debug>:DUMPALL>
)

