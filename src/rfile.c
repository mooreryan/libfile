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
#include "rstring.h"
#include "err_codes.h"
#include "bstrlib.h"

static int
index_before_first_trailing_file_separator(const rstring* fname)
{
  if (rstring_bad(fname)) { return RERROR; }

  int i = 0;

  for (i = rstring_length(fname) - 1; i >= 0; --i) {
    if (rstring_char_at(fname, i) != RFILE_SEPARATOR) {
      break;
    }
  }

  return i;
}

static int
index_of_last_file_separator_from_pos(const rstring* fname, int pos)
{
  if (rstring_bad(fname) || pos < 0 || pos >= rstring_length(fname)) {
    return RERROR;
  }
  int i = 0;

  for (i = pos; i >= 0; --i) {
    if (rstring_char_at(fname, i) == RFILE_SEPARATOR) {
      break;
    }
  }

  return i;
}

static int
index_of_last_file_separator(const rstring* fname)
{
  if (rstring_bad(fname)) { return RERROR; }

  int pos = rstring_length(fname) - 1;

  return index_of_last_file_separator_from_pos(fname, pos);
}


/**
 * @brief Tells whether a file exists or not (i.e., stat is successful).
 *
 * @param fname An rstring with the filename to check.
 *
 * @retval RTRUE The file exists
 * @retval RFALSE The file does not exist
 * @retval RERROR fname is invalid or there was an error.
 */
int
rfile_exist(const rstring* fname)
{
  if (rstring_bad(fname)) { return RERROR; }

  int ret_val = 0;
  char* cfname = bstr2cstr(fname, '?');
  if (cfname == NULL) { return RERROR; }

  struct stat st;
  ret_val = stat(cfname, &st);

  free(cfname);

  if (ret_val < 0) {
    return RFALSE;
  }
  else {
    return RTRUE;
  }
}

/**
 * @brief Tells whether the named directory exists.
 *
 * @param fname An rstring with the dirname to check.
 *
 * @retval RTRUE The dir exists
 * @retval RFALSE The dir does not exist
 * @retval RERROR The input rstring is invalid or there was an error.
 *
 * @todo Ruby also returns true if dir is a symlink that points at a directory.
 */
int
rfile_is_directory(const rstring* fname)
{
  if (rstring_bad(fname)) { return RERROR; }

  int ret_val = 0;
  char* cfname = bstr2cstr(fname, '?');
  if (cfname == NULL) { return RERROR; }

  struct stat st;

  ret_val = stat(cfname, &st);

  bcstrfree(cfname);

  if (ret_val < 0 || !S_ISDIR(st.st_mode)) {
    return RFALSE;
  }
  else {
    return RTRUE;
  }
}

/**
 * @brief Tells whether the named file exists and is a regular file.
 *
 * @param fname An rstring with the dirname to check.
 *
 * @retval RTRUE The file exists
 * @retval RFALSE The file does not exist
 * @retval RERROR The input rstring is invalid or there was an error.
 *
 * @todo Ruby also returns true if file is a symlink that points at a file.
 */
int
rfile_is_file(const rstring* fname)
{
  if (rstring_bad(fname)) { return RERROR; }

  int ret_val = 0;
  char* cfname = bstr2cstr(fname, '?');
  if (cfname == NULL) { return RERROR; }

  struct stat st;

  ret_val = stat(cfname, &st);

  bcstrfree(cfname);

  if (ret_val < 0 || !S_ISREG(st.st_mode)) {
    return RFALSE;
  }
  else {
    return RTRUE;
  }
}

/**
 * @brief Return the basename of a file.
 *
 * @param fname An rstring with the file name.
 *
 * @retval rstring* A valid rstring containing the basename of the file.
 * @retval NULL The input rstring is invalid or there was an error.
 *
 * @warning The caller must free the result.
 *
 * @todo Ruby's also takes an optional second param for extname to strip.
 */
rstring*
rfile_basename(const rstring* fname)
{
  if (rstring_bad(fname)) { return NULL; }

  int i                = 0;
  int i_last_fs        = 0;
  int i_basename_start = 0;
  int char_at = 0;

  int slen         = rstring_length(fname);
  if (slen == RERROR) { return NULL; }

  int basename_len = 0;

  rstring* basename = NULL;

  /* If we are passed an empty string.... */
  if (slen == 0) {
    basename = rstring_new("");
    if (rstring_bad(basename)) { return NULL; }

    return basename;
  }

  i = index_before_first_trailing_file_separator(fname);

  if (i < 0) {
    /* This can only happen for names like "////" */
    char_at = rstring_char_at(fname, 0);
    if (char_at == RERROR || char_at != RFILE_SEPARATOR) {
      return NULL;
    }

    basename = rstring_new(RFILE_SEPARATOR_STR);
    if (rstring_bad(basename)) { return NULL; }
  }
  else {
    /* The current index points to the char just before the first of
       trailing RFILE_SEPARATOR chars if there are any, so the trailing
       RFILE_SEPARATORs begin at i + 1. */
    i_last_fs = i + 1;

    for (i = i_last_fs - 1; i >= 0; --i) {
      char_at = rstring_char_at(fname, i);
      if (char_at == RFILE_SEPARATOR) {
        break;
      }
    }
    i_basename_start = i + 1;
    basename_len     = i_last_fs - i_basename_start;

    basename = rstring_slice(fname, i_basename_start, basename_len);
    if (rstring_bad(basename)) { return NULL; }
  }

  return basename;
}


/**
 * @brief Return the dirname of a file.
 *
 * Returns all components of the filename given in file_name except the last one (after first stripping trailing separators).
 *
 * @param fname An rstring with the file name.
 *
 * @retval rstring* A valid rstring containing the directory of the file.
 * @retval NULL The input rstring is invalid or there was an error.
 *
 * @warning The caller must free the result.
 */
rstring*
rfile_dirname(const rstring* fname)
{
  if (rstring_bad(fname)) { return NULL; }

  int i = 0;
  int char_at = 0;
  int slen = rstring_length(fname);
  if (slen == RERROR) { return NULL; }

  rstring* dirname = NULL;

  if (slen == 0) {
    dirname = rstring_new("");
    if (rstring_bad(dirname)) { return NULL; }

    return dirname;
  }

  i = index_of_last_file_separator(fname);
  char_at = rstring_char_at(fname, 0);
  if (char_at == RERROR) { return NULL; }

  if (i < 0 && char_at == RFILE_SEPARATOR) {
    /* Filenames like: "/////" */
    dirname = rstring_new(RFILE_SEPARATOR_STR);
    if (rstring_bad(dirname)) { return NULL; }
  }
  else if (i < 0) {
    /* Filenames like: "apple" */
    dirname = rstring_new(".");
    if (rstring_bad(dirname)) { return NULL; }
  }
  else {

    /* There was a basename.  Now we need to make sure that there
       aren't a bunch of fs chars right in a row. */
    i = index_of_last_file_separator_from_pos(fname, i);

    if (i <= 0) {
      /* This could happen for filenames like: "/////apple" or "/apple" */
      dirname = rstring_new(RFILE_SEPARATOR_STR);
      if (rstring_bad(dirname)) { return NULL; }
    }
    else {
      int i_before_last_fs = i - 1;
      dirname = rstring_slice(fname, 0, i_before_last_fs + 1);
      if (rstring_bad(dirname)) { return NULL; }
    }
  }

  return dirname;
}


/**
 * @brief Return the extension of a file.
 *
 * @param fname An rstring with the file name.
 *
 * @retval rstring* A valid rstring containing the extension of the file.
 * @retval NULL The input rstring is invalid or there was an error.
 *
 * @warning The caller must free the result.
 */
rstring*
rfile_extname(const rstring* fname)
{
  if (rstring_bad(fname)) { return NULL; }

  int basename_len = 0;
  int last_dot = 0;

  rstring* basename = NULL;
  rstring* extname  = NULL;

  basename     = rfile_basename(fname);
  if (rstring_bad(basename)) { return NULL; }

  basename_len = rstring_length(basename);
  if (basename_len == RERROR) { return NULL; }

  last_dot = bstrrchr((const_bstring)basename, '.');

  if (/* No '.' was found */
      (last_dot == BSTR_ERR) ||

      /* The last dot was the first char: ".profile" */
      (last_dot == 0) ||

      /* If the dot was the last thing in the fname: "foo." */
      (last_dot == basename_len - 1)) {

    extname = rstring_new("");
    if (rstring_bad(extname)) { return NULL; }
  }
  else {
    extname = rstring_slice(basename, last_dot, basename_len);
    if (rstring_bad(extname)) { return NULL; }
  }

  rstring_free(basename);

  return extname;
}

/**
 * @brief Returns a new path by joining the strings using the RFILE_SEPARATOR.
 *
 * @param rary An rstring_array with the rstrings to join.
 *
 * @retval rstring* A valid rstring with the input strings all joined up.
 * @retval NULL The input rstring_array is invalid or there was an error.
 *
 * @note Ruby has some weird behavior here where it sometimes keeps double // and sometimes it doesn't.  It doesn't in cases where there is more than one slash in a single one of the input strings.  Regardless, the function is for building paths so in that spirit, we don't want those double separators anyways. Soooo, it differs from Ruby in these cases.
 *
 * @warning The caller must free the result.
 *
 * @todo the no_doubles path will use a bit more memory than it actually needs if some of the double // are removed.
 */
rstring*
rfile_join(rstring_array* rary)
{
  if (rstring_array_bad(rary)) { return NULL; }

  int i = 0;
  int j = 0;
  int len = 0;
  int char_at = 0;

  /* Check if any of the rstrings are null */
  for (i = 0; i < rary->qty; ++i) {
    if (rstring_array_get(rary, i) == NULL) { return NULL; }
  }

  rstring* path = NULL;
  rstring* sep = NULL;
  int c = 0;
  int c_minus_1 = 0;

  sep = rstring_new(RFILE_SEPARATOR_STR);
  if (rstring_bad(sep)) { return NULL; }

  /* Clean up path if early exit from here down. */
  path = rstring_array_join(rary, sep);
  if (rstring_bad(path)) { return NULL; }
  rstring_free(sep);

  /* If it's empty, there are no doubles. */
  len = rstring_length(path);
  if (len == RERROR) { rstring_free(path); return NULL; }
  if (len == 0) { return path; }


  /* Remove doubles of file sep. */
  /* Clean up no_doubles if early exit from here down. */
  char* no_doubles = malloc(sizeof(char) * ((len + 1)));
  if (no_doubles == NULL) { rstring_free(path); return NULL; }

  j = 0;
  char_at = rstring_char_at(path, 0);
  if (char_at == RERROR) { rstring_free(path); free(no_doubles); return NULL; }

  no_doubles[j++] = char_at;
  for (i = 1; i < len; ++i) {
    c = rstring_char_at(path, i);
    c_minus_1 = rstring_char_at(path, i - 1);
    if (c == RERROR || c_minus_1 == RERROR) {
      rstring_free(path); free(no_doubles); return NULL;
    }

    if (c != RFILE_SEPARATOR || c_minus_1 != RFILE_SEPARATOR) {
      no_doubles[j++] = c;
    }
  }
  no_doubles[j] = '\0';

  rstring_free(path);

  /* Rewrite the path */
  path = rstring_new(no_doubles);
  if (rstring_bad(path)) { free(no_doubles); return NULL; }
  free(no_doubles);

  return path;
}
