#include <assert.h>
#include <stdarg.h>
#include <limits.h>

#include "bstrlib.h"

#include "rstring.h"


/**
 * @brief Make a new rstring from c string.
 *
 * @return Pointer to an rstring or NULL if it failed.
 *
 * @note Also will return NULL if you try and save a string whose length is greater than INT_MAX.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_new(const char* cstr)
{

  return (rstring*)bfromcstr_check_length(cstr);
}

/**
 * @brief Free the rstring.
 *
 * @returns BSTR_ERR on failure, BSTR_OK on success.
 */
int
rstring_free(rstring* rstr)
{
  return bdestroy((bstring)rstr);
}

/**
 * @brief Return a new string with final trailing record separator removed.
 *
 * Will remove \n, \r, and \r\n.  If there are multiple line separators at the end of the string, only the last one will be removed.
 *
 * @returns A new rstring without the last record separator, or NULL if there was an error.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_chomp(const rstring* rstr)
{


  int len = rstring_length(rstr);
  char c_last = bchar(rstr, len - 1);

  if (c_last == '\r') {
    return rstring_slice(rstr, 0, len-1);
  }
  else if (c_last == '\n' && bchar(rstr, len - 2) == '\r') {
    return rstring_slice(rstr, 0, len-2);
  }
  else if (c_last == '\n') {
    return rstring_slice(rstr, 0, len-1);
  }
  else {
    return bstrcpy((const_bstring)rstr);
  }
}

/**
 * @brief Return a copy of rstr with everything lowercase.
 *
 * @returns a copy of the string in lowercase or NULL if errors occured.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_downcase(const rstring* rstr)
{
  if (rstr == NULL) { return NULL; }

  if (rstring_length(rstr) == 0) { return rstring_new(""); }

  rstring* new_rstr = bstrcpy((const_bstring)rstr);
  if (new_rstr == NULL) { return NULL; }

  int ret_val = 0;

  ret_val = btolower(new_rstr);

  if (ret_val == RERROR) { return NULL; }

  return new_rstr;

}


/**
 * @brief Two strings are equal if they have the same length and content.
 *
 * @returns 1 if equal, 0 if not, -1 if there was an error.
 *
 * @note This differs from ruby in that you can't just check for true or false, you must check for 1 or 0 as -1 signals an error.
 */
int
rstring_eql(const rstring* rstr1, const rstring* rstr2)
{
  return biseq((const_bstring)rstr1, (const_bstring)rstr2);
}

/**
 * @brief Returns the length of rstr.
 *
 * @returns RERROR if rstr == NULL, length otherwise.
 */
int
rstring_length(const rstring* rstr)
{
  return blengthe(rstr, RERROR);
}

/**
 * @brief Gives the char at index but as an rstring.
 *
 * @return A single character rstring or NULL if there were errors.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_slice1(const rstring* rstr, int index)
{
  if (index < 0 || index >= rstring_length(rstr)) {
    return NULL;
  }

  return bmidstr(rstr, index, 1);
}

/**
 * @brief Return a substring starting at index of a given length.
 *
 * @return The specified substring or NULL if there were errors.
 *
 * @note There is a bit of weird behavior if the index is the length of the rstring, an empty string will be returned rather than NULL, which is the behavior of rstring_slice1.  It is this way to match the Ruby behavior.
 * @warning The caller must free the result.
 */
rstring*
rstring_slice(const rstring* rstr, int index, int length)
{
  int len = rstring_length(rstr);
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


  return bmidstr((bstring)rstr, index, length);
}

/**
 * @brief Split the rstring by the delimiter.
 *
 * If split_by is a ' ', string is split on whitespace, with leading whitespace and runs of contiguous whitespace characters ignored.
 *
 * @returns an rstring_array* with the splits.

 * @note Ruby also has a version of this with a limit field.
 */
rstring_array*
rstring_split_by_char(const rstring* rstr, unsigned char split_by)
{

}

/**
 * @brief Return a copy of rstr with everything uppercase.
 *
 * @returns a copy of the string in uppercase or NULL if errors occured.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_upcase(const rstring* rstr)
{
  if (rstr == NULL) { return NULL; }

  if (rstring_length(rstr) == 0) { return rstring_new(""); }

  rstring* new_rstr = bstrcpy((const_bstring)rstr);
  if (new_rstr == NULL) { return NULL; }

  int ret_val = 0;

  ret_val = btoupper(new_rstr);

  if (ret_val == RERROR) { return NULL; }

  return new_rstr;

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

rstring*
rstring_copy(const rstring* rstr)
{
  return (rstring*)bstrcpy((const_bstring)rstr);
}
