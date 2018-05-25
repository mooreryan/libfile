#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "err_codes.h"

int main(int argc, char *argv[])
{
  char* filename = "/apple/pie.txt";

  char* basename = file_basename(filename);
  PANIC_MEM(stderr, basename);

  char* extname = file_extname(filename);
  PANIC_MEM(stderr, extname);

  printf("filename: '%s'\n", filename);
  printf("basename: '%s'\n", basename);
  printf("extname:  '%s'\n", extname);

  free(basename);
  free(extname);

  return 0;
}
