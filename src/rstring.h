/**
 * @file
 * @author Ryan Moore
 * @brief Header file for rstring.c
 * @see https://ruby-doc.org/core-2.5.0/String.html
 */

#ifndef _RSTRING_H
#define _RSTRING_H

#include "rlib.h"

typedef struct tagbstring rstring;

typedef struct bstrList rstring_array;

/* Macros */

/**
 * @brief These conditions must hold for the rstring to be valid.  See the btolower function for this.
 */
#define rstring_bad(rstr) (rstr == NULL || rstr->data == NULL || rstr->mlen < rstr->slen || rstr->slen < 0 || rstr->mlen <= 0)

/**
 * @brief Check that an rstring_array is valid.
 */
#define rstring_array_bad(rary) (rary == NULL || rary->entry == NULL || rary->mlen < rary->qty || rary->qty < 0 || rary->mlen <= 0)

/**
 * @brief Get the `->data` portion of the rstring (i.e., the char* part).
 *
 * @param rstr We will get the data of this rstring.
 *
 * @retval char* The char* part of the rstring.
 * @retval NULL The rstring is NULL.
 */
#define rstring_data(rstr) ((rstring*)bdata((bstring)rstr))

/**
 * @brief This is like Ruby's `"apple %s" % "pie"` `%` operator.
 *
 * @code
... initialization code ...

rstring* rstr = rstring_new("pies");
rstring* new_rstr = rstring_format("%d apple %s", 3, rstring_data(rstr));

... cleanup code ...
 * @endcode
 *
 * @param fmt This is a char* format string.
 * @param ... A variable number of args to put into the fmt string.
 *
 * @retval rstring* A new rstring holding the formatted string.
 *
 * @note This one doesn't take rstrings like all the rest, rather char* or ints, or floats or whatever.
 * @warning If the BSTRLIB_NOVSNP macro has been set when bstrlib has been compiled the bformat function is not present.  This will NOT be checked.
 *
 * @todo Ideally, you could put rstrings straight in to plases where you have the %s character and it would JustWork.
 */
#define rstring_format(fmt, ...) ((rstring*)bformat(fmt, __VA_ARGS__))

/* Constructing */

rstring* rstring_new(const char* cstr);
rstring* rstring_copy(const rstring* rstr);
int rstring_free(rstring* rstr);


/* Returning modified rstrings */

rstring* rstring_chomp(const rstring* rstr);
rstring* rstring_downcase(const rstring* rstr);
rstring* rstring_gsub(const rstring* rstr, const rstring* pattern, const rstring* replacement);
rstring* rstring_lstrip(const rstring* rstr);
rstring* rstring_reverse(const rstring* rstr);
rstring* rstring_rstrip(const rstring* rstr);
rstring* rstring_slice1(const rstring* rstr, int index);
rstring* rstring_slice(const rstring* rstr, int index, int length);
rstring* rstring_strip(const rstring* rstr);
rstring* rstring_upcase(const rstring* rstr);

/* Get info about rstrings */

int rstring_eql(const rstring* rstr1, const rstring* rstr2);
int rstring_eql_cstr(const rstring* rstr, const char* cstr);
int rstring_include(const rstring* rstr, const rstring* substring);
int rstring_index(const rstring* rstr, const rstring* substring);
int rstring_index_offset(const rstring* rstr, const rstring* substring, int start_pos);
int rstring_length(const rstring* rstr);

/* Utility functions */

int rstring_char_at(const rstring* rstr, int idx);

/* rstring array functions */

rstring_array* rstring_array_new();
int rstring_array_free(rstring_array* rary);
int rstring_array_push_cstr(rstring_array* rary, char* cstr);
int rstring_array_push_rstr(rstring_array* rary, rstring* rstr);
rstring* rstring_array_get(rstring_array* rary, int index);
rstring* rstring_array_join(rstring_array* rstrings, const rstring* sep);
rstring_array* rstring_split(rstring* rstr, const rstring* sep);

#endif // _RSTRING_H
