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
#include "rlib.h"

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
test___rfile_basename2___should_ReturnBasenameWithoutExt(void)
{
  rstring* actual = NULL;
  rstring* rstr = NULL;
  rstring* ext = NULL;

  /* If the ext is empty, it is the same as rfile_basename() */
  ext = rstring_new("");
  rstr = rstring_new("/home/moorer/apple.pie.txt");
  actual = rfile_basename2(rstr, ext);
  TEST_ASSERT_EQUAL_RSTRING("apple.pie.txt", actual);
  rstring_free(ext);
  rstring_free(rstr);
  rstring_free(actual);

  ext = rstring_new("txt");
  rstr = rstring_new("/home/moorer/apple.pie.txt");
  actual = rfile_basename2(rstr, ext);
  TEST_ASSERT_EQUAL_RSTRING("apple.pie.", actual);
  rstring_free(ext);
  rstring_free(rstr);
  rstring_free(actual);

  ext = rstring_new(".txt");
  rstr = rstring_new("/home/moorer/apple.pie.txt");
  actual = rfile_basename2(rstr, ext);
  TEST_ASSERT_EQUAL_RSTRING("apple.pie", actual);
  rstring_free(ext);
  rstring_free(rstr);
  rstring_free(actual);

  ext = rstring_new("ie.txt");
  rstr = rstring_new("/home/moorer/apple.pie.txt");
  actual = rfile_basename2(rstr, ext);
  TEST_ASSERT_EQUAL_RSTRING("apple.p", actual);
  rstring_free(ext);
  rstring_free(rstr);
  rstring_free(actual);

  ext = rstring_new(".pie.txt");
  rstr = rstring_new("/home/moorer/apple.pie.txt");
  actual = rfile_basename2(rstr, ext);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(ext);
  rstring_free(rstr);
  rstring_free(actual);

  /* When the ext isn't actually in the fname, it doesn't strip anything off */
  ext = rstring_new(".pie.txtarstoien");
  rstr = rstring_new("/home/moorer/apple.pie.txt");
  actual = rfile_basename2(rstr, ext);
  TEST_ASSERT_EQUAL_RSTRING("apple.pie.txt", actual);
  rstring_free(ext);
  rstring_free(rstr);
  rstring_free(actual);

  /* If the ext is in the filename but not at the end, it doesn't
     strip anything. */
  ext = rstring_new(".pie");
  rstr = rstring_new("/home/moorer/apple.pie.txt");
  actual = rfile_basename2(rstr, ext);
  TEST_ASSERT_EQUAL_RSTRING("apple.pie.txt", actual);
  rstring_free(ext);
  rstring_free(rstr);
  rstring_free(actual);

  /* If it has two extensions, only the first is removed. */
  ext = rstring_new(".txt");
  rstr = rstring_new("/home/moorer/apple.pie.txt.txt");
  actual = rfile_basename2(rstr, ext);
  TEST_ASSERT_EQUAL_RSTRING("apple.pie.txt", actual);
  rstring_free(ext);
  rstring_free(rstr);
  rstring_free(actual);

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
  TEST_ASSERT_NULL(rfile_join(NULL));

  int val = 0;
  rstring_array* rary = NULL;
  rstring* actual = NULL;

  rary = rstring_array_new();
  assert(rstring_array_push_cstr(rary, "apple") == ROKAY);
  assert(rstring_array_push_cstr(rary, "pie") == ROKAY);
  assert(rstring_array_push_cstr(rary, "good") == ROKAY);

  TEST_ASSERT_EQUAL_RSTRING_MESSAGE("apple/pie/good", (actual = rfile_join(rary)), rstring_data(actual));
  rstring_free(actual);
  rstring_array_free(rary);

  rary = rstring_array_new();
  rstring_array_push_cstr(rary, "");
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rfile_join(rary)));
  rstring_free(actual);
  rstring_array_free(rary);

  rary = rstring_array_new();
  rstring_array_push_cstr(rary, "");
  rstring_array_push_cstr(rary, "");
  TEST_ASSERT_EQUAL_RSTRING("/", (actual = rfile_join(rary)));
  rstring_free(actual);
  rstring_array_free(rary);

  /* There's actually no way to get an rstring_array with one of its members being NULL now. */
  /* rstring* strings4[2] = { rstring_new(NULL), rstring_new("") }; */
  /* TEST_ASSERT_NULL(rfile_join(strings4, 2)); */
  /* rstring_free(strings4[1]); */
}
