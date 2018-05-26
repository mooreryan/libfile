#include "unity.h"
#include "rstring.h"

/* Also need to include bstrlib.h in the test file so Ceedling knows
   about it. */
#include "bstrlib.h"

#define TEST_ASSERT_EQUAL_RSTRING(expected, actual) TEST_ASSERT_TRUE(biseqcstr((const_bstring)actual, expected))

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
