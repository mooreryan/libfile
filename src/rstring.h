#ifndef _RSTRING_H
#define _RSTRING_H

typedef struct tagbstring rstring;
typedef const struct tagbstring const_rstring;

/* Constructing */

rstring* rstring_new(const char* cstr);

int rstring_free(rstring* rstr);


/* Other */

rstring* rstring_chomp(const rstring* rstr);

int rstring_eql(const rstring* rstr1, const rstring* rstr2);

int rstring_length(const rstring* rstr);

rstring* rstring_slice1(const rstring* rstr, int index);

rstring* rstring_slice(const rstring* rstr, int index, int length);

#endif // _RSTRING_H
