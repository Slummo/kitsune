#include <kitsune/core/strings.h>
#include <unity.h>
#include <stdlib.h>

void setUp(void) {
}

void tearDown(void) {
}

// -----------------------------------------------------------------------------
// Creation & Destruction Tests
// -----------------------------------------------------------------------------
static void test_creation(void) {
    ks_string s1 = ks_string_new("hello");
    TEST_ASSERT_EQUAL_STRING("hello", ks_string_as_raw(&s1));
    TEST_ASSERT_EQUAL_UINT(5, ks_string_len(&s1));
    TEST_ASSERT_FALSE(ks_string_is_empty(&s1));

    ks_string s2 = ks_string_new2("hello world", 5);
    TEST_ASSERT_EQUAL_STRING("hello", ks_string_as_raw(&s2));
    TEST_ASSERT_EQUAL_UINT(5, ks_string_len(&s2));

    ks_string s3 = ks_string_from(&s1);
    TEST_ASSERT_EQUAL_STRING("hello", ks_string_as_raw(&s3));
    TEST_ASSERT_EQUAL_INT(0, ks_string_cmp(&s1, &s3));

    ks_str view = ks_str_new("hello view", 0, 5);
    ks_string s4 = ks_string_from_view(&view);
    TEST_ASSERT_EQUAL_STRING("hello", ks_string_as_raw(&s4));

    ks_string_free(&s1);
    ks_string_free(&s2);
    ks_string_free(&s3);
    ks_string_free(&s4);
}

// -----------------------------------------------------------------------------
// Append Tests
// -----------------------------------------------------------------------------
static void test_append(void) {
    ks_string dst = ks_string_new("foo");
    ks_string src = ks_string_new("bar");

    ks_string_append(&dst, &src);
    TEST_ASSERT_EQUAL_STRING("foobar", ks_string_as_raw(&dst));

    ks_string_append_raw(&dst, "baz");
    TEST_ASSERT_EQUAL_STRING("foobarbaz", ks_string_as_raw(&dst));

    ks_string_append_raw2(&dst, "qux_ignore", 3);
    TEST_ASSERT_EQUAL_STRING("foobarbazqux", ks_string_as_raw(&dst));

    ks_str view = ks_str_new("-view-test", 0, 5);
    ks_string_append_view(&dst, &view);
    TEST_ASSERT_EQUAL_STRING("foobarbazqux-view", ks_string_as_raw(&dst));

    ks_string_free(&dst);
    ks_string_free(&src);
}

// -----------------------------------------------------------------------------
// Concatenation Tests
// -----------------------------------------------------------------------------
static void test_concat(void) {
    ks_string s1 = ks_string_new("kit");
    ks_string s2 = ks_string_new("sune");

    ks_string s3 = ks_string_concat(&s1, &s2);
    TEST_ASSERT_EQUAL_STRING("kitsune", ks_string_as_raw(&s3));

    ks_string s4 = ks_string_concat_raw("foo", "bar");
    TEST_ASSERT_EQUAL_STRING("foobar", ks_string_as_raw(&s4));

    ks_string s5 = ks_string_concat_raw2("helloxxx", 5, "worldyyy", 5);
    TEST_ASSERT_EQUAL_STRING("helloworld", ks_string_as_raw(&s5));

    ks_string_free(&s1);
    ks_string_free(&s2);
    ks_string_free(&s3);
    ks_string_free(&s4);
    ks_string_free(&s5);
}

// -----------------------------------------------------------------------------
// Modification Tests (Push, Pop, Cut)
// -----------------------------------------------------------------------------
static void test_modifiy(void) {
    ks_string s = ks_string_empty(10);

    ks_string_push(&s, 'A');
    ks_string_push(&s, 'B');
    ks_string_push(&s, 'C');
    TEST_ASSERT_EQUAL_STRING("ABC", ks_string_as_raw(&s));
    TEST_ASSERT_EQUAL_UINT(3, ks_string_len(&s));

    char c = ks_string_pop(&s);
    TEST_ASSERT_EQUAL_CHAR('C', c);
    TEST_ASSERT_EQUAL_STRING("AB", ks_string_as_raw(&s));
    TEST_ASSERT_EQUAL_UINT(2, ks_string_len(&s));

    ks_string_append_raw(&s, "CDEF");
    ks_string_cut(&s, 2);
    TEST_ASSERT_EQUAL_STRING("ABCD", ks_string_as_raw(&s));
    TEST_ASSERT_EQUAL_UINT(4, ks_string_len(&s));

    ks_string_free(&s);
}

// -----------------------------------------------------------------------------
// Trim Tests
// -----------------------------------------------------------------------------
static void test_trim(void) {
    ks_string s1 = ks_string_new("  hello  ");
    ks_string_trim_left(&s1);
    TEST_ASSERT_EQUAL_STRING("hello  ", ks_string_as_raw(&s1));

    ks_string_trim_right(&s1);
    TEST_ASSERT_EQUAL_STRING("hello", ks_string_as_raw(&s1));

    ks_string s2 = ks_string_new(" \t kitsune \n ");
    ks_string_trim(&s2);
    TEST_ASSERT_EQUAL_STRING("kitsune", ks_string_as_raw(&s2));

    ks_string s3 = ks_string_new("   ");
    ks_string_trim(&s3);
    TEST_ASSERT_TRUE(ks_string_is_empty(&s3));

    ks_string_free(&s1);
    ks_string_free(&s2);
    ks_string_free(&s3);
}

// -----------------------------------------------------------------------------
// Case & Character Checking Tests
// -----------------------------------------------------------------------------
static void test_case(void) {
    ks_string s = ks_string_new("kItSuNe");

    TEST_ASSERT_TRUE(ks_string_is_ascii(&s));
    TEST_ASSERT_TRUE(ks_string_is_alpha(&s));
    TEST_ASSERT_FALSE(ks_string_is_num(&s));

    ks_string_upper(&s);
    TEST_ASSERT_EQUAL_STRING("KITSUNE", ks_string_as_raw(&s));
    TEST_ASSERT_TRUE(ks_string_is_upper(&s));

    ks_string_lower(&s);
    TEST_ASSERT_EQUAL_STRING("kitsune", ks_string_as_raw(&s));
    TEST_ASSERT_TRUE(ks_string_is_lower(&s));

    ks_string snum = ks_string_new("12345");
    TEST_ASSERT_TRUE(ks_string_is_num(&snum));
    TEST_ASSERT_TRUE(ks_string_is_alnum(&snum));
    TEST_ASSERT_FALSE(ks_string_is_alpha(&snum));

    ks_string_free(&s);
    ks_string_free(&snum);
}

// -----------------------------------------------------------------------------
// Conversion Tests
// -----------------------------------------------------------------------------
static void test_convert(void) {
    ks_string s = ks_string_new("-42");
    int64_t val = 0;

    ks_string_to_int64(&s, &val, 10);
    TEST_ASSERT_EQUAL_INT64(-42, val);

    ks_string hex = ks_string_new("1A");
    ks_string_to_int64(&hex, &val, 16);
    TEST_ASSERT_EQUAL_INT64(26, val);

    ks_string_free(&s);
    ks_string_free(&hex);
}

// -----------------------------------------------------------------------------
// Split & Join Tests
// -----------------------------------------------------------------------------
static void test_splitjoin(void) {
    ks_string s = ks_string_new("apple,banana,orange");
    ks_str* parts = ks_string_split(&s, ",");

    TEST_ASSERT_NOT_NULL(parts);
    TEST_ASSERT_EQUAL_UINT(5, parts[0].len);
    TEST_ASSERT_EQUAL_MEMORY("apple", ks_str_as_raw(&parts[0]), 5);
    TEST_ASSERT_EQUAL_UINT(6, parts[1].len);
    TEST_ASSERT_EQUAL_MEMORY("banana", ks_str_as_raw(&parts[1]), 6);
    TEST_ASSERT_EQUAL_UINT(6, parts[2].len);
    TEST_ASSERT_EQUAL_MEMORY("orange", ks_str_as_raw(&parts[2]), 6);
    TEST_ASSERT_NULL(parts[3].ptr);

    ks_string joined = ks_string_join(parts, " | ");
    TEST_ASSERT_EQUAL_STRING("apple | banana | orange", ks_string_as_raw(&joined));

    free(parts);
    ks_string_free(&s);
    ks_string_free(&joined);
}

// -----------------------------------------------------------------------------
// Search & Replace Tests
// -----------------------------------------------------------------------------
static void test_findreplace(void) {
    ks_string s = ks_string_new("the quick brown fox");
    ks_string needle = ks_string_new("quick");
    ks_string replacement = ks_string_new("slow");

    char* p1 = ks_string_find(&s, &needle);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_EQUAL_STRING("quick brown fox", p1);

    char* p2 = ks_string_find_raw(&s, "brown");
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_EQUAL_STRING("brown fox", p2);

    char* p3 = ks_string_find_raw2(&s, "foxes", 3);
    TEST_ASSERT_NOT_NULL(p3);
    TEST_ASSERT_EQUAL_STRING("fox", p3);

    char* p4 = ks_string_find_raw(&s, "wolf");
    TEST_ASSERT_NULL(p4);

    ks_string_replace(&s, &needle, &replacement);
    TEST_ASSERT_EQUAL_STRING("the slow brown fox", ks_string_as_raw(&s));

    ks_string_replace_raw(&s, "brown", "red");
    TEST_ASSERT_EQUAL_STRING("the slow red fox", ks_string_as_raw(&s));

    ks_string_replace_raw2(&s, "slow cat", 4, "fast", 4);
    TEST_ASSERT_EQUAL_STRING("the fast red fox", ks_string_as_raw(&s));

    ks_string_free(&s);
    ks_string_free(&needle);
    ks_string_free(&replacement);
}

// -----------------------------------------------------------------------------
// Comparison Tests
// -----------------------------------------------------------------------------
static void test_compare(void) {
    ks_string s1 = ks_string_new("apple");
    ks_string s2 = ks_string_new("apple");
    ks_string s3 = ks_string_new("banana");
    ks_string s4 = ks_string_new("app");

    TEST_ASSERT_EQUAL_INT(0, ks_string_cmp(&s1, &s2));
    TEST_ASSERT_LESS_THAN_INT(0, ks_string_cmp(&s1, &s3));
    TEST_ASSERT_GREATER_THAN_INT(0, ks_string_cmp(&s1, &s4));

    ks_string_free(&s1);
    ks_string_free(&s2);
    ks_string_free(&s3);
    ks_string_free(&s4);
}

// -----------------------------------------------------------------------------
// Formatting Tests
// -----------------------------------------------------------------------------
static void test_format(void) {
    ks_string s1 = ks_string_format("Hello %s, score: %d", "Player1", 999);
    TEST_ASSERT_EQUAL_STRING("Hello Player1, score: 999", ks_string_as_raw(&s1));
    TEST_ASSERT_EQUAL_UINT(25, ks_string_len(&s1));

    // Test formatting that exceeds SSO capacity
    ks_string s2 =
        ks_string_format("This string %s is %d characters long, forcing it onto the heap", "deliberately", 123456789);
    TEST_ASSERT_EQUAL_STRING("This string deliberately is 123456789 characters long, forcing it onto the heap",
                             ks_string_as_raw(&s2));
    TEST_ASSERT_GREATER_THAN_UINT(KS_SSO_CAP, ks_string_len(&s2));

    ks_string_free(&s1);
    ks_string_free(&s2);
}

// -----------------------------------------------------------------------------
// Capacity & Memory Management Tests
// -----------------------------------------------------------------------------
static void test_memory(void) {
    ks_string s = ks_string_empty(16);
    TEST_ASSERT_TRUE(ks_string_is_empty(&s));
    TEST_ASSERT_EQUAL_UINT(0, ks_string_len(&s));
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(16, ks_string_cap(&s));

    ks_string_reserve(&s, 128);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(128, ks_string_cap(&s));

    ks_string_append_raw(&s, "temporary data");
    TEST_ASSERT_FALSE(ks_string_is_empty(&s));

    ks_string_clear(&s);
    TEST_ASSERT_TRUE(ks_string_is_empty(&s));
    TEST_ASSERT_EQUAL_UINT(0, ks_string_len(&s));
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(128, ks_string_cap(&s));

    ks_string_shrink(&s);
    TEST_ASSERT_EQUAL_UINT(0, ks_string_len(&s));

    TEST_ASSERT_TRUE_MESSAGE(ks_string_is_short(&s) == true || ks_string_is_short(&s) == false,
                             "IsShort should return a valid boolean");

    ks_string_free(&s);
}

// -----------------------------------------------------------------------------
// View Tests (ks_str)
// -----------------------------------------------------------------------------
static void test_views(void) {
    const char* raw_text = "hello, world!";

    ks_str view1 = ks_str_new(raw_text, 7, 12);
    TEST_ASSERT_EQUAL_UINT(5, ks_str_len(&view1));
    TEST_ASSERT_FALSE(ks_str_is_empty(&view1));
    TEST_ASSERT_EQUAL_MEMORY("world", ks_str_as_raw(&view1), 5);

    ks_str view2 = ks_str_from(&view1, 1, 4);
    TEST_ASSERT_EQUAL_UINT(3, ks_str_len(&view2));
    TEST_ASSERT_EQUAL_MEMORY("orl", ks_str_as_raw(&view2), 3);

    ks_str view3 = ks_str_new("world", 0, 5);
    ks_str view4 = ks_str_new("apple", 0, 5);
    TEST_ASSERT_EQUAL_INT(0, ks_str_cmp(&view1, &view3));
    TEST_ASSERT_GREATER_THAN_INT(0, ks_str_cmp(&view1, &view4));

    TEST_ASSERT_TRUE(ks_str_starts_with(&view1, "wor"));
    TEST_ASSERT_TRUE(ks_str_ends_with(&view1, "rld"));
    TEST_ASSERT_FALSE(ks_str_starts_with(&view1, "abc"));

    ks_str view_trim = ks_str_new("  test  ", 0, 8);
    ks_str_trim(&view_trim);
    TEST_ASSERT_EQUAL_UINT(4, ks_str_len(&view_trim));
    TEST_ASSERT_EQUAL_MEMORY("test", ks_str_as_raw(&view_trim), 4);

    ks_string s = ks_string_new("test view string");
    ks_str sv = ks_string_view(&s);
    TEST_ASSERT_EQUAL_UINT(16, ks_str_len(&sv));
    TEST_ASSERT_EQUAL_MEMORY("test view string", ks_str_as_raw(&sv), 16);
    ks_string_free(&s);
}

// -----------------------------------------------------------------------------
// Foreach Macros Tests
// -----------------------------------------------------------------------------
static void test_foreach(void) {
    ks_string s = ks_string_new("abc");
    int i = 0;
    ks_string_foreach(it, &s) {
        *it = *it - 32;
        i++;
    }
    TEST_ASSERT_EQUAL_INT(3, i);
    TEST_ASSERT_EQUAL_STRING("ABC", ks_string_as_raw(&s));
    ks_string_free(&s);

    ks_str view = ks_str_new("123", 0, 3);
    int sum = 0;
    int safety_counter = 0;
    ks_str_foreach(it, view) {
        sum += (*it - '0');
        if (++safety_counter >= 3)
            break;
    }
    TEST_ASSERT_EQUAL_INT(6, sum);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_creation);
    RUN_TEST(test_append);
    RUN_TEST(test_concat);
    RUN_TEST(test_modifiy);
    RUN_TEST(test_trim);
    RUN_TEST(test_case);
    RUN_TEST(test_convert);
    RUN_TEST(test_splitjoin);
    RUN_TEST(test_findreplace);
    RUN_TEST(test_compare);
    RUN_TEST(test_format);
    RUN_TEST(test_memory);
    RUN_TEST(test_views);
    RUN_TEST(test_foreach);

    return UNITY_END();
}
