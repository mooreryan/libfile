#ifndef _RSTRING_H
#define _RSTRING_H

typedef struct tagbstring rstring;

typedef struct bstrList rstring_array;

#define RSTRING_ERR BSTR_ERR
#define RSTRING_OK BSTR_OK

/**
 * @brief Return the char at given position
 *
 * @param rstring* rstr
 * @param int index
 *
 * @returns The char at the position.  Or '\0' if the position doesn't exist ro the rstring is NULL.
 */
/* #define rstring_char_at bchar */

/* Constructing */

rstring* rstring_new(const char* cstr);

int rstring_free(rstring* rstr);


/* Other */

char rstring_char_at(const rstring* rstr, int index);

rstring* rstring_chomp(const rstring* rstr);

rstring* rstring_downcase(const rstring* rstr);

int rstring_eql(const rstring* rstr1, const rstring* rstr2);

int rstring_length(const rstring* rstr);

rstring* rstring_slice1(const rstring* rstr, int index);

rstring* rstring_slice(const rstring* rstr, int index, int length);

rstring* rstring_upcase(const rstring* rstr);



rstring_array* rstring_array_new(const rstring** strings, int size);
int rstring_array_free(rstring_array* rary);

rstring* rstring_array_join(const rstring_array* rstrings, const rstring* sep);

rstring_array* rstring_split(const rstring* rstr, const rstring* sep);

#endif // _RSTRING_H
