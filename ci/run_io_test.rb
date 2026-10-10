#!/usr/bin/env ruby
#------------------------------------------------------------------------------
#
# Source code for MIPT ILab
# Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Generic stdin/stdout test driver (invoked from ctest): feed input to a
# binary and compare its stdout with the expected output.
#
# Usage: run_io_test.rb <binary> str  <input-text> <expected-text>
#        run_io_test.rb <binary> file <input-file> <expected-file>
#
#------------------------------------------------------------------------------

require 'open3'

BINARY, MODE, INPUT, EXPECTED = *ARGV
abort 'Usage: run_io_test.rb <binary> <str|file> <input> <expected>' unless
  ARGV.size == 4 && %w[str file].include?(MODE)

def read_arg(mode, value)
  mode == 'file' ? File.read(value) : "#{value}\n"
end

input = read_arg(MODE, INPUT)
expected = read_arg(MODE, EXPECTED)

actual, errors, status = Open3.capture3([BINARY, BINARY], stdin_data: input)
abort "#{BINARY} failed (#{status}):\n#{errors}" unless status.success?

if actual == expected
  puts "#{File.basename(BINARY)}: output matches"
else
  puts "#{File.basename(BINARY)}: MISMATCH"
  puts "--- expected ---"
  puts expected
  puts "--- actual ---"
  puts actual
  exit 1
end
