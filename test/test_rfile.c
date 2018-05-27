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
#include "helper.h"
#include "rfile.h"

/* Need this for the tests to compile. */
#include "bstrlib.h"
#include "rlib.h"
#include "rstring.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void
test___rfile_exist___should_TellIfFnameExists(void)
{
  TEST_ASSERT_RERROR(rfile_exist(NULL));

  int ret_val = 0;
  rstring* rstr = NULL;
  char* cfname = "ryan_lala.txt";
  rstr = rstring_new(cfname);

  remove(cfname);

  TEST_ASSERT_RFALSE(rfile_exist(rstr));

  FILE* file = fopen(cfname, "w");
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }
  fclose(file);

  TEST_ASSERT_RTRUE(rfile_exist(rstr));

  remove(cfname);
  rstring_free(rstr);

  rstr = rstring_new(__FILE__);
  TEST_ASSERT_RTRUE(rfile_exist(rstr));
  rstring_free(rstr);

  rstr = rstring_new(".");
  TEST_ASSERT_RTRUE(rfile_exist(rstr));
  rstring_free(rstr);
}

void
test___rfile_is_directory___should_TellIfPathIsDirectory(void)
{
  TEST_ASSERT_RERROR(rfile_is_directory(NULL));

  rstring* rstr = NULL;
  int ret_val = 0;
  char* dirname = "ryan_lala";
  rmdir(dirname);

  ret_val = mkdir(dirname, S_IRUSR | S_IWUSR | S_IXUSR);
  if (ret_val != 0) {
    perror("mkdir");
    exit(1);
  }
  rstr = rstring_new(dirname);
  TEST_ASSERT_RTRUE(rfile_is_directory(rstr));
  rstring_free(rstr);

  ret_val = rmdir(dirname);
  if (ret_val != 0) {
    perror("rmdir");
    exit(1);
  }
  rstr = rstring_new(dirname);
  TEST_ASSERT_RFALSE(rfile_is_directory(rstr));
  rstring_free(rstr);

  rstr = rstring_new(__FILE__);
  TEST_ASSERT_RFALSE(rfile_is_directory(rstr));
  rstring_free(rstr);
}

void
test___rfile_is_file___should_TellIfFileNameIsAFile(void)
{

  rstring* rstr = NULL;

  TEST_ASSERT_RERROR(rfile_is_file(NULL));

  char* fname = "ryan_lala.txt";
  remove(fname);

  FILE* file = fopen(fname, "w");

  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }
  fclose(file);

  rstr = rstring_new(fname);
  TEST_ASSERT_RTRUE(rfile_is_file(rstr));
  remove(fname);
  TEST_ASSERT_RFALSE(rfile_is_file(rstr));
  rstring_free(rstr);


  rstr = rstring_new(__FILE__);
  TEST_ASSERT_RTRUE(rfile_is_file(rstr));
  rstring_free(rstr);
}



/* On my mac NTFS would be false when looking here: https://github.com/ruby/ruby/blob/e6ad53beaa8f61c784d7e6c9cace5bd6ecc4d5c8/test/ruby/test_file_exhaustive.rb */
void
test___rfile_basename___should_ReturnTheBaseName(void)
{

  TEST_ASSERT_NULL(rfile_basename(NULL));

  rstring* rstr = NULL;
  rstring* actual = NULL;

  TEST_ASSERT_EQUAL_RSTRING("/", (actual = rfile_basename(rstr = (rstring_new("/")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("/", (actual = rfile_basename(rstr = (rstring_new("//")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple.c", (actual = rfile_basename(rstr = (rstring_new("/home/mooreryan/apple.c")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rfile_basename(rstr = (rstring_new("/home/mooreryan/apple")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rfile_basename(rstr = (rstring_new("apple")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rfile_basename(rstr = (rstring_new("/apple")))));
  rstring_free(actual); rstring_free(rstr);

  TEST_ASSERT_EQUAL_RSTRING(" apple", (actual = rfile_basename(rstr = (rstring_new(" apple")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(" apple", (actual = rfile_basename(rstr = (rstring_new("/ apple")))));
  rstring_free(actual); rstring_free(rstr);

  TEST_ASSERT_EQUAL_RSTRING(".apple", (actual = rfile_basename(rstr = (rstring_new(".apple")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(".apple", (actual = rfile_basename(rstr = (rstring_new("/.apple")))));
  rstring_free(actual); rstring_free(rstr);

  TEST_ASSERT_EQUAL_RSTRING(". apple", (actual = rfile_basename(rstr = (rstring_new(". apple")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(". apple", (actual = rfile_basename(rstr = (rstring_new("/. apple")))));
  rstring_free(actual); rstring_free(rstr);

  TEST_ASSERT_EQUAL_RSTRING(" .apple", (actual = rfile_basename(rstr = (rstring_new(" .apple")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(" .apple", (actual = rfile_basename(rstr = (rstring_new("/ .apple")))));
  rstring_free(actual); rstring_free(rstr);

  TEST_ASSERT_EQUAL_RSTRING("pie", (actual = rfile_basename(rstr = (rstring_new("/apple/pie/")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("pie", (actual = rfile_basename(rstr = (rstring_new("//apple/pie/")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("pie", (actual = rfile_basename(rstr = (rstring_new("/apple//pie/")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("pie", (actual = rfile_basename(rstr = (rstring_new("/apple/pie//")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("pie", (actual = rfile_basename(rstr = (rstring_new("//apple//pie/")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("pie", (actual = rfile_basename(rstr = (rstring_new("/apple//pie//")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("pie", (actual = rfile_basename(rstr = (rstring_new("//apple//pie//")))));
  rstring_free(actual); rstring_free(rstr);


  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_basename(rstr = (rstring_new("")))));
  rstring_free(actual); rstring_free(rstr);


  TEST_ASSERT_EQUAL_RSTRING("apple.txt ", (actual = rfile_basename(rstr = (rstring_new("apple.txt ")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple.txt.", (actual = rfile_basename(rstr = (rstring_new("apple.txt.")))));
  rstring_free(actual); rstring_free(rstr);
}

void
test___rfile_dirname___should_ReturnTheDirName(void)
{
  TEST_ASSERT_NULL(rfile_dirname(NULL));

  rstring* rstr = NULL;
  rstring* actual = NULL;

  TEST_ASSERT_EQUAL_RSTRING("/apple", (actual = rfile_dirname((rstr = rstring_new("/apple/pie.txt")))));
  rstring_free(actual); rstring_free(rstr);

  TEST_ASSERT_EQUAL_RSTRING("/", (actual = rfile_dirname((rstr = rstring_new("/apple")))));
  rstring_free(actual); rstring_free(rstr);
}

void
test___rfile_extname___should_ReturnTheExtension(void)
{
  TEST_ASSERT_NULL(rfile_extname(NULL));

  rstring* rstr = NULL;
  rstring* actual = NULL;

  TEST_ASSERT_EQUAL_RSTRING(".c", (actual = rfile_extname((rstr = rstring_new("test.c")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_extname((rstr = rstring_new("te.s/t")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_extname((rstr = rstring_new("te.s/t.")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_extname((rstr = rstring_new(".te.s/t.")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(".c", (actual = rfile_extname((rstr = rstring_new(".te.s/t.c")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(".c", (actual = rfile_extname((rstr = rstring_new("a/b/d/te.st.c")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(".c", (actual = rfile_extname((rstr = rstring_new(".a/b/d/test.c")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_extname((rstr = rstring_new("test")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_extname((rstr = rstring_new("test.")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_extname((rstr = rstring_new(".test.")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_extname((rstr = rstring_new(".profile")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(".sh", (actual = rfile_extname((rstr = rstring_new(".profile.sh")))));
  rstring_free(actual); rstring_free(rstr);
  TEST_ASSERT_EQUAL_RSTRING(".rb", (actual = rfile_extname((rstr = rstring_new("/tmp//bla.rb")))));
  rstring_free(actual); rstring_free(rstr);
}

void
test___rfile_join___should_JoinStringsWithFileSep(void)
{
  TEST_ASSERT_NULL(rfile_join(NULL, 1));

  rstring* rstr = NULL;
  rstring* actual = NULL;

  rstring* strings1[3] = {
    rstring_new("apple"),
    rstring_new("pie"),
    rstring_new("good")
  };
  TEST_ASSERT_EQUAL_RSTRING_MESSAGE("apple/pie/good", (actual = rfile_join(strings1, 3)), actual->data);
  rstring_free(actual);
  rstring_free(rstr);
  rstring_free(strings1[0]);
  rstring_free(strings1[1]);
  rstring_free(strings1[2]);

  rstring* strings2[1] = { rstring_new("") };
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_join(strings2, 1)));
  rstring_free(actual); rstring_free(rstr);
  rstring_free(strings2[0]);

  rstring* strings3[2] = { rstring_new(""), rstring_new("") };
  TEST_ASSERT_EQUAL_RSTRING("/", (actual = rfile_join(strings3, 2)));
  rstring_free(actual); rstring_free(rstr);
  rstring_free(strings3[0]);
  rstring_free(strings3[1]);

  rstring* strings4[2] = { rstring_new(NULL), rstring_new("") };
  TEST_ASSERT_NULL(rfile_join(strings4, 2));
  rstring_free(strings4[1]);

  rstring* strings5[1] = { rstring_new("apple") };
  TEST_ASSERT_NULL(rfile_join(strings5, 0));
  rstring_free(strings5[0]);

  TEST_ASSERT_NULL(rfile_join(NULL, 2));
}
