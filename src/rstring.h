#ifndef _RSTRING_H
#define _RSTRING_H

typedef struct tagbstring rstring;
typedef const struct tagbstring const_rstring;

rstring* rstring_new(const char* cstr);
int rstring_free(rstring* rstr);

rstring* rstring_chomp(rstring* rstr);
int rstring_length(rstring* rstr);

#endif // _RSTRING_H
