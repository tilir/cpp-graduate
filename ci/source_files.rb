# Shared scope for source checks: lecture sources only, never build artifacts.
require 'pathname'

def source_files(root)
  Dir.glob(File.join(root, '[0-9][0-9]-*', '**', '*.{cc,cpp,c,hpp,hh,h}')).sort.reject do |file|
    Pathname.new(file).relative_path_from(Pathname.new(root)).each_filename.any? do |part|
      part.start_with?('build')
    end
  end
end
