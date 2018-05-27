#include <assert.h>
#include <stdarg.h>

#include "bstrlib.h"

#include "rstring.h"

/**
 * @brief Make a new rstring from c string.
 *
 * @return Pointer to an rstring or NULL if it failed.
 *
 * @warning The caller must free the result.
 */
rstring*
rstring_new(const char* cstr)
{
  return bfromcstr(cstr);
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

  if (ret_val == RSTRING_ERR) { return NULL; }

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
 * @returns -1 if rstr == NULL, length otherwise.
 */
int
rstring_length(const rstring* rstr)
{
  return blengthe(rstr, -1);
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

  if (ret_val == RSTRING_ERR) { return NULL; }

  return new_rstr;

}

rstring_array*
rstring_array_new(const rstring** strings, int size)
{
  if (strings == NULL) { return NULL; }

  int i = 0;
  rstring_array* rary = NULL;

  rary = bstrListCreate();
  if (rary == NULL) { return NULL; }

  if (size == 0) {
    return rary;
  }
  else {
    int ret_val = bstrListAlloc(rary, size);
    if (ret_val == BSTR_ERR) { return NULL; }

    assert(rary->qty == 0 && rary->mlen >= size);

    for (i = 0; i < size; ++i) {
      rary->entry[i] = strings[i];
    }
    rary->qty = size;

    return rary;
  }
}

int
rstring_array_free(rstring_array* rary)
{
  return bstrListDestroy((struct bstrList*)rary);
}

rstring*
rstring_array_join(const rstring_array* rstrings, const rstring* sep)
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
