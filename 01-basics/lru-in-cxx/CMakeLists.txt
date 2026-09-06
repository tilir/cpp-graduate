cmake_minimum_required(VERSION 3.16)
project(lru_in_cxx LANGUAGES CXX)

add_library(lru_cache INTERFACE)
target_include_directories(lru_cache INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(lru_cache INTERFACE cxx_std_17)

add_executable(lru cache.cc)
target_link_libraries(lru PRIVATE lru_cache)
set_target_properties(lru PROPERTIES CXX_EXTENSIONS OFF)

include(CTest)
if(BUILD_TESTING)
  add_executable(cache_tests tests/cache_tests.cc)
  target_link_libraries(cache_tests PRIVATE lru_cache)
  add_test(NAME cache COMMAND cache_tests)
  find_package(Python3 COMPONENTS Interpreter)
  if(Python3_Interpreter_FOUND)
    add_test(NAME driver COMMAND "${Python3_EXECUTABLE}"
      "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_lru.py" "$<TARGET_FILE:lru>")
  endif()
endif()
