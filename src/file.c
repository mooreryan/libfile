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
  assert(fname != NULL);

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
  assert(fname != NULL);

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
  assert(fname != NULL);
  assert(pos <= strlen(fname) - 1);

  int i = 0;

  for (i = pos; i >= 0; --i) {
    if (fname[i] == FILE_SEPARATOR) {
      break;
    }
  }

  return i;
}

char*
make_simple_string(char c)
{
  char* str = malloc(sizeof(char) * 2);
  if (str == NULL) { return NULL; }

  str[0] = c;
  str[1] = '\0';

  return str;
}

char*
make_empty_string(void)
{
  char* str = strdup("");
  if (str == NULL) { return NULL; }

  return str;
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
 *
 * TEST_ASSERT_FALSE(file_is_directory(NULL));
 * @endcode
 *
 * @param fname The file name or the path.
 * @return 1 if fname is a directory, 0 otherwise.  If the fname was NULL, 0 is returned.
 *
 * @todo Ruby also returns true if fname is a symlink that points at a directory.
 */
int
file_is_directory(const char* fname)
{
  if (fname == NULL) { return 0; }

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
 *
 * TEST_ASSERT_FALSE(file_is_file(NULL));
 * @endcode
 *
 * @param fname The file name or the path.
 * @return True if file exists and is regular file.  If the fname was NULL, 0 is returned.
 *
 * @todo Ruby also returns true if fname is a symlink that points at a file.
 */
int
file_is_file(const char* fname)
{
  if (fname == NULL) { return 0; }

  struct stat st;

  if (stat(fname, &st) < 0) {
    return 0;
  }
  else {
    return S_ISREG(st.st_mode);
  }
}

/**
 *
 * @brief Return the dirname of a file.
 *
 * Returns all components of the filename given in file_name except the last one (after first stripping trailing separators).
 *
 * @code
 * TEST_ASSERT_EQUAL_STRING("/apple", file_dirname("/apple/pie.txt"));
 * @endcode
 *
 * @param fname The file name or the path.  NULL is returned if param is NULL.
 * @return The dirname of the file or path or NULL if there were errors.
 *
 * @note The caller must free the return value.
 * @warning The caller must check the return value for NULL.
 */
char*
file_dirname(const char* fname)
{
  if (fname == NULL) { return NULL; }

  int i = 0;
  size_t slen = strlen(fname);

  char* dirname = NULL;

  if (slen == 0) {
    dirname = make_empty_string();

    return dirname;
  }

  i = index_of_last_file_separator(fname);
  if (i < 0 && fname[0] == FILE_SEPARATOR) {
    /* Filenames like: "/////" */
    dirname = make_simple_string(FILE_SEPARATOR);
  }
  else if (i < 0) {
    /* Filenames like: "apple" */
    dirname = make_simple_string('.');
  }
  else {

    /* There was a basename.  Now we need to make sure that there
       aren't a bunch of fs chars right in a row. */
    i = index_of_last_file_separator_from_pos(fname, i);

    if (i <= 0) {
      /* This could happen for filenames like: "/////apple" or "/apple" */
      dirname = make_simple_string(FILE_SEPARATOR);
    }
    else {
      int i_before_last_fs = i - 1;

      dirname = malloc(sizeof(char) * (i_before_last_fs + 2));
      if (dirname == NULL) { return NULL; }

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
 * @param fname The file name or the path.  NULL is returned if param is NULL.
 * @return The basename of the file or path or NULL if there were errors.
 *
 * @note The caller must free the return value.
 * @warning The caller must check the return value for NULL.
 *
 * @todo Ruby's also takes an optional second param for extname to strip.
 */
char*
file_basename(const char* fname)
{
  if (fname == NULL) { return NULL; }

  int i                = 0;
  int i_last_fs        = 0;
  int i_basename_start = 0;

  size_t slen         = strlen(fname);
  size_t basename_len = 0;

  char* basename = NULL;


  /* If we are passed an empty string.... */
  if (slen == 0) {
    basename = make_empty_string();

    return basename;
  }

  i = index_before_first_trailing_file_separator(fname);

  if (i < 0) {
    /* This can only happen for names like "////" */
    assert(fname[0] == FILE_SEPARATOR);
    basename = make_simple_string(FILE_SEPARATOR);
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
    if (basename == NULL) { return NULL; }

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
 * @param fname The file name or the path.  NULL is returned if param is NULL.
 * @return The extension of the file or path or NULL if there were errors.
 *
 * @note The caller must free the return value.
 * @warning The caller must check the return value for NULL.
 */
char*
file_extname(const char* fname)
{
  if (fname == NULL) { return NULL; }

  size_t basename_len = 0;

  char* basename = NULL;
  char* extname  = NULL;
  char* last_dot = NULL;

  basename     = file_basename(fname);
  if (basename == NULL) { return NULL; }

  basename_len = strlen(basename);

  last_dot = strrchr(basename, '.');

  if (/* No '.' was found */
      (last_dot == NULL) ||

      /* The last dot was the first char: ".profile" */
      (last_dot == basename) ||

      /* If the dot was the last thing in the fname: "foo." */
      (*last_dot == basename[basename_len - 1])) {

    extname = malloc(sizeof(char) * 2);
    if (extname == NULL) { return NULL; }

    strncpy(extname, "", 1);
    extname[1] = '\0';
  }
  else {
    extname = malloc(sizeof(char) * (basename_len + 1));
    if (extname == NULL) { return NULL; }

    strncpy(extname, last_dot, basename_len);
    extname[basename_len] = '\0';
  }

  free(basename);

  return extname;
}

/**
 *
 * @brief Returns a new path by joining the strings using the FILE_SEPARATOR.
 *
 * @code
char* actual = NULL;
const char* strings1[3] = { "apple", "pie", "good" };

TEST_ASSERT_EQUAL_STRING("apple/pie/good", (actual = file_join(strings1, 3)));

free(actual);
 * @endcode
 *
 * @param strings Array of char arrays
 * @param num_strings The number of elems in the strings array
 *
 * @return The path or NULL if there are errors or any of the strings are NULL or the strings array itself is null.
 *
 * @note Ruby has some weird behavior here where it sometimes keeps double // and sometimes it doesn't.  It doesn't in cases where there is more than one slash in a single one of the input strings.  Regardless, the function is for building paths so in that spirit, we don't want those double separators anyways. Soooo, it differs from Ruby in these cases.
 *
 * @note The caller must free the return value.
 * @warning The caller must check the return value for NULL.
 *
 * @todo the no_doubles path will use a bit more memory than it actually needs if some of the double // are removed.
 * @todo Clean this up...
 */
char*
file_join(const char** strings, int num_strings)
{
  if (strings == NULL) { return NULL; }
  if (num_strings < 1) { return NULL; }

  int i = 0;
  int j = 0;

  /* Include enough space for the number of file_separators as well as
     the null char */
  int path_len = num_strings;

  char* path = NULL;

  for (i = 0; i < num_strings; ++i) {
    if (strings[i] == NULL) { return NULL; }

    path_len += strlen(strings[i]);
  }

  path = malloc(sizeof(char) * path_len);
  if (path == NULL) { return NULL; }

  int total_i = 0;
  for (i = 0; i < num_strings; ++i) {
    if (strlen(strings[i]) > 0) {
      for (j = 0; j < strlen(strings[i]); ++j) {
        path[total_i++] = strings[i][j];
      }
    }
    if (i < num_strings - 1) {
      path[total_i++] = FILE_SEPARATOR;
    }
  }
  path[total_i++] = '\0';

  char* no_doubles = malloc(sizeof(char) * (total_i + 1));
  if (no_doubles == NULL) { return NULL; }

  j = 0;
  no_doubles[j++] = path[0];
  for (i = 1; i < total_i-1; ++i) {
    if (path[i] != FILE_SEPARATOR ||
        (path[i] == FILE_SEPARATOR && path[i-1] != FILE_SEPARATOR)) {
      no_doubles[j++] = path[i];
    }
  }
  no_doubles[j++] = '\0';

  free(path);
  return no_doubles;
}
