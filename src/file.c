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
  int i = 0;

  size_t slen = strlen(file_name);

  char* new_file_name = NULL;

  /* First strip trailing separators. */
  for (i = slen - 1; i >= 0; --i) {
    if (file_name[i] != FILE_SEPARATOR) {
      break;
    }
  }
  slen = i + 1;

  /* The current index points to the char just before the first of
     trailing FILE_SEPARATOR chars if there are any, so the trailing
     FILE_SEPARATORs begin at i + 1.

     We want to copy all bytes up until that first separator.
 */
  new_file_name = malloc(sizeof(char) * (slen + 1));
  PANIC_MEM(stderr, new_file_name);
  memcpy(new_file_name, file_name, slen);
  new_file_name[slen] = '\0';

  char* last_sep = strrchr(new_file_name, FILE_SEPARATOR);

  if (last_sep == NULL) {
    return new_file_name;
  }
  else {
    size_t basename_len = strlen(last_sep);
    char* basename = malloc(sizeof(char) * basename_len);
    PANIC_MEM(stderr, basename);
    strcpy(basename, last_sep+1);
    basename[basename_len] = '\0';
    free(new_file_name);

    return basename;
  }
}
