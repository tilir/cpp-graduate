#!/usr/bin/env ruby
require 'open3'
require_relative 'source_files'

formatter, root, mode = ARGV
abort 'Usage: format.rb <clang-format-21> <repo-root> [--fix]' unless
  formatter && root && (mode.nil? || mode == '--fix')
root = File.expand_path(root)
version, status = Open3.capture2(formatter, '--version')
abort 'Formatting requires clang-format 21' unless status.success? && version.match?(/version 21\./)

# Keep intentional compile-error demos untouched, as required by AGENTS.md.
cmake = File.read(File.join(root, 'CMakeLists.txt'))
excluded = cmake.match(/set\(INTENTIONAL_ERRORS\s+(.*?)\)/m)[1].split
files = source_files(root).reject { |file| excluded.include?(file.delete_prefix("#{root}/")) }
failed = false
files.each_slice(50) do |batch|
  args = mode == '--fix' ? ['-i'] : ['--dry-run', '--Werror', '--ferror-limit=1']
  failed = true unless system(formatter, '--style=file', *args, *batch)
end
exit(failed ? 1 : 0)
