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

#include "file.h"
#include "err_codes.h"

/**
 *
 * @brief Return the basename of a file.
 *
 * @code
 * char* extname = file_extname("/apple/pie");
 * @endcode
 *
 * @param filename The file name or the path
 * @return The basename of the file or path
 *
 * @note The caller must free the return value.
 */
char*
file_basename(char* filename)
{
  PANIC_MEM(stderr, filename);

  int i                = 0;
  int i_last_fs        = 0;
  int i_basename_start = 0;

  size_t slen         = strlen(filename);
  size_t basename_len = 0;

  char* basename = NULL;


  /* If we are passed an empty string.... */
  if (slen == 0) {
    basename = strdup("");
    PANIC_MEM(stderr, basename);

    return basename;
  }

  /* First find index of last trailing file separator. */
  for (i = slen - 1; i >= 0; --i) {
    if (filename[i] != FILE_SEPARATOR) {
      break;
    }
  }

  if (i < 0) {
    /* This can only happen for names like "////" */
    assert(filename[0] == FILE_SEPARATOR);

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

    for (i           = i_last_fs - 1; i >= 0; --i) {
      if (filename[i] == FILE_SEPARATOR) {
        break;
      }
    }
    i_basename_start = i + 1;
    basename_len     = i_last_fs - i_basename_start;

    basename = malloc(sizeof(char) * (basename_len + 1));
    PANIC_MEM(stderr, basename);

    for (i                 = 0; i < basename_len; ++i) {
      basename[i] = filename[i + i_basename_start];
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
 * @param filename The file name or the path
 * @return The extension of the file or path
 *
 * @note The caller must free the return value.
 */
char*
file_extname(char* filename)
{
  size_t basename_len = 0;

  char* basename = NULL;
  char* extname  = NULL;
  char* last_dot = NULL;

  basename     = file_basename(filename);
  basename_len = strlen(basename);

  last_dot = strrchr(basename, '.');

  if (/* No '.' was found */
      (last_dot == NULL) ||

      /* The last dot was the first char: ".profile" */
      (last_dot == basename) ||

      /* If the dot was the last thing in the filename: "foo." */
      (*last_dot == basename[basename_len - 1])) {

    extname = malloc(sizeof(char) * 2);
    strcpy(extname, "");
  }
  else {
    extname = malloc(sizeof(char) * (basename_len + 1));
    strcpy(extname, last_dot);
  }

  free(basename);

  return extname;
}
