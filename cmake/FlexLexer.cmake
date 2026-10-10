# FindFLEX requires the generator, but does not require the C++ scanner header.
find_package(FLEX REQUIRED)
if(NOT FLEX_INCLUDE_DIRS)
  message(FATAL_ERROR
    "FlexLexer.h was not found. Install the Flex development headers "
    "(libfl-dev on Debian/Ubuntu), or set FLEX_INCLUDE_DIR.")
endif()
