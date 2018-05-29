/**
 * @file
 * @author Ryan Moore
 * @brief File containing functions like those found in Ruby's String class.
 * @see https://ruby-doc.org/core-2.5.0/String.html
 */

#include <assert.h>
#include <stdarg.h>
#include <limits.h>

#include "bstrlib.h"

#include "rstring.h"
#include "rlib.h"

/**
 * @brief Make a new rstring from c string.
 *
 * @param cstr Char array to convert to rstring.  (Not modified.)
 *
 * @retval rstring* A valid rstring.
 * @retval NULL The cstr is NULL, the length of cstr >= INT_MAX, or there were errors creating the rstring.
 *
 * @warning The caller must free the result.
*/
rstring*
rstring_new(const char* cstr)
{
  if (cstr == NULL) { return NULL; }

  rstring* rstr = (rstring*)bfromcstr_check_length(cstr);

  if (rstring_bad(rstr)) { return NULL; }

  return rstr;
}

/**
 * @brief Copies the rstring.
 *
 * @param rstr The rstring to copy. (Not modified.)
 *
 * @retval rstring* A valid rstring copy.
 * @retval NULL The input rstring is invalid or there was an error.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_copy(const rstring* rstr)
{
  if (rstring_bad(rstr)) { return NULL; }

  return (rstring*)bstrcpy((const_bstring)rstr);
}

/**
 * @brief Free the rstring.
 *
 * @param rstr An rstring to free.
 *
 * @retval RERROR If there was an error.
 * @retval ROKAY If there were no errors.
 */
int
rstring_free(rstring* rstr)
{
  return bdestroy((bstring)rstr);
}

/**
 * @brief Make a new string with final trailing record separator removed.
 *
 * Will remove `\n`, `\r`, and `\r\n`.  If there are multiple line separators at the end of the string, only the last one will be removed.
 *
 * @param rstr An rstring. (Not modified.)
 *
 * @retval rstring* A valid rstring without the last record separator.
 * @retval NULL The input rstring is invalid or there was an error.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_chomp(const rstring* rstr)
{
  if (rstring_bad(rstr)) { return NULL; }

  int len = rstring_length(rstr);
  if (len == RERROR) { return NULL; }

  int c_last = rstring_char_at(rstr, len - 1);
  if (c_last == RERROR) { return NULL; }

  if (c_last == '\r') {
    return rstring_slice(rstr, 0, len-1);
  }
  else if (c_last == '\n' && rstring_char_at(rstr, len - 2) == '\r') {
    return rstring_slice(rstr, 0, len-2);
  }
  else if (c_last == '\n') {
    return rstring_slice(rstr, 0, len-1);
  }
  else {
    return rstring_copy(rstr);
  }
}

/**
 * @brief Return a copy of rstr with everything lowercase.
 *
 * @param rstr The rstring to downcase. (Not modified.)
 *
 * @retval rstring* A valid rstring copy of rstr with all chars lowercase.
 * @retval NULL The input rstring is invalid or there was an error.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_downcase(const rstring* rstr)
{
  if (rstring_bad(rstr)) { return NULL; }

  int len = rstring_length(rstr);
  if (len == RERROR) { return NULL; }

  if (len == 0) { return rstring_new(""); }

  rstring* new_rstr = rstring_copy(rstr);
  if (new_rstr == NULL) { return NULL; }

  int val = btolower(new_rstr);
  if (val == RERROR) { return NULL; }

  return new_rstr;
}


/**
 * @brief Gives the char at index but as an rstring.
 *
 * @param rstr The rstring that we want to substring. (Not modified.)
 * @param index The starting index of the substring.
 *
 * @retval NULL The rstring is invalid, the index < 0 or >= rstr length, or there was an error.
 *
 * @return A single character rstring or NULL if there were errors.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_slice1(const rstring* rstr, int index)
{
  if (rstring_bad(rstr)) { return NULL; }

  if (index < 0 || index >= rstring_length(rstr)) {
    return NULL;
  }

  return bmidstr(rstr, index, 1);
}

/**
 * @brief Return a substring starting at index of a given length (clamped by the end of rstr).
 *
 * @param rstr The rstring that we want to substring. (Not modified.)
 * @param index The starting index of the substring.
 * @param length The length of the substring.
 *
 * @retval rstring* A valid rstring containing the specified substring.
 * @retval NULL The input rstring is invalid or there was an error.
 *
 * @note There is a bit of weird behavior if the index is the length of the rstring, an empty string will be returned rather than NULL, which is the behavior of rstring_slice1.  It is this way to match the Ruby behavior.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_slice(const rstring* rstr, int index, int length)
{
  if (rstring_bad(rstr)) { return NULL; }

  int len = rstring_length(rstr);
  if (len == RERROR) { return NULL; }

  int idx = index;

  /* Special weird ruby case. */
  if (index == len) {
    return rstring_new("");
  }

  /* Negative indices count back from the end. */
  if (index < 0) {
    idx = len - idx - 1;
  }

  if (idx > len || length < 0) {
    return NULL;
  }

  return (rstring*)bmidstr((bstring)rstr, index, length);
}


/**
 * @brief Return a copy of rstr with everything uppercase.
 *
 * @param rstr The rstring to upcase. (Not modified.)
 *
 * @retval rstring* A valid rstring copy of rstr with all chars uppercase.
 * @retval NULL The input rstring is invalid or there was an error.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_upcase(const rstring* rstr)
{
  if (rstring_bad(rstr)) { return NULL; }

  int len = rstring_length(rstr);
  if (len == RERROR) { return NULL; }

  if (len == 0) { return rstring_new(""); }

  rstring* new_rstr = rstring_copy(rstr);
  if (new_rstr == NULL) { return NULL; }

  int val = btoupper(new_rstr);
  if (val == RERROR) { return NULL; }

  return new_rstr;
}

/*
 * String info functions
 */

/**
 * @brief Two strings are equal if they have the same length and content.
 *
 * @param rstr1 Not modified.
 * @param rstr2 Not modified.
 *
 * @retval RTRUE The rstrings are equal.
 * @retval RFALSE The rstrings are not equal.
 * @retval RERROR The rstr1 or rstr2 is invalid or there was an error.
 *
 * @note This differs from ruby in that you can't just check for true or false, you must check for 1 or 0 as RERROR signals an error.
 */
int
rstring_eql(const rstring* rstr1, const rstring* rstr2)
{
  if (rstring_bad(rstr1)) { return RERROR; }
  if (rstring_bad(rstr2)) { return RERROR; }

  int val = biseq((const_bstring)rstr1, (const_bstring)rstr2);

  if (val == 1) {
    return RTRUE;
  }
  else if (val == 0) {
    return RFALSE;
  }
  else {
    return RERROR;
  }
}

/**
 * @brief Returns the length of the rstring.
 *
 * @param rstr Not modified.
 *
 * @retval length The length of the rstring.
 * @retval RERROR The input rstring is invalid or there was an error.
 */
int
rstring_length(const rstring* rstr)
{
  if (rstring_bad(rstr)) { return RERROR; }

  return blengthe(rstr, RERROR);
}



/*
 * Utility functions
 */

/**
 * @brief Return the char at given position
 *
 * @param rstr Not modified.
 * @param index Get the char at this index in rstr.
 *
 * @retval char The idx'th character of the rstring.
 * @retval RERROR If The input rstring is invalid or there was an error.
 */
int
rstring_char_at(const rstring* rstr, int idx)
{
  if (rstring_bad(rstr)) { return RERROR; }

  return bchare(rstr, idx, RERROR);
}


rstring_array*
rstring_array_new()
{
  rstring_array* rary = NULL;

  rary = bstrListCreate();
  if (rary == NULL) { return NULL; }

  return rary;
}

int
rstring_array_push_rstr(rstring_array* rary, rstring* rstr)
{
  int current_size = rary->qty;
  int rval = 0;
  rval = bstrListAlloc((struct bstrList*)rary, current_size + 1);
  if (rval == BSTR_ERR) { return RERROR; }
  rary->qty = current_size + 1;
  rary->entry[rary->qty - 1] = rstr;
  assert(rary->qty <= rary->mlen);
  assert(rary->qty == current_size + 1);

  return ROKAY;
}

int
rstring_array_push_cstr(rstring_array* rary, char* cstr)
{
  int current_size = rary->qty;
  int rval = 0;
  rstring* rstr = NULL;
  rval = bstrListAlloc((struct bstrList*)rary, current_size + 1);
  if (rval == BSTR_ERR) { return RERROR; }

  rstr = rstring_new(cstr);
  if (rstr == NULL) { return RERROR; }

  rary->qty = current_size + 1;
  rary->entry[rary->qty - 1] = rstr;
  assert(rary->qty <= rary->mlen);
  assert(rary->qty == current_size + 1);

  return ROKAY;
}

rstring*
rstring_array_get(rstring_array* rary, int index)
{
  if (index < 0 || index >= rary->qty) { return NULL; }
  if (rary->qty > rary->mlen) { return NULL; }

  rstring* rstr = NULL;
  rstr = rary->entry[index];
  if (rstr == NULL) { return NULL; }

  return rstr;
}

int
rstring_array_free(rstring_array* rary)
{
  return bstrListDestroy((struct bstrList*)rary);
}

rstring*
rstring_array_join(rstring_array* rstrings, const rstring* sep)
{
  if (rstrings == NULL || sep == NULL) { return NULL; }
  if (rstrings->qty == 0) {
    return rstring_new("");
  }

  return bjoin((const struct bstrList*)rstrings, (const_bstring)sep);
}

rstring_array*
rstring_split(rstring* rstr, const rstring* sep)
{
  return bsplitstr((bstring)rstr, (const_bstring)sep);
}
