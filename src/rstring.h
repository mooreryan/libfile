#ifndef _RSTRING_H
#define _RSTRING_H

typedef struct tagbstring rstring;
typedef const struct tagbstring const_rstring;

#define RSTRING_ERR BSTR_ERR
#define RSTRING_OK BSTR_OK

/* Constructing */

rstring* rstring_new(const char* cstr);

int rstring_free(rstring* rstr);


/* Other */

rstring* rstring_chomp(const rstring* rstr);

rstring* rstring_downcase(const rstring* rstr);

int rstring_eql(const rstring* rstr1, const rstring* rstr2);

int rstring_length(const rstring* rstr);

rstring* rstring_slice1(const rstring* rstr, int index);

rstring* rstring_slice(const rstring* rstr, int index, int length);

rstring* rstring_upcase(const rstring* rstr);

#endif // _RSTRING_H
