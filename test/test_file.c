#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* For mkdir */
#include <sys/stat.h>

/* For rmdir */
#include <unistd.h>

#include <errno.h>

#include "unity.h"
#include "file.h"

void
test___file_is_directory___should_TellIfPathIsDirectory(void)
{

  int rval = 0;
  char* dirname = "ryan_lala";
  rmdir(dirname);

  rval = mkdir(dirname, S_IRUSR | S_IWUSR | S_IXUSR);
  if (rval != 0) {
    perror("mkdir");
    exit(1);
  }
  TEST_ASSERT_TRUE(file_is_directory(dirname));


  rval = rmdir(dirname);
  if (rval != 0) {
    perror("rmdir");
    exit(1);
  }
  TEST_ASSERT_FALSE(file_is_directory(dirname));

  TEST_ASSERT_FALSE(file_is_directory(__FILE__));
}

void
test___file_is_file___should_TellIfFileNameIsAFile(void)
{
  char* fname = "ryan_lala.txt";
  remove(fname);

  FILE* file = fopen(fname, "w");

  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }
  fclose(file);

  TEST_ASSERT_TRUE(file_is_file(fname));

  remove(fname);

  TEST_ASSERT_FALSE(file_is_file(fname));

  TEST_ASSERT_TRUE(file_is_file(__FILE__));
}



/* On my mac NTFS would be false when looking here: https://github.com/ruby/ruby/blob/e6ad53beaa8f61c784d7e6c9cace5bd6ecc4d5c8/test/ruby/test_file_exhaustive.rb */
void
test___file_basename___should_ReturnTheBaseName(void)
{
  char* actual = NULL;

  TEST_ASSERT_EQUAL_STRING("/", (actual = file_basename("/")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("/", (actual = file_basename("//")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("apple.c", (actual = file_basename("/home/mooreryan/apple.c")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("apple", (actual = file_basename("/home/mooreryan/apple")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("apple", (actual = file_basename("apple")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("apple", (actual = file_basename("/apple")));
  free(actual);

  TEST_ASSERT_EQUAL_STRING(" apple", (actual = file_basename(" apple")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(" apple", (actual = file_basename("/ apple")));
  free(actual);

  TEST_ASSERT_EQUAL_STRING(".apple", (actual = file_basename(".apple")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(".apple", (actual = file_basename("/.apple")));
  free(actual);

  TEST_ASSERT_EQUAL_STRING(". apple", (actual = file_basename(". apple")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(". apple", (actual = file_basename("/. apple")));
  free(actual);

  TEST_ASSERT_EQUAL_STRING(" .apple", (actual = file_basename(" .apple")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(" .apple", (actual = file_basename("/ .apple")));
  free(actual);

  TEST_ASSERT_EQUAL_STRING("pie", (actual = file_basename("/apple/pie/")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("pie", (actual = file_basename("//apple/pie/")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("pie", (actual = file_basename("/apple//pie/")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("pie", (actual = file_basename("/apple/pie//")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("pie", (actual = file_basename("//apple//pie/")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("pie", (actual = file_basename("/apple//pie//")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("pie", (actual = file_basename("//apple//pie//")));
  free(actual);


  TEST_ASSERT_EQUAL_STRING("", (actual = file_basename("")));
  free(actual);


  TEST_ASSERT_EQUAL_STRING("apple.txt ", (actual = file_basename("apple.txt ")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("apple.txt.", (actual = file_basename("apple.txt.")));
  free(actual);
}

void
test___file_extname___should_ReturnTheExtension(void)
{
  char* actual = NULL;

  TEST_ASSERT_EQUAL_STRING(".c", (actual = file_extname("test.c")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("", (actual = file_extname("te.s/t")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("", (actual = file_extname("te.s/t.")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("", (actual = file_extname(".te.s/t.")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(".c", (actual = file_extname(".te.s/t.c")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(".c", (actual = file_extname("a/b/d/te.st.c")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(".c", (actual = file_extname(".a/b/d/test.c")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("", (actual = file_extname("test")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("", (actual = file_extname("test.")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("", (actual = file_extname(".test.")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING("", (actual = file_extname(".profile")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(".sh", (actual = file_extname(".profile.sh")));
  free(actual);
  TEST_ASSERT_EQUAL_STRING(".rb", (actual = file_extname("/tmp//bla.rb")));
  free(actual);
}
