#!/usr/bin/env ruby
Signal.trap("PIPE", "EXIT")

# See https://github.com/ruby/ruby/blob/e6ad53beaa8f61c784d7e6c9cace5bd6ecc4d5c8/test/ruby/test_file_exhaustive.rb

TEST_FILE_DIR = File.join __dir__, "..", "test", "test_files"
TEST_CODE_DIR = File.join __dir__, "..", "test"

prefixes = ["", "//", "/", ".", "/.", "bar/.", "/bar/."]
infixes = ["", " ", ".", ".ext "]
appendices = ["", " ", ".", ".bar", "::$DATA", "::$DATA.bar"]

methods = {
  extname: [],
  basename: []
}

prefixes.each do |prefix|
  appendices.each do |appendix|
    infixes.each do |infix|

      path1 = "#{prefix}foo#{infix}#{appendix}"
      path2 = "#{prefix}foo#{infix}.ext#{appendix}"

      methods.each do |method, results|
        [path1, path2].each do |path|
          results << [path, File.send(method, path)]
        end
      end
    end
  end
end

FUNCTION = "void\ntest___file_%s(void)\n{\n%s\n}\n\n"

File.open(File.join(TEST_CODE_DIR, "test_file_autogen.c"), "w") do |f|
  f.puts '#include "unity.h"'
  f.puts '#include "file.h"'
  f.puts

  methods.each do |method, results|
    assertions = []
    results.each do |(path, expected)|
      msg = "Path: '#{path}'"

      code = %Q[TEST_ASSERT_EQUAL_STRING_MESSAGE("#{expected}", file_#{method}("#{path}"), "#{msg}");]

      assertions << code
    end

    f.puts FUNCTION % [method, assertions.join("\n")]
  end
end