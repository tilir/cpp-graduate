#!/usr/bin/env ruby
# Regression checks: a crashing process must never pass just because stdout matches.
require 'open3'
require 'tmpdir'
require 'rbconfig'

Dir.mktmpdir('io driver ') do |dir|
  binary = File.join(dir, 'program with spaces')
  driver = File.join(__dir__, 'run_io_test.rb')
  File.write(binary, "#!#{RbConfig.ruby}\nSTDOUT.write(STDIN.read)\n")
  File.chmod(0o755, binary)
  input = File.join(dir, 'input.txt')
  File.write(input, "hello\n")

  [['str', 'hello', 'hello', true], ['str', 'hello', 'wrong', false],
   ['file', input, input, true]].each do |mode, data, expected, success|
    output, status = Open3.capture2e(RbConfig.ruby, driver, binary, mode, data, expected)
    abort output unless status.success? == success
  end

  File.write(binary, "#!#{RbConfig.ruby}\nSTDOUT.write(STDIN.read)\nexit 7\n")
  output, status = Open3.capture2e(RbConfig.ruby, driver, binary, 'str', 'hello', 'hello')
  abort 'Nonzero exit incorrectly accepted' if status.success?
  abort output unless output.include?('failed')
end
