#include <stdio.h>
#include <stdlib.h>

#include "rlib.h"

int main(int argc, char* argv[])
{

  rstring* str = rstring_new("apple");
  PANIC_MEM(stderr, str);

  puts(rstring_data(str));

  rstring* str2 = rstring_format("%s pie is %s", rstring_data(str), "good");
  PANIC_MEM(stderr, str2);

  puts(rstring_data(str2));

  return 0;
}
