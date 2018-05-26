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
 * @brief Returns the length of rstr.
 *
 * @returns -1 if rstr == NULL, length otherwise.
 */
int
rstring_length(rstring* rstr)
{
  return blengthe(rstr, -1);
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
rstring_chomp(rstring* rstr)
{
  int len = rstring_length(rstr);
  char c_last = bchar(rstr, len - 1);

  if (c_last == '\r') {
    return bmidstr((bstring)rstr, 0, len-1);
  }
  else if (c_last == '\n' && bchar(rstr, len - 2) == '\r') {
    return bmidstr((bstring)rstr, 0, len-2);
  }
  else if (c_last == '\n') {
    return bmidstr((bstring)rstr, 0, len-1);
  }
  else {
    return bstrcpy((bstring)rstr);
  }
}
