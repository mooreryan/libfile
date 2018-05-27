#define TEST_ASSERT_EQUAL_RSTRING(expected, actual) TEST_ASSERT_TRUE(biseqcstr((const_bstring)actual, expected))
#define TEST_ASSERT_EQUAL_RSTRING_MESSAGE(expected, actual, msg) TEST_ASSERT_TRUE_MESSAGE(biseqcstr((const_bstring)actual, expected), msg)
#define TEST_ASSERT_RTRUE(actual) TEST_ASSERT_EQUAL(RTRUE, actual)
#define TEST_ASSERT_RFALSE(actual) TEST_ASSERT_EQUAL(RFALSE, actual)
#define TEST_ASSERT_RERROR(actual) TEST_ASSERT_EQUAL(RERROR, actual)

#define TEST_ASSERT_EQUAL_RSTRING_ARRAY(expected, actual) \
  do { \
    TEST_ASSERT_EQUAL(1, rstring_array_eql(expected, actual)); \
  } while (0)
