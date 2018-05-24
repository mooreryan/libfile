#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "file.h"

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

  size_t len = strlen(file_name);

  char* new_name = NULL;

  /* First strip trailing separators. */
  for (i = len - 1; i >= 0; --i) {
    if (file_name[i] != FILE_SEPARATOR) {

      break;
    }
  }
  /* The current index points to the char just before the first of
     trailing FILE_SEPARATOR chars if there are any, so move it
     forward by 1.*/
  ++i;

  if (i < len) { /* Then we trimmed off some stuff */
    new_name = malloc(sizeof(char) * (i + 1));
    memcpy(new_name, file_name, i);
    new_name[i] = '\0';
  }
  else {
    new_name = malloc(sizeof(char) * (len + 1));
    strcpy(new_name, file_name);

    /* Make sure the null char gets on there. */
    new_name[i] = '\0';
  }

  char* last_sep = strrchr(new_name, FILE_SEPARATOR);

  if (last_sep == NULL) {
    return new_name;
  }
  else {
    return last_sep + 1;
  }
}
