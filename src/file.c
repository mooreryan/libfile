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
  char* last_dot = strrchr(file_name, '.');
  char* last_sep = strrchr(file_name, FILE_SEPARATOR);


  if (/* No '.' was found */
      (last_dot == NULL) ||

      /* The last dot was the first char: ".profile" */
      (last_dot == file_name) ||

      /* If the dot was the last thing in the file_name: "foo." */
      (*last_dot == file_name[strlen(file_name) - 1]) ||

      /* The last dot is somewhere in the directory: ".apple/pie" */
      (last_sep && strlen(last_sep) < strlen(last_dot)) ||

      /* Last dot is directly after /: /.foo */
      (*(last_dot - 1) == '/')

      ) {
    return "";
  }
  else {
    return last_dot;
  }
}
