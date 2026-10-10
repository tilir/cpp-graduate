#!/usr/bin/env ruby
#------------------------------------------------------------------------------
#
# Source code for MIPT ILab
# Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# ParaCL end-to-end test driver (invoked from ctest):
# compile a .pcl source to LLVM IR with ParaCL, link it with pcllib via
# clang++, run with the given input and diff against the expected output.
#
# Usage: run_pcl_test.rb <paracl-bin> <clang++> <pcllib.cc> <src.pcl>
#          <input> <expected.out>
#
#------------------------------------------------------------------------------

require 'fileutils'
require 'tmpdir'
require 'open3'

PARACL, CXX, PCLLIB, SRC, INPUT, EXPECTED = *ARGV

def run(*cmd, **opts)
  system(*cmd, **opts, exception: true)
end

Dir.mktmpdir do |tmp|
  FileUtils.cp(SRC, tmp)
  FileUtils.cd(tmp) do
    base = File.basename(SRC)
    run(PARACL, base, out: File::NULL)
    run(CXX, "#{base}.ll", PCLLIB, '-o', 'test.bin')
    actual, errors, status = Open3.capture3('./test.bin', stdin_data: "#{INPUT}\n")
    abort "#{base} failed (#{status}):\n#{errors}" unless status.success?
    expected = File.read(EXPECTED)
    if actual == expected
      puts "#{base}: output matches #{File.basename(EXPECTED)}"
    else
      puts "#{base}: MISMATCH"
      puts "--- expected ---"
      puts expected
      puts "--- actual ---"
      puts actual
      exit 1
    end
  end
end
