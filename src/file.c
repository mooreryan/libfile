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
  char* last_dot = strrchr(basename, '.');

  if (/* No '.' was found */
      (last_dot == NULL) ||

      /* The last dot was the first char: ".profile" */
      (last_dot == basename) ||

      /* If the dot was the last thing in the file_name: "foo." */
      (*last_dot == basename[strlen(basename) - 1])) {
    return "";
  }
  else {
    return last_dot;
  }
}

char*
file_basename(char* file_name)
{
  char* last_sep = strrchr(file_name, FILE_SEPARATOR);

  if (last_sep == NULL) {
    return file_name;
  }
  else {
    return last_sep + 1;
  }
}
