#include <stdlib.h>

#include "unity.h"
#include "helper.h"
#include "rlib.h"



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
  TEST_ASSERT_EQUAL(ROKAY, ret);
}

void
test___rstring_format___should_ProduceNewStringLikesprintf(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  TEST_ASSERT_NULL(rstring_format(NULL, "apple"));

  rstr = rstring_new("pies");
  actual = rstring_format("%d apple %s", 3, rstring_data(rstr));
  TEST_ASSERT_EQUAL_RSTRING("3 apple pies", actual);
  rstring_free(rstr);
  rstring_free(actual);
}

void test___rstring_length___should_ReturnTheLength(void)
{
  int len = 0;
  rstring* rstr = NULL;

  TEST_ASSERT_RERROR(rstring_length(NULL));

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
  rstr2->slen = 2;
  TEST_ASSERT_EQUAL(0, rstring_eql(rstr1, rstr2));
  rstr2->slen = 5;

  rstring_free(rstr1);
  rstring_free(rstr2);
}

void
test___rstring_eql_cstr___should_TellIfStringsAreEqual(void)
{
  rstring* rstr1 = NULL;
  char* cstr2 = NULL;

  rstr1 = rstring_new("apple");
  cstr2 = "apple";

  TEST_ASSERT_EQUAL(1, rstring_eql_cstr(rstr1, cstr2));

  cstr2 = "pie";

  TEST_ASSERT_EQUAL(0, rstring_eql_cstr(rstr1, cstr2));

  rstring_free(rstr1);
}

void
test___rstring_include___should_TellIfSubstringIsPresent(void)
{
  rstring* rstr = NULL;
  rstring* substring = NULL;

  rstr = rstring_new("apple pie");
  substring = rstring_new("apple pie");
  TEST_ASSERT_EQUAL(RTRUE, rstring_include(rstr, substring));
  rstring_free(substring);

  substring = rstring_new("le p");
  TEST_ASSERT_EQUAL(RTRUE, rstring_include(rstr, substring));
  rstring_free(substring);

  substring = rstring_new("");
  TEST_ASSERT_EQUAL(RTRUE, rstring_include(rstr, substring));
  rstring_free(substring);

  substring = rstring_new("pie ");
  TEST_ASSERT_EQUAL(RFALSE, rstring_include(rstr, substring));
  rstring_free(substring);

  substring = rstring_new("el");
  TEST_ASSERT_EQUAL(RFALSE, rstring_include(rstr, substring));
  rstring_free(substring);

  rstring_free(rstr);
}

void
test___rstring_include_cstr___should_TellIfSubstringIsPresent(void)
{
  rstring* rstr = NULL;
  char* substring = NULL;

  rstr = rstring_new("apple pie");
  substring = "apple pie";
  TEST_ASSERT_EQUAL(RTRUE, rstring_include_cstr(rstr, substring));

  substring = "le p";
  TEST_ASSERT_EQUAL(RTRUE, rstring_include_cstr(rstr, substring));

  substring = "";
  TEST_ASSERT_EQUAL(RTRUE, rstring_include_cstr(rstr, substring));

  substring = "pie ";
  TEST_ASSERT_EQUAL(RFALSE, rstring_include_cstr(rstr, substring));

  substring = "el";
  TEST_ASSERT_EQUAL(RFALSE, rstring_include_cstr(rstr, substring));

  rstring_free(rstr);
}

void
test___rstring_index___should_TellGiveTheIndexOfSubstring(void)
{
  rstring* rstr = NULL;
  rstring* substring = NULL;

  rstr = rstring_new("apple pie");
  substring = rstring_new("apple pie");
  TEST_ASSERT_EQUAL(0, rstring_index(rstr, substring));
  rstring_free(substring);

  substring = rstring_new("le p");
  TEST_ASSERT_EQUAL(3, rstring_index(rstr, substring));
  rstring_free(substring);

  substring = rstring_new("");
  TEST_ASSERT_EQUAL(0, rstring_index(rstr, substring));
  rstring_free(substring);

  substring = rstring_new("pie ");
  TEST_ASSERT_EQUAL(RERROR, rstring_index(rstr, substring));
  rstring_free(substring);

  substring = rstring_new("el");
  TEST_ASSERT_EQUAL(RERROR, rstring_index(rstr, substring));
  rstring_free(substring);

  rstring_free(rstr);
}

void
test___rstring_index_cstr___should_TellGiveTheIndexOfSubstring(void)
{
  rstring* rstr = NULL;
  char* substring = NULL;

  rstr = rstring_new("apple pie");
  substring = "apple pie";
  TEST_ASSERT_EQUAL(0, rstring_index_cstr(rstr, substring));

  substring = "le p";
  TEST_ASSERT_EQUAL(3, rstring_index_cstr(rstr, substring));

  substring = "";
  TEST_ASSERT_EQUAL(0, rstring_index_cstr(rstr, substring));

  substring = "pie ";
  TEST_ASSERT_EQUAL(RERROR, rstring_index_cstr(rstr, substring));

  substring = "el";
  TEST_ASSERT_EQUAL(RERROR, rstring_index_cstr(rstr, substring));

  rstring_free(rstr);
}

void
test___rstring_index_offset___should_TellGiveTheIndexOfSubstring(void)
{
  rstring* rstr = NULL;
  rstring* substring = NULL;

  rstr = rstring_new("apple pie");
  substring = rstring_new("apple pie");
  TEST_ASSERT_EQUAL(0, rstring_index_offset(rstr, substring, 0));
  rstring_free(substring);

  substring = rstring_new("apple pie");
  TEST_ASSERT_EQUAL(RERROR, rstring_index_offset(rstr, substring, 1));
  rstring_free(substring);

  substring = rstring_new("le p");
  TEST_ASSERT_EQUAL(3, rstring_index_offset(rstr, substring, 1));
  rstring_free(substring);

  substring = rstring_new("");
  TEST_ASSERT_EQUAL(0, rstring_index_offset(rstr, substring, 0));
  rstring_free(substring);

  /* Possibly weird Ruby behavior. */
  substring = rstring_new("");
  TEST_ASSERT_EQUAL(1, rstring_index_offset(rstr, substring, 1));
  rstring_free(substring);

  substring = rstring_new("pie ");
  TEST_ASSERT_EQUAL(RERROR, rstring_index_offset(rstr, substring, 0));
  rstring_free(substring);

  substring = rstring_new("el");
  TEST_ASSERT_EQUAL(RERROR, rstring_index_offset(rstr, substring, 0));
  rstring_free(substring);

  rstring_free(rstr);
}

void
test___rstring_index_offset_cstr___should_TellGiveTheIndexOfSubstring(void)
{
  rstring* rstr = NULL;
  char* substring = NULL;

  rstr = rstring_new("apple pie");
  substring = "apple pie";
  TEST_ASSERT_EQUAL(0, rstring_index_offset_cstr(rstr, substring, 0));

  substring = "apple pie";
  TEST_ASSERT_EQUAL(RERROR, rstring_index_offset_cstr(rstr, substring, 1));

  substring = "le p";
  TEST_ASSERT_EQUAL(3, rstring_index_offset_cstr(rstr, substring, 1));

  substring = "";
  TEST_ASSERT_EQUAL(0, rstring_index_offset_cstr(rstr, substring, 0));

  /* Possibly weird Ruby behavior. */
  substring = "";
  TEST_ASSERT_EQUAL(1, rstring_index_offset_cstr(rstr, substring, 1));

  substring = "pie ";
  TEST_ASSERT_EQUAL(RERROR, rstring_index_offset_cstr(rstr, substring, 0));

  substring = "el";
  TEST_ASSERT_EQUAL(RERROR, rstring_index_offset_cstr(rstr, substring, 0));

  rstring_free(rstr);
}

void
test___rstring_reverse___should_ReturnReversedCopy(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  TEST_ASSERT_NULL(rstring_reverse(NULL));

  rstr = rstring_new("");
  actual = rstring_reverse(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("1");
  actual = rstring_reverse(rstr);
  TEST_ASSERT_EQUAL_RSTRING("1", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("123");
  actual = rstring_reverse(rstr);
  TEST_ASSERT_EQUAL_RSTRING("321", actual);
  rstring_free(rstr);
  rstring_free(actual);
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
test___rstring_strip___should_StripWhitespace(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  TEST_ASSERT_NULL(rstring_strip(NULL));

  rstr = rstring_new("");
  actual = rstring_strip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new(" \r \n \t\t\r\n   ");
  actual = rstring_strip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple");
  actual = rstring_strip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("   apple   ");
  actual = rstring_strip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new(" \n\r\t   apple   \t\r\n\n    ");
  actual = rstring_strip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(rstr);
  rstring_free(actual);
}

void
test___rstring_lstrip___should_StripWhitespace(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  TEST_ASSERT_NULL(rstring_lstrip(NULL));

  rstr = rstring_new("");
  actual = rstring_lstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new(" \r \n \t\t\r\n   ");
  actual = rstring_lstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple");
  actual = rstring_lstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("   apple   ");
  actual = rstring_lstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple   ", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new(" \n\r\t   apple   \t\r\n\n    ");
  actual = rstring_lstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple   \t\r\n\n    ", actual);
  rstring_free(rstr);
  rstring_free(actual);
}

void
test___rstring_rstrip___should_StripWhitespace(void)
{
  rstring* rstr = NULL;
  rstring* actual = NULL;

  TEST_ASSERT_NULL(rstring_rstrip(NULL));

  rstr = rstring_new("");
  actual = rstring_rstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new(" \r \n \t\t\r\n   ");
  actual = rstring_rstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("apple");
  actual = rstring_rstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new("   apple   ");
  actual = rstring_rstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING("   apple", actual);
  rstring_free(rstr);
  rstring_free(actual);

  rstr = rstring_new(" \n\r\t   apple   \t\r\n\n    ");
  actual = rstring_rstrip(rstr);
  TEST_ASSERT_EQUAL_RSTRING(" \n\r\t   apple", actual);
  rstring_free(rstr);
  rstring_free(actual);
}

void
gsub_test (char* cstr,
           char* cpattern,
           char* creplacement,
           char* cexpected)
{
  rstring* rstr = rstring_new(cstr);
  rstring* pattern = rstring_new(cpattern);
  rstring* replacement = rstring_new(creplacement);
  rstring* actual = rstring_gsub(rstr, pattern, replacement);

  char* msg = malloc(sizeof(char) * 10000);
  snprintf(msg,
           9999,
           "cstr: '%s', pattern: '%s', replacement: '%s', Expected: '%s', actual: '%s'\n",
           cstr,
           cpattern,
           creplacement,
           cexpected,
           actual->data);

  TEST_ASSERT_EQUAL_RSTRING_MESSAGE(cexpected, actual, msg);

  free(msg);
  rstring_free(rstr);
  rstring_free(pattern);
  rstring_free(replacement);
  rstring_free(actual);
}

void
gsub_cstr_test (char* cstr,
           char* cpattern,
           char* creplacement,
           char* cexpected)
{
  rstring* rstr = rstring_new(cstr);
  char* pattern = cpattern;
  char* replacement = creplacement;
  rstring* actual = rstring_gsub_cstr(rstr, pattern, replacement);

  char* msg = malloc(sizeof(char) * 10000);
  snprintf(msg,
           9999,
           "cstr: '%s', pattern: '%s', replacement: '%s', Expected: '%s', actual: '%s'\n",
           cstr,
           cpattern,
           creplacement,
           cexpected,
           actual->data);

  TEST_ASSERT_EQUAL_RSTRING_MESSAGE(cexpected, actual, msg);

  free(msg);
  rstring_free(rstr);
  rstring_free(actual);
}

void
gsub_test_be_null (char* cstr,
                   char* cpattern,
                   char* creplacement)
{
  rstring* rstr = rstring_new(cstr);
  rstring* pattern = rstring_new(cpattern);
  rstring* replacement = rstring_new(creplacement);
  rstring* actual = rstring_gsub(rstr, pattern, replacement);

  TEST_ASSERT_NULL(actual);

  rstring_free(rstr);
  rstring_free(pattern);
  rstring_free(replacement);
  rstring_free(actual);
}

void
gsub_cstr_test_be_null (char* cstr,
                   char* cpattern,
                   char* creplacement)
{
  rstring* rstr = rstring_new(cstr);
  char* pattern = cpattern;
  char* replacement = creplacement;
  rstring* actual = rstring_gsub_cstr(rstr, pattern, replacement);

  TEST_ASSERT_NULL(actual);

  rstring_free(rstr);
  rstring_free(actual);
}
void
test___rstring_gsub___should_ReturnCopyWithSubbedStuff()
{
  /* Pattern must have length > 0. */
  gsub_test_be_null("", "", "");
  gsub_test("", "apple", "", "");
  gsub_test("apple", "apple", "", "");

  gsub_test("  ", " ", "a", "aa");
  gsub_test("  ", "  ", "a", "a");

  gsub_test("apple", "apple pie", "ryan", "apple");

  gsub_test("apple", "p", "AP", "aAPAPle");
  gsub_test("apple", "pie", "PIE", "apple");
  gsub_test("apple", "p", "", "ale");
  gsub_test("aabaAb", "a", "aa", "aaaabaaAb");
}

void
test___rstring_gsub_cstr___should_ReturnCopyWithSubbedStuff()
{
  /* Pattern must have length > 0. */
  gsub_cstr_test_be_null("", "", "");
  gsub_cstr_test("", "apple", "", "");
  gsub_cstr_test("apple", "apple", "", "");

  gsub_cstr_test("  ", " ", "a", "aa");
  gsub_cstr_test("  ", "  ", "a", "a");

  gsub_cstr_test("apple", "apple pie", "ryan", "apple");

  gsub_cstr_test("apple", "p", "AP", "aAPAPle");
  gsub_cstr_test("apple", "pie", "PIE", "apple");
  gsub_cstr_test("apple", "p", "", "ale");
  gsub_cstr_test("aabaAb", "a", "aa", "aaaabaaAb");
}

void
test___rstring_array_join___should_JoinStrings(void)
{
  int size = 0;
  rstring* actual = NULL;
  rstring* sep = NULL;

  rstring* strings[3] = { rstring_new("apple"), rstring_new("pie"), rstring_new("good") };
  size = 3;
  rstring_array* rary = rstring_array_new();
  rstring_array_push_cstr(rary, "apple");
  rstring_array_push_cstr(rary, "pie");
  rstring_array_push_cstr(rary, "good");

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
  rstring_free(strings[0]);
  rstring_free(strings[1]);
  rstring_free(strings[2]);


  /* Empty array */
  rary = rstring_array_new();

  sep = rstring_new(".");
  actual = rstring_array_join(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(actual);
  rstring_free(sep);

  rstring_array_free(rary);

  /* Single element */
  rary = rstring_array_new();
  rstring_array_push_cstr(rary, "apple");

  sep = rstring_new(".");
  actual = rstring_array_join(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(actual);
  rstring_free(sep);

  rstring_array_free(rary);

}

void
test___rstring_array_join_cstr___should_JoinStrings(void)
{
  int size = 0;
  rstring* actual = NULL;
  char* sep = NULL;

  rstring* strings[3] = { rstring_new("apple"), rstring_new("pie"), rstring_new("good") };
  size = 3;
  rstring_array* rary = rstring_array_new();
  rstring_array_push_cstr(rary, "apple");
  rstring_array_push_cstr(rary, "pie");
  rstring_array_push_cstr(rary, "good");

  sep = "";
  actual = rstring_array_join_cstr(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("applepiegood", actual);
  rstring_free(actual);

  sep = "/";
  actual = rstring_array_join_cstr(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("apple/pie/good", actual);
  rstring_free(actual);

  sep = "//";
  actual = rstring_array_join_cstr(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("apple//pie//good", actual);
  rstring_free(actual);

  rstring_array_free(rary);
  rstring_free(strings[0]);
  rstring_free(strings[1]);
  rstring_free(strings[2]);


  /* Empty array */
  rary = rstring_array_new();

  sep = ".";
  actual = rstring_array_join_cstr(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("", actual);
  rstring_free(actual);

  rstring_array_free(rary);

  /* Single element */
  rary = rstring_array_new();
  rstring_array_push_cstr(rary, "apple");

  sep = ".";
  actual = rstring_array_join_cstr(rary, sep);
  TEST_ASSERT_EQUAL_RSTRING("apple", actual);
  rstring_free(actual);

  rstring_array_free(rary);

}


void
test___rstring_split___should_SplitString(void)
{
  rstring_array* actual = NULL;
  rstring* sep = NULL;
  rstring* rstr = NULL;

  actual = rstring_split((rstr = rstring_new("")), (sep = rstring_new("/")));
  TEST_ASSERT_EQUAL_RSTRING("", actual->entry[0]);
  TEST_ASSERT_EQUAL(1, actual->qty);
  rstring_free(rstr);
  rstring_free(sep);
  rstring_array_free(actual);


  actual = rstring_split((rstr = rstring_new("apple")), (sep = rstring_new("/")));
  TEST_ASSERT_EQUAL_RSTRING("apple", actual->entry[0]);
  TEST_ASSERT_EQUAL(1, actual->qty);
  /* Also it doesn't change the input */
  TEST_ASSERT_EQUAL_RSTRING("apple", rstr);
  rstring_free(rstr);
  rstring_free(sep);
  rstring_array_free(actual);

  actual = rstring_split((rstr = rstring_new("apple/pie")), (sep = rstring_new("/")));
  TEST_ASSERT_EQUAL_RSTRING("apple", actual->entry[0]);
  TEST_ASSERT_EQUAL_RSTRING("pie", actual->entry[1]);
  TEST_ASSERT_EQUAL(2, actual->qty);
  rstring_free(rstr);
  rstring_free(sep);
  rstring_array_free(actual);

  /* It can take strings of whatever length as seps */
  actual = rstring_split((rstr = rstring_new("apple//pie")), (sep = rstring_new("//")));
  TEST_ASSERT_EQUAL_RSTRING("apple", actual->entry[0]);
  TEST_ASSERT_EQUAL_RSTRING("pie", actual->entry[1]);
  TEST_ASSERT_EQUAL(2, actual->qty);
  rstring_free(rstr);
  rstring_free(sep);
  rstring_array_free(actual);

  /* It doesn't match part of the string, rather the whole thing. */
  actual = rstring_split((rstr = rstring_new("apple/pie")), (sep = rstring_new("//")));
  TEST_ASSERT_EQUAL_RSTRING("apple/pie", actual->entry[0]);
  TEST_ASSERT_EQUAL(1, actual->qty);
  rstring_free(rstr);
  rstring_free(sep);
  rstring_array_free(actual);
}

void
test___rstring_split_cstr___should_SplitString(void)
{
  rstring_array* actual = NULL;
  char* sep = NULL;
  rstring* rstr = NULL;

  actual = rstring_split_cstr((rstr = rstring_new("")), "/");
  TEST_ASSERT_EQUAL_RSTRING("", actual->entry[0]);
  TEST_ASSERT_EQUAL(1, actual->qty);
  rstring_free(rstr);
  rstring_array_free(actual);


  actual = rstring_split_cstr((rstr = rstring_new("apple")), "/");
  TEST_ASSERT_EQUAL_RSTRING("apple", actual->entry[0]);
  TEST_ASSERT_EQUAL(1, actual->qty);
  /* Also it doesn't change the input */
  TEST_ASSERT_EQUAL_RSTRING("apple", rstr);
  rstring_free(rstr);
  rstring_array_free(actual);

  actual = rstring_split_cstr((rstr = rstring_new("apple/pie")), "/");
  TEST_ASSERT_EQUAL_RSTRING("apple", actual->entry[0]);
  TEST_ASSERT_EQUAL_RSTRING("pie", actual->entry[1]);
  TEST_ASSERT_EQUAL(2, actual->qty);
  rstring_free(rstr);
  rstring_array_free(actual);

  /* It can take strings of whatever length as seps */
  actual = rstring_split_cstr((rstr = rstring_new("apple//pie")), "//");
  TEST_ASSERT_EQUAL_RSTRING("apple", actual->entry[0]);
  TEST_ASSERT_EQUAL_RSTRING("pie", actual->entry[1]);
  TEST_ASSERT_EQUAL(2, actual->qty);
  rstring_free(rstr);
  rstring_array_free(actual);

  /* It doesn't match part of the string, rather the whole thing. */
  actual = rstring_split_cstr((rstr = rstring_new("apple/pie")), "//");
  TEST_ASSERT_EQUAL_RSTRING("apple/pie", actual->entry[0]);
  TEST_ASSERT_EQUAL(1, actual->qty);
  rstring_free(rstr);
  rstring_array_free(actual);
}

void
test___rstring_array_push_cstr___should_AddTheCstr(void)
{
  rstring_array* rary = rstring_array_new();

  rstring_array_push_cstr(rary, "apple");
  TEST_ASSERT_EQUAL_RSTRING("apple", rary->entry[0]);
  TEST_ASSERT_EQUAL(1, rary->qty);
  TEST_ASSERT(rary->mlen >= rary->qty);

  rstring_array_push_cstr(rary, "pie");
  TEST_ASSERT_EQUAL_RSTRING("apple", rary->entry[0]);
  TEST_ASSERT_EQUAL_RSTRING("pie", rary->entry[1]);
  TEST_ASSERT_EQUAL(2, rary->qty);
  TEST_ASSERT(rary->mlen >= rary->qty);

  rstring_array_free(rary);
}

void
test___rstring_array_push_rstr___should_AddTheCstr(void)
{
  rstring_array* rary = rstring_array_new();

  rstring_array_push_rstr(rary, rstring_new("apple"));
  TEST_ASSERT_EQUAL_RSTRING("apple", rary->entry[0]);
  TEST_ASSERT_EQUAL(1, rary->qty);
  TEST_ASSERT(rary->mlen >= rary->qty);

  rstring_array_push_rstr(rary, rstring_new("pie"));
  TEST_ASSERT_EQUAL_RSTRING("apple", rary->entry[0]);
  TEST_ASSERT_EQUAL_RSTRING("pie", rary->entry[1]);
  TEST_ASSERT_EQUAL(2, rary->qty);
  TEST_ASSERT(rary->mlen >= rary->qty);

  rstring_array_free(rary);
}

void
test___rstring_array_get___should_ReturnElemAtIndex(void)
{
  rstring_array* rary = rstring_array_new();

  TEST_ASSERT_NULL(rstring_array_get(rary, -1));
  TEST_ASSERT_NULL(rstring_array_get(rary, 0));
  TEST_ASSERT_NULL(rstring_array_get(rary, 1));

  rstring_array_push_cstr(rary, "apple");
  TEST_ASSERT_EQUAL_RSTRING("apple", rstring_array_get(rary, 0));
  TEST_ASSERT_NULL(rstring_array_get(rary, 1))

    rstring_array_push_cstr(rary, "pie");
  TEST_ASSERT_EQUAL_RSTRING("apple", rstring_array_get(rary, 0));
  TEST_ASSERT_EQUAL_RSTRING("pie", rstring_array_get(rary, 1));
  TEST_ASSERT_NULL(rstring_array_get(rary, 2));

  rstring_array_free(rary);
}
