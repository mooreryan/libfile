#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "file.h"

#define FILE_EXTNAME_TXT "test_files/file_extname.txt"
#define FILE_BASENAME_TXT "test_files/file_basename.txt"

void
test___file_extname___should_ReturnTheExtension(void)
{
  TEST_ASSERT_EQUAL_STRING(".c", file_extname("test.c"));
  TEST_ASSERT_EQUAL_STRING("", file_extname("te.s/t"));
  TEST_ASSERT_EQUAL_STRING("", file_extname("te.s/t."));
  TEST_ASSERT_EQUAL_STRING("", file_extname(".te.s/t."));
  TEST_ASSERT_EQUAL_STRING(".c", file_extname(".te.s/t.c"));
  TEST_ASSERT_EQUAL_STRING(".c", file_extname("a/b/d/te.st.c"));
  TEST_ASSERT_EQUAL_STRING(".c", file_extname(".a/b/d/test.c"));
  TEST_ASSERT_EQUAL_STRING("", file_extname("test"));
  TEST_ASSERT_EQUAL_STRING("", file_extname("test."));
  TEST_ASSERT_EQUAL_STRING("", file_extname(".test."));
  TEST_ASSERT_EQUAL_STRING("", file_extname(".profile"));
  TEST_ASSERT_EQUAL_STRING(".sh", file_extname(".profile.sh"));
  TEST_ASSERT_EQUAL_STRING(".rb", file_extname("/tmp//bla.rb"));
}

/* On my mac NTFS would be false when looking here: https://github.com/ruby/ruby/blob/e6ad53beaa8f61c784d7e6c9cace5bd6ecc4d5c8/test/ruby/test_file_exhaustive.rb */
void
test___file_basename___should_ReturnTheBaseName(void)
{
  TEST_ASSERT_EQUAL_STRING("apple.c",
                           file_basename("/home/mooreryan/apple.c"));
  TEST_ASSERT_EQUAL_STRING("apple",
                           file_basename("/home/mooreryan/apple"));
  TEST_ASSERT_EQUAL_STRING("apple",
                           file_basename("apple"));
  TEST_ASSERT_EQUAL_STRING("apple",
                           file_basename("/apple"));

  TEST_ASSERT_EQUAL_STRING(" apple",
                           file_basename(" apple"));
  TEST_ASSERT_EQUAL_STRING(" apple",
                           file_basename("/ apple"));

  TEST_ASSERT_EQUAL_STRING(".apple",
                           file_basename(".apple"));
  TEST_ASSERT_EQUAL_STRING(".apple",
                           file_basename("/.apple"));

  TEST_ASSERT_EQUAL_STRING(". apple",
                           file_basename(". apple"));
  TEST_ASSERT_EQUAL_STRING(". apple",
                           file_basename("/. apple"));

  TEST_ASSERT_EQUAL_STRING(" .apple",
                           file_basename(" .apple"));
  TEST_ASSERT_EQUAL_STRING(" .apple",
                           file_basename("/ .apple"));

  TEST_ASSERT_EQUAL_STRING("pie",
                           file_basename("/apple/pie/"));
  TEST_ASSERT_EQUAL_STRING("pie",
                           file_basename("//apple/pie/"));
  TEST_ASSERT_EQUAL_STRING("pie",
                           file_basename("/apple//pie/"));
  TEST_ASSERT_EQUAL_STRING("pie",
                           file_basename("/apple/pie//"));
  TEST_ASSERT_EQUAL_STRING("pie",
                           file_basename("//apple//pie/"));
  TEST_ASSERT_EQUAL_STRING("pie",
                           file_basename("/apple//pie//"));
  TEST_ASSERT_EQUAL_STRING("pie",
                           file_basename("//apple//pie//"));


  TEST_ASSERT_EQUAL_STRING("", file_basename(""));


  TEST_ASSERT_EQUAL_STRING("apple.txt ",
                           file_basename("apple.txt "));
  TEST_ASSERT_EQUAL_STRING("apple.txt.",
                           file_basename("apple.txt."));
}
