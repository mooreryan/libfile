#if defined(_WIN32)
#define FILE_SEPARATOR	'\\'
#else
#define FILE_SEPARATOR	'/'
#endif

char* file_basename(char* file_name);
char* file_dirname(char* file_name);
char* file_extname(char* file_name);
