/**
 * @file
 * @author Ryan Moore
 * @brief File containing functions like those found in Ruby's File class.
 * @see https://ruby-doc.org/core-2.5.0/File.html
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>

#include "file.h"
#include "err_codes.h"

int
index_before_first_trailing_file_separator(const char* fname)
{
  int i = 0;

  for (i = strlen(fname) - 1; i >= 0; --i) {
    if (fname[i] != FILE_SEPARATOR) {
      break;
    }
  }

  return i;
}

int
index_of_last_file_separator(const char* fname)
{
  int i = 0;

  for (i = strlen(fname) - 1; i >= 0; --i) {
    if (fname[i] == FILE_SEPARATOR) {
      break;
    }
  }

  return i;
}

int
index_of_last_file_separator_from_pos(const char* fname, int pos)
{
  int i = 0;

  assert(pos <= strlen(fname) - 1);

  for (i = pos; i >= 0; --i) {
    if (fname[i] == FILE_SEPARATOR) {
      break;
    }
  }

  return i;
}

char*
file_dirname(const char* fname)
{
  PANIC_MEM(stderr, fname);

  int i = 0;
  size_t slen = strlen(fname);

  char* dirname = NULL;

  if (slen == 0) {
    dirname = strdup("");
    PANIC_MEM(stderr, dirname);

    return dirname;
  }

  i = index_of_last_file_separator(fname);
  if (i < 0 && fname[0] == FILE_SEPARATOR) {
    /* Filenames like: "/////" */

    dirname = malloc(sizeof(char) * 2);
    PANIC_MEM(stderr, dirname);
    dirname[0] = FILE_SEPARATOR;
    dirname[1] = '\0';
  }
  else if (i < 0) {
    /* Filenames like: "apple" */

    dirname = malloc(sizeof(char) * 2);
    PANIC_MEM(stderr, dirname);
    dirname[0] = '.';
    dirname[1] = '\0';
  }
  else {
    /* Need to make sure that there aren't a bunch of fs chars right
       in a row. */
    i = index_of_last_file_separator_from_pos(fname, i);
    if (i < 0) {
      /* This could happen for filenames like: "/////apple" */
      assert(fname[0] == FILE_SEPARATOR);

      dirname = malloc(sizeof(char) * 2);
      PANIC_MEM(stderr, dirname);
      dirname[0] = FILE_SEPARATOR;
      dirname[1] = '\0';
    }
    else if (i == 0) {
      /* For fnames like: "/apple" */
      dirname = malloc(sizeof(char) * 2);
      PANIC_MEM(stderr, dirname);
      dirname[0] = FILE_SEPARATOR;
      dirname[1] = '\0';
    }
    else {
      int i_before_last_fs = i - 1;
      dirname = malloc(sizeof(char) * (i_before_last_fs + 2));
      PANIC_MEM(stderr, dirname);
      strncpy(dirname, fname, i_before_last_fs + 1);
      dirname[i_before_last_fs + 1] = '\0';
    }
  }

  return dirname;
}


/**
 *
 * @brief Return the basename of a file.
 *
 * @code
 * char* extname = file_extname("/apple/pie");
 * @endcode
 *
 * @param fname The file name or the path
 * @return The basename of the file or path
 *
 * @note The caller must free the return value.
 *
 * @todo Ruby's also takes an optional second param for extname to strip.
 */
char*
file_basename(const char* fname)
{
  PANIC_MEM(stderr, fname);

  int i                = 0;
  int i_last_fs        = 0;
  int i_basename_start = 0;

  size_t slen         = strlen(fname);
  size_t basename_len = 0;

  char* basename = NULL;


  /* If we are passed an empty string.... */
  if (slen == 0) {
    basename = strdup("");
    PANIC_MEM(stderr, basename);

    return basename;
  }

  i = index_before_first_trailing_file_separator(fname);

  if (i < 0) {
    /* This can only happen for names like "////" */
    assert(fname[0] == FILE_SEPARATOR);

    basename = malloc(sizeof(char) * 2);
    PANIC_MEM(stderr, basename);
    basename[0] = FILE_SEPARATOR;
    basename[1] = '\0';
  }
  else {
    /* The current index points to the char just before the first of
       trailing FILE_SEPARATOR chars if there are any, so the trailing
       FILE_SEPARATORs begin at i + 1. */
    i_last_fs = i + 1;

    for (i = i_last_fs - 1; i >= 0; --i) {
      if (fname[i] == FILE_SEPARATOR) {
        break;
      }
    }
    i_basename_start = i + 1;
    basename_len     = i_last_fs - i_basename_start;

    basename = malloc(sizeof(char) * (basename_len + 1));
    PANIC_MEM(stderr, basename);

    for (i = 0; i < basename_len; ++i) {
      basename[i] = fname[i + i_basename_start];
    }
    basename[basename_len] = '\0';
  }

  return basename;
}

/**
 *
 * @brief Return the extension of a file.
 *
 * @code
 * char* extname = file_extname("/apple/pie");
 * @endcode
 *
 * @param fname The file name or the path
 * @return The extension of the file or path
 *
 * @note The caller must free the return value.
 */
char*
file_extname(const char* fname)
{
  size_t basename_len = 0;

  char* basename = NULL;
  char* extname  = NULL;
  char* last_dot = NULL;

  basename     = file_basename(fname);
  basename_len = strlen(basename);

  last_dot = strrchr(basename, '.');

  if (/* No '.' was found */
      (last_dot == NULL) ||

      /* The last dot was the first char: ".profile" */
      (last_dot == basename) ||

      /* If the dot was the last thing in the fname: "foo." */
      (*last_dot == basename[basename_len - 1])) {

    extname = malloc(sizeof(char) * 2);
    strncpy(extname, "", 1);
    extname[1] = '\0';
  }
  else {
    extname = malloc(sizeof(char) * (basename_len + 1));
    strncpy(extname, last_dot, basename_len);
    extname[basename_len] = '\0';
  }

  free(basename);

  return extname;
}

/**
 *
 * @brief Returns true if the named file is a directory, and false otherwise.
 *
 * @code
 * mkdir("apple", S_IRUSR | S_IWUSR | S_IXUSR);
 * TEST_ASSERT_TRUE(file_is_directory("apple"));
 *
 * rmdir("apple");
 * TEST_ASSERT_FALSE(file_is_directory("apple"));
 * @endcode
 *
 * @param fname The file name or the path
 * @return 1 if fname is a directory, 0 otherwise.
 *
 * @todo Ruby also returns true if fname is a symlink that points at a directory.
 */
int
file_is_directory(const char* fname)
{
  struct stat st;

  if (stat(fname, &st) < 0) {
    return 0;
  }
  else {
    return S_ISDIR(st.st_mode);
  }
}

/**
 *
 * @brief Returns true if the named file exists and is a regular file, and false otherwise.
 *
 * @code
 * TEST_ASSERT_TRUE(file_is_file("file.h"));
 *
 * remove("file.h");
 * TEST_ASSERT_FALSE(file_is_file("file.h"));
 * @endcode
 *
 * @param fname The file name or the path
 * @return True if file exists and is regular file.
 *
 * @todo Ruby also returns true if fname is a symlink that points at a file.
 */
int
file_is_file(const char* fname)
{
  struct stat st;

  if (stat(fname, &st) < 0) {
    return 0;
  }
  else {
    return S_ISREG(st.st_mode);
  }
}
