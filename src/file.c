#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <bstring.h>

#include "file.h"
#include "err_codes.h"

#if defined(_WIN32)
#define FILE_SEPARATOR	'\\'
#else
#define FILE_SEPARATOR	'/'
#endif

char*
file_extname(char* file_name)
{
  char* basename = file_basename(file_name);
  size_t basename_len = strlen(basename);
  char* last_dot = strrchr(basename, '.');
  char* extname = NULL;

  if (/* No '.' was found */
      (last_dot == NULL) ||

      /* The last dot was the first char: ".profile" */
      (last_dot == basename) ||

      /* If the dot was the last thing in the file_name: "foo." */
      (*last_dot == basename[basename_len - 1])) {

    extname = malloc(sizeof(char));
    strcpy(extname, "");
  }
  else {
    extname = malloc(sizeof(char) * (basename_len + 1));
    strcpy(extname, last_dot);
  }

  return extname;
}

char*
file_basename(char* file_name)
{
  PANIC_MEM(stderr, file_name);

  int i = 0;
  int i_last_fs = 0;
  int i_basename_start = 0;

  size_t slen = strlen(file_name);
  size_t basename_len = 0;

  char* basename = NULL;



  /* If we are passed an empty string.... */
  if (slen == 0) {
    basename = strdup("");
    PANIC_MEM(stderr, basename);

    return basename;
  }

  /* First strip trailing separators. */
  for (i = slen - 1; i >= 0; --i) {
    if (file_name[i] != FILE_SEPARATOR) {
      break;
    }
  }

  if (i < 0) {
    /* This can only happen for names like "////" */
    assert(file_name[0] == FILE_SEPARATOR);

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
      if (file_name[i] == FILE_SEPARATOR) {
        break;
      }
    }
    i_basename_start = i + 1;
    basename_len = i_last_fs - i_basename_start;

    basename = malloc(sizeof(char) * (basename_len + 1));
    PANIC_MEM(stderr, basename);

    for (i = 0; i < basename_len; ++i) {
      basename[i] = file_name[i + i_basename_start];
    }
    basename[basename_len] = '\0';
  }

  return basename;
}
