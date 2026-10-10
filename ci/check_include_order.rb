#!/usr/bin/env ruby
#------------------------------------------------------------------------------
#
# Source code for MIPT ILab
# Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Style rule check (invoked from ctest): in every source file, project
# headers (#include "...") must come before standard/system headers
# (#include <...>). See AGENTS.md.
#
# Usage: check_include_order.rb [repo-root]
#
#------------------------------------------------------------------------------

require_relative 'source_files'

ROOT = File.expand_path(ARGV[0] || '.')

violations = []
source_files(ROOT).each do |f|
  rel = f.delete_prefix("#{ROOT}/")

  angle_seen = false
  File.foreach(f) do |line|
    case line
    when /^\s*#\s*include\s*</
      angle_seen = true
    when /^\s*#\s*include\s*"/
      # annotated exceptions: generated/configured headers that must stay
      # late (e.g. bison output requiring YYSTYPE), see AGENTS.md
      if angle_seen && !line.include?('include-order: late')
        violations << rel
        break
      end
    end
  end
end

violations.each { |v| puts "include-order violation: #{v}" }
exit(violations.empty? ? 0 : 1)
