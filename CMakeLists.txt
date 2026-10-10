#------------------------------------------------------------------------------
#
# Source code for MIPT ILab
# Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Umbrella build for the course examples.
#
#   cmake -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build -j
#   ctest --test-dir build --output-on-failure
#
# Useful options:
#   -DENABLE_IWYU=ON   run include-what-you-use on every translation unit
#   -DLLVM_DIR=...     path to LLVM cmake package (for 15-llvm/naive-pcl)
#
# Every standalone example is compiled as an object-library target, which
# checks that it builds without requiring it to link or run. Subprojects
# with their own build logic (parsers, LRU caches, ParaCL) are added as
# subdirectories or as CMake targets.
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)
project(cpp_graduate LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)

option(ENABLE_IWYU "Run include-what-you-use on every translation unit" OFF)
option(REQUIRE_LLVM "Fail configuration if LLVM 21 is unavailable" OFF)
option(ENABLE_FORMAT_CHECK "Require clang-format-21 and register its CTest check" OFF)
if(ENABLE_IWYU)
  if(CMAKE_VERSION VERSION_LESS 3.27)
    message(FATAL_ERROR "ENABLE_IWYU requires CMake 3.27+ to exclude generated sources")
  endif()
  find_program(IWYU_EXECUTABLE NAMES include-what-you-use iwyu REQUIRED)
  set(IWYU_COMMAND "${IWYU_EXECUTABLE}" "-Xiwyu" "--quoted_includes_first"
    "-Xiwyu" "--mapping_file=${CMAKE_CURRENT_SOURCE_DIR}/ci/iwyu.imp"
    "-Xiwyu" "--check_also=${CMAKE_CURRENT_SOURCE_DIR}/*")
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_COMMAND}
    "-Xiwyu" "--mapping_file=${CMAKE_CURRENT_SOURCE_DIR}/ci/iwyu-cxx.imp")
  set(CMAKE_C_INCLUDE_WHAT_YOU_USE "${IWYU_COMMAND}")
  message(STATUS "IWYU enabled: ${IWYU_EXECUTABLE}")
endif()

#------------------------------------------------------------------------------
# Standalone examples sweep
#------------------------------------------------------------------------------

# Directories with their own build logic, excluded from the sweep
set(SUBPROJECT_DIRS
  01-basics/lru-in-c
  01-basics/lru-in-c-datastructs
  01-basics/lru-in-c-naive
  01-basics/lru-in-cxx
  07-parsers/advgrammar
  07-parsers/numadvanced
  07-parsers/numexample
  07-parsers/numgrammar
  07-parsers/simplest-arith
  15-llvm/naive-pcl
  15-llvm/tests-pcl
)

# Intentional compile-error demos from the lectures: they MUST NOT build
set(INTENTIONAL_ERRORS
  03-concrete/ctors-ambig.cc
  03-concrete/overload.cc
  03-concrete/setsol.cc
  03-concrete/volstr.cc
  04-raii/lvannot.cc
  05-opers/binaryop-basic.cc
  05-opers/binaryop.cc
  06-inheritance/constraints.cc
  06-inheritance/romb-virtual-casts.cc
  18-constexpr/req-overload.cc
  18-constexpr/unruh.cc
)

# Require the glad loader, which is not vendored in the repository
set(NO_GLAD
  10-3d/ogl-extensions.cc
  10-3d/ogl-frag-shader.cc
  10-3d/ogl-motion.cc
)

# google-benchmark examples: linked as real executables when possible
set(BENCH_SOURCES
  16-containers/accbench.cc
  16-containers/fillbench.cc
  16-containers/multimap_bench.cc
)

# These examples compile to objects: header probes must not require GPU libraries.
set(SAVED_TRY_COMPILE_TARGET_TYPE "${CMAKE_TRY_COMPILE_TARGET_TYPE}")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
check_include_file("CL/cl.h" HAVE_OPENCL_H)
check_include_file_cxx("CL/opencl.hpp" HAVE_OPENCL_HPP)
check_include_file_cxx("GLFW/glfw3.h" HAVE_GLFW_H)
check_include_file_cxx("vulkan/vulkan.h" HAVE_VULKAN_H)
check_include_file_cxx("glm/glm.hpp" HAVE_GLM_H)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "${SAVED_TRY_COMPILE_TARGET_TYPE}")

if(NOT HAVE_OPENCL_H)
  list(APPEND INTENTIONAL_ERRORS # reuse the skip mechanism
    11-compute/cl_justbuf.c 11-compute/cl_platform.c)
  message(STATUS "OpenCL headers not found, skipping 11-compute OpenCL examples")
endif()
if(NOT HAVE_OPENCL_HPP)
  list(APPEND INTENTIONAL_ERRORS
    11-compute/cl_gemm.cc 11-compute/cl_hist.cc 11-compute/cl_justbuf.cc
    11-compute/cl_vectoradd.cc)
  message(STATUS "OpenCL C++ headers not found, skipping OpenCL C++ examples")
endif()
if(NOT HAVE_GLFW_H)
  list(APPEND INTENTIONAL_ERRORS 10-3d/ogl-simplest.cc)
  message(STATUS "GLFW headers not found, skipping 10-3d/ogl-simplest.cc")
endif()
if(NOT (HAVE_VULKAN_H AND HAVE_GLM_H AND HAVE_GLFW_H))
  list(APPEND INTENTIONAL_ERRORS 10-3d/vk-simplest.cc)
  message(STATUS "Vulkan/GLM/GLFW headers not found, skipping 10-3d/vk-simplest.cc")
endif()

file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "[0-9][0-9]-*/*.cc" "[0-9][0-9]-*/*.cpp" "[0-9][0-9]-*/*.c")

set(BUILT_COUNT 0)
foreach(src ${EXAMPLE_SOURCES})
  set(skip FALSE)
  if(src MATCHES "(^|/)(archive|ci|\\.github)/" OR src MATCHES "(^|/)build[^/]*/")
    set(skip TRUE)
  endif()
  foreach(dir ${SUBPROJECT_DIRS})
    if(src MATCHES "^${dir}/")
      set(skip TRUE)
    endif()
  endforeach()
  list(FIND INTENTIONAL_ERRORS "${src}" idx)
  if(NOT idx EQUAL -1)
    set(skip TRUE)
  endif()
  list(FIND NO_GLAD "${src}" idx)
  if(NOT idx EQUAL -1)
    set(skip TRUE)
  endif()
  list(FIND BENCH_SOURCES "${src}" idx)
  if(NOT idx EQUAL -1)
    set(skip TRUE)
  endif()
  if(skip)
    continue()
  endif()

  string(REGEX REPLACE "[^A-Za-z0-9]" "_" tgt "ex_${src}")
  add_library(${tgt} OBJECT "${src}")
  if(src STREQUAL "05-opers/binaryop-resolved.cc")
    target_compile_definitions(${tgt} PRIVATE NOFAIL)
  endif()
  math(EXPR BUILT_COUNT "${BUILT_COUNT} + 1")
endforeach()
message(STATUS "Standalone examples to compile: ${BUILT_COUNT}")

#------------------------------------------------------------------------------
# google-benchmark executables
#------------------------------------------------------------------------------

find_package(benchmark QUIET)
find_package(Threads QUIET)
if(benchmark_FOUND AND Threads_FOUND)
  foreach(src ${BENCH_SOURCES})
    string(REGEX REPLACE "[^A-Za-z0-9]" "_" tgt "bench_${src}")
    add_executable(${tgt} "${src}")
    target_link_libraries(${tgt} PRIVATE benchmark::benchmark Threads::Threads)
  endforeach()
else()
  message(STATUS "google benchmark not found, skipping ${BENCH_SOURCES}")
endif()

#------------------------------------------------------------------------------
# Subprojects
#------------------------------------------------------------------------------

add_subdirectory(01-basics/lru-in-cxx)
include(cmake/LruExamples.cmake)
add_subdirectory(07-parsers/simplest-arith)
add_subdirectory(07-parsers/numgrammar)
add_subdirectory(07-parsers/numexample)
add_subdirectory(07-parsers/numadvanced)
add_subdirectory(07-parsers/advgrammar)

if(REQUIRE_LLVM)
  find_package(LLVM 21.1 REQUIRED CONFIG)
else()
  find_package(LLVM 21.1 QUIET CONFIG)
endif()
if(LLVM_FOUND)
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}, building naive-pcl")
  add_subdirectory(15-llvm/naive-pcl)
else()
  message(STATUS "LLVM 21 cmake package not found, skipping 15-llvm/naive-pcl")
endif()

#------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------

if(BUILD_TESTING)
  find_package(Ruby REQUIRED)

  add_test(NAME io-test-driver
    COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/test_io_driver.rb")

  # parser smoke tests
  add_test(NAME simplest-arith-001
    COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/run_io_test.rb"
            "$<TARGET_FILE:Simplest>" str "1+2*3" "7")
  # The lecture's 001.ans describes 002.dat with a corrected grammar.
  # Keep the original ambiguous grammar and use an answer for the actual input.
  add_test(NAME numgrammar-001
    COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/run_io_test.rb"
            "$<TARGET_FILE:numgrammar>" file
            "${CMAKE_CURRENT_SOURCE_DIR}/07-parsers/numgrammar/tests/001.dat"
            "${CMAKE_CURRENT_SOURCE_DIR}/ci/expected/numgrammar-001.out")
  add_test(NAME advgrammar-001
    COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/run_io_test.rb"
            "$<TARGET_FILE:advgrammar>" file
            "${CMAKE_CURRENT_SOURCE_DIR}/07-parsers/advgrammar/tests/001.dat"
            "${CMAKE_CURRENT_SOURCE_DIR}/07-parsers/advgrammar/tests/001.ans")

  # ParaCL end-to-end tests: compile .pcl to LLVM IR, link with pcllib, run
  if(TARGET ParaCL)
    find_program(CLANGXX_EXECUTABLE NAMES clang++-21)
    if(CLANGXX_EXECUTABLE)
      foreach(t "fib/file.pcl 25 15-llvm/tests-pcl/fib/file.out"
                "simplest/addsub.pcl 5 ci/expected/addsub.out"
                "simplest/addsubif.pcl 5 ci/expected/addsubif.out"
                "simplest/parentheses.pcl 2 ci/expected/parentheses.out"
                "collatz/basic.pcl 27 ci/expected/collatz-basic.out")
        string(REGEX MATCH "^([^ ]+) ([^ ]+) (.+)$" m "${t}")
        set(pcl "${CMAKE_MATCH_1}")
        set(input "${CMAKE_MATCH_2}")
        set(expected "${CMAKE_MATCH_3}")
        get_filename_component(tname "${pcl}" NAME_WE)
        add_test(NAME "pcl-${tname}"
          COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/run_pcl_test.rb"
                  "$<TARGET_FILE:ParaCL>" "${CLANGXX_EXECUTABLE}"
                  "${CMAKE_CURRENT_SOURCE_DIR}/15-llvm/pcl-lib/pcllib.cc"
                  "${CMAKE_CURRENT_SOURCE_DIR}/15-llvm/tests-pcl/${pcl}"
                  "${input}"
                  "${CMAKE_CURRENT_SOURCE_DIR}/${expected}")
      endforeach()
    else()
      message(STATUS "clang++ not found, skipping ParaCL runtime tests")
    endif()
  endif()

  # style rule: project headers ("...") come before standard headers (<...>)
  add_test(NAME include-order
    COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/check_include_order.rb"
            "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# Formatting works without compiling any examples.
find_package(Ruby QUIET)
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format-21)
if(ENABLE_FORMAT_CHECK AND (NOT CLANG_FORMAT_EXECUTABLE OR NOT Ruby_FOUND))
  message(FATAL_ERROR "ENABLE_FORMAT_CHECK requires clang-format-21 and Ruby")
endif()
if(CLANG_FORMAT_EXECUTABLE AND Ruby_FOUND)
  add_custom_target(format
    COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/format.rb"
            "${CLANG_FORMAT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}" --fix
    VERBATIM)
  add_custom_target(format-check
    COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/format.rb"
            "${CLANG_FORMAT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM)
  if(BUILD_TESTING)
    add_test(NAME clang-format
      COMMAND "${Ruby_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/ci/format.rb"
              "${CLANG_FORMAT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}")
    set_tests_properties(clang-format include-order PROPERTIES LABELS style)
  endif()
endif()
