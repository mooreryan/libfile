#ifndef _RSTRING_H
#define _RSTRING_H

typedef struct tagbstring rstring;
typedef const struct tagbstring const_rstring;

/* Constructing */

rstring*
rstring_new(const char* cstr);

int
rstring_free(rstring* rstr);


/* Other */

rstring*
rstring_chomp(rstring* rstr);

int
rstring_length(rstring* rstr);

rstring*
rstring_slice1(rstring* rstr, int index);

rstring*
rstring_slice(rstring* rstr, int index, int length);

#endif // _RSTRING_H
