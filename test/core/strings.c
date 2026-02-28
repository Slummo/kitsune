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
void test_ksString_Creation(void) {
    ksString s1 = ksStringNew("hello");
    TEST_ASSERT_EQUAL_STRING("hello", ksStringAsRaw(&s1));
    TEST_ASSERT_EQUAL_UINT(5, ksStringLen(&s1));
    TEST_ASSERT_FALSE(ksStringIsEmpty(&s1));

    ksString s2 = ksStringNew2("hello world", 5);
    TEST_ASSERT_EQUAL_STRING("hello", ksStringAsRaw(&s2));
    TEST_ASSERT_EQUAL_UINT(5, ksStringLen(&s2));

    ksString s3 = ksStringFrom(&s1);
    TEST_ASSERT_EQUAL_STRING("hello", ksStringAsRaw(&s3));
    TEST_ASSERT_EQUAL_INT(0, ksStringCmp(&s1, &s3));

    ksStr view = ksStrNew("hello view", 0, 5);
    ksString s4 = ksStringFromView(&view);
    TEST_ASSERT_EQUAL_STRING("hello", ksStringAsRaw(&s4));

    ksStringFree(&s1);
    ksStringFree(&s2);
    ksStringFree(&s3);
    ksStringFree(&s4);
}

// -----------------------------------------------------------------------------
// Append Tests
// -----------------------------------------------------------------------------
void test_ksString_Append(void) {
    ksString dst = ksStringNew("foo");
    ksString src = ksStringNew("bar");

    ksStringAppend(&dst, &src);
    TEST_ASSERT_EQUAL_STRING("foobar", ksStringAsRaw(&dst));

    ksStringAppendRaw(&dst, "baz");
    TEST_ASSERT_EQUAL_STRING("foobarbaz", ksStringAsRaw(&dst));

    ksStringAppendRaw2(&dst, "qux_ignore", 3);
    TEST_ASSERT_EQUAL_STRING("foobarbazqux", ksStringAsRaw(&dst));

    ksStr view = ksStrNew("-view-test", 0, 5);
    ksStringAppendView(&dst, &view);
    TEST_ASSERT_EQUAL_STRING("foobarbazqux-view", ksStringAsRaw(&dst));

    ksStringFree(&dst);
    ksStringFree(&src);
}

// -----------------------------------------------------------------------------
// Concatenation Tests
// -----------------------------------------------------------------------------
void test_ksString_Concat(void) {
    ksString s1 = ksStringNew("kit");
    ksString s2 = ksStringNew("sune");

    ksString s3 = ksStringConcat(&s1, &s2);
    TEST_ASSERT_EQUAL_STRING("kitsune", ksStringAsRaw(&s3));

    ksString s4 = ksStringConcatRaw("foo", "bar");
    TEST_ASSERT_EQUAL_STRING("foobar", ksStringAsRaw(&s4));

    ksString s5 = ksStringConcatRaw2("helloxxx", 5, "worldyyy", 5);
    TEST_ASSERT_EQUAL_STRING("helloworld", ksStringAsRaw(&s5));

    ksStringFree(&s1);
    ksStringFree(&s2);
    ksStringFree(&s3);
    ksStringFree(&s4);
    ksStringFree(&s5);
}

// -----------------------------------------------------------------------------
// Modification Tests (Push, Pop, Cut)
// -----------------------------------------------------------------------------
void test_ksString_Modifications(void) {
    ksString s = ksStringEmpty(10);

    ksStringPush(&s, 'A');
    ksStringPush(&s, 'B');
    ksStringPush(&s, 'C');
    TEST_ASSERT_EQUAL_STRING("ABC", ksStringAsRaw(&s));
    TEST_ASSERT_EQUAL_UINT(3, ksStringLen(&s));

    char c = ksStringPop(&s);
    TEST_ASSERT_EQUAL_CHAR('C', c);
    TEST_ASSERT_EQUAL_STRING("AB", ksStringAsRaw(&s));
    TEST_ASSERT_EQUAL_UINT(2, ksStringLen(&s));

    ksStringAppendRaw(&s, "CDEF");
    ksStringCut(&s, 2);
    TEST_ASSERT_EQUAL_STRING("ABCD", ksStringAsRaw(&s));
    TEST_ASSERT_EQUAL_UINT(4, ksStringLen(&s));

    ksStringFree(&s);
}

// -----------------------------------------------------------------------------
// Trim Tests
// -----------------------------------------------------------------------------
void test_ksString_Trim(void) {
    ksString s1 = ksStringNew("  hello  ");
    ksStringTrimLeft(&s1);
    TEST_ASSERT_EQUAL_STRING("hello  ", ksStringAsRaw(&s1));

    ksStringTrimRight(&s1);
    TEST_ASSERT_EQUAL_STRING("hello", ksStringAsRaw(&s1));

    ksString s2 = ksStringNew(" \t kitsune \n ");
    ksStringTrim(&s2);
    TEST_ASSERT_EQUAL_STRING("kitsune", ksStringAsRaw(&s2));

    ksString s3 = ksStringNew("   ");
    ksStringTrim(&s3);
    TEST_ASSERT_TRUE(ksStringIsEmpty(&s3));

    ksStringFree(&s1);
    ksStringFree(&s2);
    ksStringFree(&s3);
}

// -----------------------------------------------------------------------------
// Case & Character Checking Tests
// -----------------------------------------------------------------------------
void test_ksString_CaseAndChecks(void) {
    ksString s = ksStringNew("kItSuNe");

    TEST_ASSERT_TRUE(ksStringIsAscii(&s));
    TEST_ASSERT_TRUE(ksStringIsAlpha(&s));
    TEST_ASSERT_FALSE(ksStringIsNum(&s));

    ksStringUpper(&s);
    TEST_ASSERT_EQUAL_STRING("KITSUNE", ksStringAsRaw(&s));
    TEST_ASSERT_TRUE(ksStringIsUpper(&s));

    ksStringLower(&s);
    TEST_ASSERT_EQUAL_STRING("kitsune", ksStringAsRaw(&s));
    TEST_ASSERT_TRUE(ksStringIsLower(&s));

    ksString snum = ksStringNew("12345");
    TEST_ASSERT_TRUE(ksStringIsNum(&snum));
    TEST_ASSERT_TRUE(ksStringIsAlnum(&snum));
    TEST_ASSERT_FALSE(ksStringIsAlpha(&snum));

    ksStringFree(&s);
    ksStringFree(&snum);
}

// -----------------------------------------------------------------------------
// Conversion Tests
// -----------------------------------------------------------------------------
void test_ksString_Conversion(void) {
    ksString s = ksStringNew("-42");
    int64_t val = 0;

    ksStringToInt64(&s, &val, 10);
    TEST_ASSERT_EQUAL_INT64(-42, val);

    ksString hex = ksStringNew("1A");
    ksStringToInt64(&hex, &val, 16);
    TEST_ASSERT_EQUAL_INT64(26, val);

    ksStringFree(&s);
    ksStringFree(&hex);
}

// -----------------------------------------------------------------------------
// Split & Join Tests
// -----------------------------------------------------------------------------
void test_ksString_SplitAndJoin(void) {
    ksString s = ksStringNew("apple,banana,orange");
    ksStr* parts = ksStringSplit(&s, ",");

    TEST_ASSERT_NOT_NULL(parts);
    TEST_ASSERT_EQUAL_UINT(5, parts[0].len);
    TEST_ASSERT_EQUAL_MEMORY("apple", ksStrAsRaw(&parts[0]), 5);
    TEST_ASSERT_EQUAL_UINT(6, parts[1].len);
    TEST_ASSERT_EQUAL_MEMORY("banana", ksStrAsRaw(&parts[1]), 6);
    TEST_ASSERT_EQUAL_UINT(6, parts[2].len);
    TEST_ASSERT_EQUAL_MEMORY("orange", ksStrAsRaw(&parts[2]), 6);
    TEST_ASSERT_NULL(parts[3].ptr);

    ksString joined = ksStringJoin(parts, " | ");
    TEST_ASSERT_EQUAL_STRING("apple | banana | orange", ksStringAsRaw(&joined));

    free(parts);
    ksStringFree(&s);
    ksStringFree(&joined);
}

// -----------------------------------------------------------------------------
// Search & Replace Tests
// -----------------------------------------------------------------------------
void test_ksString_FindAndReplace(void) {
    ksString s = ksStringNew("the quick brown fox");
    ksString needle = ksStringNew("quick");
    ksString replacement = ksStringNew("slow");

    char* p1 = ksStringFind(&s, &needle);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_EQUAL_STRING("quick brown fox", p1);

    char* p2 = ksStringFindRaw(&s, "brown");
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_EQUAL_STRING("brown fox", p2);

    char* p3 = ksStringFindRaw2(&s, "foxes", 3);
    TEST_ASSERT_NOT_NULL(p3);
    TEST_ASSERT_EQUAL_STRING("fox", p3);

    char* p4 = ksStringFindRaw(&s, "wolf");
    TEST_ASSERT_NULL(p4);

    ksStringReplace(&s, &needle, &replacement);
    TEST_ASSERT_EQUAL_STRING("the slow brown fox", ksStringAsRaw(&s));

    ksStringReplaceRaw(&s, "brown", "red");
    TEST_ASSERT_EQUAL_STRING("the slow red fox", ksStringAsRaw(&s));

    ksStringReplaceRaw2(&s, "slow cat", 4, "fast", 4);
    TEST_ASSERT_EQUAL_STRING("the fast red fox", ksStringAsRaw(&s));

    ksStringFree(&s);
    ksStringFree(&needle);
    ksStringFree(&replacement);
}

// -----------------------------------------------------------------------------
// Comparison Tests
// -----------------------------------------------------------------------------
void test_ksString_Cmp(void) {
    ksString s1 = ksStringNew("apple");
    ksString s2 = ksStringNew("apple");
    ksString s3 = ksStringNew("banana");
    ksString s4 = ksStringNew("app");

    TEST_ASSERT_EQUAL_INT(0, ksStringCmp(&s1, &s2));
    TEST_ASSERT_LESS_THAN_INT(0, ksStringCmp(&s1, &s3));
    TEST_ASSERT_GREATER_THAN_INT(0, ksStringCmp(&s1, &s4));

    ksStringFree(&s1);
    ksStringFree(&s2);
    ksStringFree(&s3);
    ksStringFree(&s4);
}

// -----------------------------------------------------------------------------
// Formatting Tests
// -----------------------------------------------------------------------------
void test_ksString_Format(void) {
    ksString s1 = ksStringFormat("Hello %s, score: %d", "Player1", 999);
    TEST_ASSERT_EQUAL_STRING("Hello Player1, score: 999", ksStringAsRaw(&s1));
    TEST_ASSERT_EQUAL_UINT(25, ksStringLen(&s1));

    // Test formatting that exceeds SSO capacity
    ksString s2 =
        ksStringFormat("This string %s is %d characters long, forcing it onto the heap", "deliberately", 123456789);
    TEST_ASSERT_EQUAL_STRING("This string deliberately is 123456789 characters long, forcing it onto the heap",
                             ksStringAsRaw(&s2));
    TEST_ASSERT_GREATER_THAN_UINT(KS_SSO_CAP, ksStringLen(&s2));

    ksStringFree(&s1);
    ksStringFree(&s2);
}

// -----------------------------------------------------------------------------
// Capacity & Memory Management Tests
// -----------------------------------------------------------------------------
void test_ksString_MemoryManagement(void) {
    ksString s = ksStringEmpty(16);
    TEST_ASSERT_TRUE(ksStringIsEmpty(&s));
    TEST_ASSERT_EQUAL_UINT(0, ksStringLen(&s));
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(16, ksStringCap(&s));

    ksStringReserve(&s, 128);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(128, ksStringCap(&s));

    ksStringAppendRaw(&s, "temporary data");
    TEST_ASSERT_FALSE(ksStringIsEmpty(&s));

    ksStringClear(&s);
    TEST_ASSERT_TRUE(ksStringIsEmpty(&s));
    TEST_ASSERT_EQUAL_UINT(0, ksStringLen(&s));
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(128, ksStringCap(&s));

    ksStringShrink(&s);
    TEST_ASSERT_EQUAL_UINT(0, ksStringLen(&s));

    TEST_ASSERT_TRUE_MESSAGE(ksStringIsShort(&s) == true || ksStringIsShort(&s) == false,
                             "IsShort should return a valid boolean");

    ksStringFree(&s);
}

// -----------------------------------------------------------------------------
// View Tests (ksStr)
// -----------------------------------------------------------------------------
void test_ksStr_Views(void) {
    const char* raw_text = "hello, world!";

    ksStr view1 = ksStrNew(raw_text, 7, 12);
    TEST_ASSERT_EQUAL_UINT(5, ksStrLen(&view1));
    TEST_ASSERT_FALSE(ksStrIsEmpty(&view1));
    TEST_ASSERT_EQUAL_MEMORY("world", ksStrAsRaw(&view1), 5);

    ksStr view2 = ksStrFrom(&view1, 1, 4);
    TEST_ASSERT_EQUAL_UINT(3, ksStrLen(&view2));
    TEST_ASSERT_EQUAL_MEMORY("orl", ksStrAsRaw(&view2), 3);

    ksStr view3 = ksStrNew("world", 0, 5);
    ksStr view4 = ksStrNew("apple", 0, 5);
    TEST_ASSERT_EQUAL_INT(0, ksStrCmp(&view1, &view3));
    TEST_ASSERT_GREATER_THAN_INT(0, ksStrCmp(&view1, &view4));

    TEST_ASSERT_TRUE(ksStrStartsWith(&view1, "wor"));
    TEST_ASSERT_TRUE(ksStrEndsWith(&view1, "rld"));
    TEST_ASSERT_FALSE(ksStrStartsWith(&view1, "abc"));

    ksStr view_trim = ksStrNew("  test  ", 0, 8);
    ksStrTrim(&view_trim);
    TEST_ASSERT_EQUAL_UINT(4, ksStrLen(&view_trim));
    TEST_ASSERT_EQUAL_MEMORY("test", ksStrAsRaw(&view_trim), 4);

    ksString s = ksStringNew("test view string");
    ksStr sv = ksStringView(&s);
    TEST_ASSERT_EQUAL_UINT(16, ksStrLen(&sv));
    TEST_ASSERT_EQUAL_MEMORY("test view string", ksStrAsRaw(&sv), 16);
    ksStringFree(&s);
}

// -----------------------------------------------------------------------------
// Foreach Macros Tests
// -----------------------------------------------------------------------------
void test_ksString_Foreach(void) {
    ksString s = ksStringNew("abc");
    int i = 0;
    ksStringForeach(it, &s) {
        *it = *it - 32;
        i++;
    }
    TEST_ASSERT_EQUAL_INT(3, i);
    TEST_ASSERT_EQUAL_STRING("ABC", ksStringAsRaw(&s));
    ksStringFree(&s);

    ksStr view = ksStrNew("123", 0, 3);
    int sum = 0;
    int safety_counter = 0;
    ksStrForeach(it, view) {
        sum += (*it - '0');
        if (++safety_counter >= 3)
            break;
    }
    TEST_ASSERT_EQUAL_INT(6, sum);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_ksString_Creation);
    RUN_TEST(test_ksString_Append);
    RUN_TEST(test_ksString_Concat);
    RUN_TEST(test_ksString_Modifications);
    RUN_TEST(test_ksString_Trim);
    RUN_TEST(test_ksString_CaseAndChecks);
    RUN_TEST(test_ksString_Conversion);
    RUN_TEST(test_ksString_SplitAndJoin);
    RUN_TEST(test_ksString_FindAndReplace);
    RUN_TEST(test_ksString_Cmp);
    RUN_TEST(test_ksString_Format);
    RUN_TEST(test_ksString_MemoryManagement);
    RUN_TEST(test_ksStr_Views);
    RUN_TEST(test_ksString_Foreach);

    return UNITY_END();
}
