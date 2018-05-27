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

#include "rfile.h"
#include "err_codes.h"
#include "bstrlib.h"
#include "rstring.h"

int
index_before_first_trailing_file_separator(const rstring* fname)
{
  if (fname == NULL) { return RSTR_ERR; }

  int i = 0;

  for (i = rstring_length(fname) - 1; i >= 0; --i) {
    if (rstring_char_at(fname, i) != RFILE_SEPARATOR) {
      break;
    }
  }

  return i;
}

int
index_of_last_file_separator_from_pos(const rstring* fname, int pos)
{
  if (fname == NULL || pos < 0 || pos >= rstring_length(fname)) {
    return RSTR_ERR;
  }
  int i = 0;

  for (i = pos; i >= 0; --i) {
    if (rstring_char_at(fname, i) == RFILE_SEPARATOR) {
      break;
    }
  }

  return i;
}

int
index_of_last_file_separator(const rstring* fname)
{
  if (fname == NULL) { return RSTR_ERR; }

  int pos = rstring_length(fname) - 1;

  return index_of_last_file_separator_from_pos(fname, pos);
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
 * @param fname The file name or the path.
 * @return 1 if fname is a directory, 0 otherwise.  If the fname was NULL, 0 is returned.
 *
 * @todo Ruby also returns true if fname is a symlink that points at a directory.
 */
int
rfile_is_directory(const rstring* fname)
{
  if (fname == NULL) { return 0; }

  int ret_val = 0;
  char* cfname = bstr2cstr(fname, '?');
  if (cfname == NULL) { return 0; }

  struct stat st;

  ret_val = stat(cfname, &st);

  bcstrfree(cfname);

  if (ret_val < 0) {
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
 * @param fname The file name or the path.
 * @return True if file exists and is regular file.  If the fname was NULL, 0 is returned.
 *
 * @todo Ruby also returns true if fname is a symlink that points at a file.
 */
int
rfile_is_file(const rstring* fname)
{
  if (fname == NULL) { return 0; }

  int ret_val = 0;
  char* cfname = bstr2cstr(fname, '?');
  if (cfname == NULL) { return 0; }

  struct stat st;

  ret_val = stat(cfname, &st);

  bcstrfree(cfname);

  if (ret_val < 0) {
    return 0;
  }
  else {
    return S_ISREG(st.st_mode);
  }
}

/**
 *
 * @brief Return the basename of a file.
 *
 * @param fname The file name or the path.  NULL is returned if param is NULL.
 * @return The basename of the file or path or NULL if there were errors.
 *
 * @note The caller must free the return value.
 * @warning The caller must check the return value for NULL.
 *
 * @todo Ruby's also takes an optional second param for extname to strip.
 */
rstring*
rfile_basename(const rstring* fname)
{
  if (fname == NULL) { return NULL; }

  int i                = 0;
  int i_last_fs        = 0;
  int i_basename_start = 0;

  int slen         = rstring_length(fname);
  if (slen == RSTR_ERR) { return NULL; }

  int basename_len = 0;

  rstring* basename = NULL;


  /* If we are passed an empty string.... */
  if (slen == 0) {
    basename = rstring_new("");
    if (basename == NULL) { return NULL; }

    return basename;
  }

  i = index_before_first_trailing_file_separator(fname);

  if (i < 0) {
    /* This can only happen for names like "////" */
    if (rstring_char_at(fname, 0) != RFILE_SEPARATOR) {
      return NULL;
    }

    basename = rstring_new(RFILE_SEPARATOR_STR);
    if (basename == NULL) { return NULL; }
  }
  else {
    /* The current index points to the char just before the first of
       trailing RFILE_SEPARATOR chars if there are any, so the trailing
       RFILE_SEPARATORs begin at i + 1. */
    i_last_fs = i + 1;

    for (i = i_last_fs - 1; i >= 0; --i) {
      if (rstring_char_at(fname, i) == RFILE_SEPARATOR) {
        break;
      }
    }
    i_basename_start = i + 1;
    basename_len     = i_last_fs - i_basename_start;

    basename = rstring_slice(fname, i_basename_start, basename_len);
    if (basename == NULL) { return NULL; }
  }

  return basename;
}

/**
 *
 * @brief Return the dirname of a file.
 *
 * Returns all components of the filename given in file_name except the last one (after first stripping trailing separators).
 *
 * @param fname The file name or the path.  NULL is returned if param is NULL.
 * @return The dirname of the file or path or NULL if there were errors.
 *
 * @note The caller must free the return value.
 * @warning The caller must check the return value for NULL.
 */
rstring*
rfile_dirname(const rstring* fname)
{
  if (fname == NULL) { return NULL; }

  int i = 0;
  int slen = rstring_length(fname);

  rstring* dirname = NULL;

  if (slen == 0) {
    dirname = rstring_new("");

    return dirname;
  }

  i = index_of_last_file_separator(fname);
  if (i < 0 && rstring_char_at(fname, 0) == RFILE_SEPARATOR) {
    /* Filenames like: "/////" */
    dirname = rstring_new(RFILE_SEPARATOR_STR);
  }
  else if (i < 0) {
    /* Filenames like: "apple" */
    dirname = rstring_new(".");
  }
  else {

    /* There was a basename.  Now we need to make sure that there
       aren't a bunch of fs chars right in a row. */
    i = index_of_last_file_separator_from_pos(fname, i);

    if (i <= 0) {
      /* This could happen for filenames like: "/////apple" or "/apple" */
      dirname = rstring_new(RFILE_SEPARATOR_STR);
    }
    else {
      int i_before_last_fs = i - 1;
      dirname = rstring_slice(fname, 0, i_before_last_fs + 1);
    }
  }

  return dirname;
}


/**
 *
 * @brief Return the extension of a file.
 *
 * @param fname The file name or the path.  NULL is returned if param is NULL.
 * @return The extension of the file or path or NULL if there were errors.
 *
 * @note The caller must free the return value.
 * @warning The caller must check the return value for NULL.
 */
rstring*
rfile_extname(const rstring* fname)
{
  if (fname == NULL) { return NULL; }

  int basename_len = 0;
  int last_dot = 0;

  rstring* basename = NULL;
  rstring* extname  = NULL;

  basename     = rfile_basename(fname);
  if (basename == NULL) { return NULL; }

  basename_len = rstring_length(basename);

  last_dot = bstrrchr((const_bstring)basename, '.');

  if (/* No '.' was found */
      (last_dot == BSTR_ERR) ||

      /* The last dot was the first char: ".profile" */
      (last_dot == 0) ||

      /* If the dot was the last thing in the fname: "foo." */
      (last_dot == basename_len - 1)) {

    extname = rstring_new("");
    if (extname == NULL) { return NULL; }
  }
  else {
    extname = rstring_slice(basename, last_dot, basename_len);
    if (extname == NULL) { return NULL; }
  }

  rstring_free(basename);

  return extname;
}

/**
 *
 * @brief Returns a new path by joining the strings using the RFILE_SEPARATOR.
 *
 * @code
char* actual = NULL;
const char* strings1[3] = { "apple", "pie", "good" };

TEST_ASSERT_EQUAL_STRING("apple/pie/good", (actual = rfile_join(strings1, 3)));

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
rstring*
rfile_join(rstring** rstrings, int num_rstrings)
{
  if (rstrings == NULL) { return NULL; }
  if (num_rstrings < 1) { return NULL; }

  int i = 0;
  int j = 0;

  /* Check if any of the rstrings are null */
  for (i = 0; i < num_rstrings; ++i) {
    if (rstrings[i] == NULL) { return NULL; }
  }

  rstring* path = NULL;
  rstring* sep = NULL;
  rstring_array* rary = NULL;
  unsigned char c = '\0';

  rary = rstring_array_new(rstrings, num_rstrings);
  if (rary == NULL) { return NULL; }

  sep = rstring_new(RFILE_SEPARATOR_STR);
  path = rstring_array_join(rary, sep);
  rstring_free(sep);
  rstring_array_free(rary);

  /* If it's empty, there are no doubles. */
  if (rstring_length(path) == 0) {
    return path;
  }

  /* Remove doubles of file sep. */
  char* no_doubles = malloc(sizeof(char) * ((rstring_length(path) + 1)));
  if (no_doubles == NULL) { return NULL; }
  j = 0;
  no_doubles[j++] = rstring_char_at(path, 0);
  for (i = 1; i < rstring_length(path); ++i) {
    c = rstring_char_at(path, i);
    if (c != RFILE_SEPARATOR || rstring_char_at(path, i - 1) != RFILE_SEPARATOR) {
      no_doubles[j++] = c;
    }
  }
  no_doubles[j] = '\0';

  rstring_free(path);

  path = rstring_new(no_doubles);
  if (path == NULL) { return NULL; }
  free(no_doubles);

  return path;
}
