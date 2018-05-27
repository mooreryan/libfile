#include "unity.h"
#include "rstring.h"

/* Also need to include bstrlib.h in the test file so Ceedling knows
   about it. */
#include "bstrlib.h"

#define TEST_ASSERT_EQUAL_RSTRING(expected, actual) TEST_ASSERT_TRUE(biseqcstr((const_bstring)actual, expected))
#define TEST_ASSERT_EQUAL_RSTRING_ARRAY(expected, actual) \
  do { \
    TEST_ASSERT_EQUAL(1, rstring_array_eql(expected, actual)); \
  } while (0)


void setUp(void)
{
}

void tearDown(void)
{
}

void test___rstring_new___should_ReturnANewRstring(void)
{
  rstring* actual = NULL;

  TEST_ASSERT_NULL(rstring_new(NULL));

  TEST_ASSERT_EQUAL_RSTRING("", (actual = rstring_new("")));
  rstring_free(actual);

  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_new("apple")));
  rstring_free(actual);
}

void
test___rstring_free___should_FreeRstring(void)
{
  rstring* rstr = rstring_new("apple");
  int ret = rstring_free(rstr);
  TEST_ASSERT_EQUAL(BSTR_OK, ret);
}

void test___rstring_length___should_ReturnTheLength(void)
{
  int len = 0;
  rstring* rstr = NULL;

  TEST_ASSERT_EQUAL(-1, rstring_length(NULL));

  rstr = rstring_new("");
  len = rstring_length(rstr);
  TEST_ASSERT_EQUAL(0, len);
  rstring_free(rstr);

  rstr = rstring_new("1234");
  len = rstring_length(rstr);
  TEST_ASSERT_EQUAL(4, len);
  rstring_free(rstr);

  rstr = rstring_new("1234\r\n");
  len = rstring_length(rstr);
  TEST_ASSERT_EQUAL(6, len);
  rstring_free(rstr);
}

void
test___rstring_chomp___should_RemoveRecordSeparators(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  rstr = rstring_new("apple");
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  /* \n style */
  rstr = rstring_new("apple\n");
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple\n\n");
  TEST_ASSERT_EQUAL_RSTRING("apple\n", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  /* \r style */
  rstr = rstring_new("apple\r");
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple\r\r");
  TEST_ASSERT_EQUAL_RSTRING("apple\r", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  /* \r\n style */
  rstr = rstring_new("apple\r\n");
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple\r\n\r\n");
  TEST_ASSERT_EQUAL_RSTRING("apple\r\n", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  /* Other stuff */
  rstr = rstring_new("apple\n\r");
  TEST_ASSERT_EQUAL_RSTRING("apple\n", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple \n pie");
  TEST_ASSERT_EQUAL_RSTRING("apple \n pie", (actual = rstring_chomp(rstr)));
  rstring_free(rstr);
  rstring_free(actual);
}

void
test___rstring_slice1___should_ReturnSubstringOfOneCharacter(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  rstr = rstring_new("apple");

  TEST_ASSERT_EQUAL_RSTRING("a", (actual = rstring_slice1(rstr, 0)));
  rstring_free(actual);

  /* Returns NULL if you try and access beyond the length. */
  TEST_ASSERT_NULL(rstring_slice1(rstr, 5));

  /* Also returns NULL if you try and get a negative index. */
  TEST_ASSERT_NULL(rstring_slice1(rstr, -1));

  rstring_free(rstr);
}

void
test___rstring_slice___should_ReturnSubstringOfGivenLength(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;
  int len = 5;

  rstr = rstring_new("apple");

  /* Zero for the length gives an empty string. */
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rstring_slice(rstr, 0, 0)));
  rstring_free(actual);

  TEST_ASSERT_EQUAL_RSTRING("a", (actual = rstring_slice(rstr, 0, 1)));
  rstring_free(actual);

  TEST_ASSERT_EQUAL_RSTRING("ap", (actual = rstring_slice(rstr, 0, 2)));
  rstring_free(actual);

  /* A length > the actual length of the string won't cause any
     problems. */
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_slice(rstr, 0, 10)));
  rstring_free(actual);

  /* This one is just weird Ruby behavior.  Note how this is different
     from rstring_slice1(rstr, rstring_length(rstr)). */
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rstring_slice(rstr, len, 0)));
  rstring_free(actual);
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rstring_slice(rstr, len, 10)));
  rstring_free(actual);

  /* Returns NULL if you try and access just beyond the string.. */
  TEST_ASSERT_NULL(rstring_slice(rstr, len + 1, 0));
  TEST_ASSERT_NULL(rstring_slice(rstr, len + 1, 10));

  /* Also returns NULL if you pass a negative length. */
  TEST_ASSERT_NULL(rstring_slice(rstr, 0, -1));

  /* Negative indices count backwards from the end. */
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rstring_slice(rstr, -2, 0)));
  rstring_free(actual);
  TEST_ASSERT_EQUAL_RSTRING("le", (actual = rstring_slice(rstr, -2, 2)));
  rstring_free(actual);
  TEST_ASSERT_EQUAL_RSTRING("app", (actual = rstring_slice(rstr, -len, 3)));
  rstring_free(actual);
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_slice(rstr, -len, len)));
  rstring_free(actual);
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_slice(rstr, -len, len + 10)));
  rstring_free(actual);


  /* Negative 0 is still just 0. */
  TEST_ASSERT_EQUAL_RSTRING("a", (actual = rstring_slice(rstr, -0, 1)));
  rstring_free(actual);


  /* Negative indices > the length of the string return NULL */
  TEST_ASSERT_NULL(rstring_slice(rstr, -len - 1, 1));

  rstring_free(rstr);

}

void
test___rstring_eql___should_TellIfStringsAreEqual(void)
{
  rstring* rstr1 = NULL;
  rstring* rstr2 = NULL;

  rstr1 = rstring_new("apple");
  rstr2 = rstring_new("apple");

  TEST_ASSERT_EQUAL(1, rstring_eql(rstr1, rstr2));

  rstring_free(rstr2);
  rstr2 = rstring_new("pie");

  TEST_ASSERT_EQUAL(0, rstring_eql(rstr1, rstr2));
  rstring_free(rstr2);

  /* Weird things you shouldn't do, like manually change the length. */
  rstr2 = rstring_new("apple");
  rstr2->data[0] = 'Z';
  TEST_ASSERT_EQUAL(0, rstring_eql(rstr1, rstr2));

  rstr2->data[0] = 'a';
  rstr2->slen = 10;
  TEST_ASSERT_EQUAL(0, rstring_eql(rstr1, rstr2));
  rstr2->slen = 5;

  rstring_free(rstr1);
  rstring_free(rstr2);
}

void
test___rstring_downcase___should_ReturnALowcaseString(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  rstr = rstring_new("APPLE");
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_downcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("Apple");
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_downcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple");
  TEST_ASSERT_EQUAL_RSTRING("apple", (actual = rstring_downcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple PIE 123");
  TEST_ASSERT_EQUAL_RSTRING("apple pie 123", (actual = rstring_downcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("");
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rstring_downcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);
}

void
test___rstring_upcase___should_ReturnAnUppercaseString(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  rstr = rstring_new("APPLE");
  TEST_ASSERT_EQUAL_RSTRING("APPLE", (actual = rstring_upcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("Apple");
  TEST_ASSERT_EQUAL_RSTRING("APPLE", (actual = rstring_upcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple");
  TEST_ASSERT_EQUAL_RSTRING("APPLE", (actual = rstring_upcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple PIE 123");
  TEST_ASSERT_EQUAL_RSTRING("APPLE PIE 123", (actual = rstring_upcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("");
  TEST_ASSERT_EQUAL_RSTRING("", (actual = rstring_upcase(rstr)));
  rstring_free(rstr);
  rstring_free(actual);
}

void
test___rstring_array_join___should_JoinStrings(void)
{
  int size = 0;
  rstring* actual = NULL;
  rstring* sep = NULL;

  rstring* strings[3] = { rstring_new("apple"), rstring_new("pie"), rstring_new("good") };
  size = 3;
  rstring_array* rary = rstring_array_new(strings, size);

  sep = rstring_new("");
  actual = rstring_array_join(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("applepiegood", actual);
  rstring_free(actual);
  rstring_free(sep);

  sep = rstring_new("/");
  actual = rstring_array_join(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("apple/pie/good", actual);
  rstring_free(actual);
  rstring_free(sep);

  sep = rstring_new("//");
  actual = rstring_array_join(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("apple//pie//good", actual);
  rstring_free(actual);
  rstring_free(sep);

  rstring_array_free(rary);


  /* Empty array */
  rstring* strings2[0] = {};
  rary = rstring_array_new(strings2, 0);

  sep = rstring_new(".");
  actual = rstring_array_join(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(actual);
  rstring_free(sep);

  rstring_array_free(rary);

  /* Single element */
  rstring* strings3[1] = { rstring_new("apple") };
  rary = rstring_array_new(strings3, 1);

  sep = rstring_new(".");
  actual = rstring_array_join(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(actual);
  rstring_free(sep);

  rstring_array_free(rary);

}
