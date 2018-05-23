#include "unity.h"
#include "file.h"

void test___file_extname___should_ReturnTheExtension(void)
{
  TEST_ASSERT_EQUAL_STRING(".c", file_extname("test.c"));
  TEST_ASSERT_EQUAL_STRING("", file_extname("te.s/t"));
  TEST_ASSERT_EQUAL_STRING("", file_extname("te.s/t."));
  TEST_ASSERT_EQUAL_STRING("", file_extname(".te.s/t."));
  TEST_ASSERT_EQUAL_STRING(".c", file_extname(".te.s/t.c"));
  TEST_ASSERT_EQUAL_STRING(".c", file_extname("a/b/d/te.st.c"));
  TEST_ASSERT_EQUAL_STRING(".c", file_extname(".a/b/d/test.c"));
  TEST_ASSERT_EQUAL_STRING("", file_extname("test"));
  TEST_ASSERT_EQUAL_STRING("", file_extname("test."));
  TEST_ASSERT_EQUAL_STRING("", file_extname(".test."));
  TEST_ASSERT_EQUAL_STRING("", file_extname(".profile"));
  TEST_ASSERT_EQUAL_STRING(".sh", file_extname(".profile.sh"));
  TEST_ASSERT_EQUAL_STRING(".rb", file_extname("/tmp//bla.rb"));

  /* See https://github.com/ruby/ruby/blob/e6ad53beaa8f61c784d7e6c9cace5bd6ecc4d5c8/test/ruby/test_file_exhaustive.rb#L1229 */
  char* prefixes[6] = { "", "/", ".", "/.", "bar/.", "/bar/." };
  char* infixes[3] = { "", " ", "." };
  char* infixes2[4] = { "", " ", ".", ".ext " };
  char* appendices[4] = { "", " ", ".", ".bar" };

  char* buf = malloc(sizeof(char) * 100);
  /* TODO: assert mem */

  for (int pi = 0; pi < 6; ++pi) { /* prefixes */
    for (int ai = 0; ai < 4; ++ai) { /* appendices */
      for (int ii = 0; ii < 3; ++ii) { /* infixes */
        sprintf(buf,
                "%sfoo%s%s",
                prefixes[pi],
                infixes[ii],
                appendices[ai]);

        if (strcmp(infixes[ii], ".") == 0 && strcmp(appendices[ai], " ") == 0) {
          TEST_ASSERT_EQUAL_STRING_MESSAGE(". ", file_extname(buf), buf);
        }
        else if (strcmp(appendices[ai], ".bar") == 0) {
          TEST_ASSERT_EQUAL_STRING_MESSAGE(".bar", file_extname(buf), buf);
        }
        else {
          TEST_ASSERT_EQUAL_STRING_MESSAGE("", file_extname(buf), buf);
        }
      }

      for (int ii = 0; ii < 4; ++ii) { /* infixes2 */
        sprintf(buf,
                "%sfoo%s.ext%s",
                prefixes[pi],
                infixes2[ii],
                appendices[ai]);

        if (strcmp(appendices[ai], ".") == 0) {
          TEST_ASSERT_EQUAL_STRING_MESSAGE("", file_extname(buf), buf);
        }
        else if (strcmp(appendices[ai], ".bar") == 0) {
          TEST_ASSERT_EQUAL_STRING_MESSAGE(".bar", file_extname(buf), buf);
        }
        else if (strcmp(appendices[ai], " ") == 0) {
          TEST_ASSERT_EQUAL_STRING_MESSAGE(".ext ", file_extname(buf), buf);
        }
        else {
          TEST_ASSERT_EQUAL_STRING_MESSAGE(".ext", file_extname(buf), buf);
        }
      }
    }
  }
}
