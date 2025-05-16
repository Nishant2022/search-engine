#include "ndstring.h"
#include "test_framework.h"

TEST_CASE(String) {
    SECTION(test_default_string) {
        ndash::string str;

        REQUIRE_THAT(str.size(), EQ(0));
        REQUIRE_THAT(str.length(), EQ(0));
        REQUIRE_THAT(str[0], EQ('\0'));
    };

    SECTION(test_char_constructor) {
        ndash::string str(10, 'a');

        REQUIRE_THAT(str.size(), EQ(10));
        for (int i = 0; i < 10; ++i) {
            REQUIRE_THAT(str[i], EQ('a'));
        }

        REQUIRE_THAT(str[10], EQ('\0'));
    };

    SECTION(test_iter_constructor) {
        const char* test_str = "test_str";
        ndash::string str(test_str, test_str + 4);

        REQUIRE_THAT(str.size(), EQ(4));
        for (int i = 0; i < 4; ++i) {
            REQUIRE_THAT(str[i], EQ(test_str[i]));
        }

        REQUIRE_THAT(str[4], NEQ(test_str[4]));
        REQUIRE_THAT(str[4], EQ('\0'));
    };

    SECTION(test_cstr_size_constructor) {
        const char* test_str = "test_str";
        ndash::string str(test_str, 4);

        REQUIRE_THAT(str.size(), EQ(4));
        for (int i = 0; i < 4; ++i) {
            REQUIRE_THAT(str[i], EQ(test_str[i]));
        }

        REQUIRE_THAT(str[4], NEQ(test_str[4]));
        REQUIRE_THAT(str[4], EQ('\0'));
    };

    SECTION(test_cstr_constructor) {
        const char* test_str = "test_str";
        ndash::string str(test_str);

        REQUIRE_THAT(str.size(), EQ(8));
        for (int i = 0; i < 8; ++i) {
            REQUIRE_THAT(str[i], EQ(test_str[i]));
            REQUIRE_THAT(str[i], NEQ('\0'));
        }
        REQUIRE_THAT(str[8], EQ('\0'));
    };

    SECTION(test_ilist_constructor) {
        const char* test_str = "test";
        ndash::string str({ 't', 'e', 's', 't' });

        REQUIRE_THAT(str.size(), EQ(4));
        for (int i = 0; i < 5; ++i) {
            REQUIRE_THAT(str[i], EQ(test_str[i]));
        }
        REQUIRE_THAT(str[4], EQ('\0'));
    };

    SECTION(test_copy_constructors) {
        ndash::string str("test");
        ndash::string copy(str);

        REQUIRE_THAT(copy.size(), EQ(4));
        REQUIRE_THAT(copy.size(), EQ(str.size()));
        REQUIRE_THAT(copy.c_str(), NEQ(str.c_str()));

        for (int i = 0; i < 5; ++i) {
            REQUIRE_THAT(copy[i], EQ(str[i]));
        }

        auto str_loc = str.c_str();
        ndash::string moved(std::move(str));
        REQUIRE_THAT(moved.size(), EQ(4));
        REQUIRE_THAT(str.size(), NEQ(4));
        REQUIRE_THAT(moved.c_str(), EQ(str_loc));

        for (int i = 0; i < 5; ++i) {
            REQUIRE_THAT(moved[i], EQ(copy[i]));
        }
    };

    SECTION(test_assignment_operators) {
        ndash::string str("test");
        ndash::string copy;

        REQUIRE_THAT(copy.size(), EQ(0));
        REQUIRE_THAT(copy.size(), NEQ(str.size()));
        REQUIRE_THAT(copy.c_str(), NEQ(str.c_str()));

        copy = str;

        REQUIRE_THAT(copy.size(), EQ(4));
        REQUIRE_THAT(copy.size(), EQ(str.size()));
        REQUIRE_THAT(copy.c_str(), NEQ(str.c_str()));

        for (int i = 0; i < 5; ++i) {
            REQUIRE_THAT(copy[i], EQ(str[i]));
        }

        auto str_loc = str.c_str();
        ndash::string moved;

        REQUIRE_THAT(moved.size(), EQ(0));
        REQUIRE_THAT(moved.size(), NEQ(str.size()));

        moved = std::move(str);

        REQUIRE_THAT(moved.size(), EQ(4));
        REQUIRE_THAT(str.size(), NEQ(4));
        REQUIRE_THAT(moved.c_str(), EQ(str_loc));

        for (int i = 0; i < 5; ++i) {
            REQUIRE_THAT(moved[i], EQ(copy[i]));
        }

        auto expected = "test_str";
        moved = expected;

        REQUIRE_THAT(moved.size(), EQ(8));
        REQUIRE_THAT(moved.data(), NEQ(expected));
        for (int i = 0; i <= 8; ++i) {
            REQUIRE_THAT(moved[i], EQ(expected[i]));
        }

        expected = "a";
        moved = 'a';
        REQUIRE_THAT(moved.size(), EQ(1));
        for (int i = 0; i <= 1; ++i) {
            REQUIRE_THAT(moved[i], EQ(expected[i]));
        }

        expected = "test";
        moved = { 't', 'e', 's', 't' };
        REQUIRE_THAT(moved.size(), EQ(4));
        for (int i = 0; i <= 4; ++i) {
            REQUIRE_THAT(moved[i], EQ(expected[i]));
        }
    };

    SECTION(test_front_and_back) {
        ndash::string str("abc");
        REQUIRE_THAT(str.front(), EQ('a'));
        REQUIRE_THAT(str.back(), EQ('c'));

        str.front() = 'c';
        str.back() = 'a';

        REQUIRE_THAT(str.front(), EQ('c'));
        REQUIRE_THAT(str.back(), EQ('a'));
    };

    SECTION(test_clear) {
        ndash::string str("test");

        REQUIRE_THAT(str.size(), NEQ(0));
        REQUIRE_THAT(str[0], NEQ('\0'));

        str.clear();

        REQUIRE_THAT(str.size(), EQ(0));
        REQUIRE_THAT(str[0], EQ('\0'));
    };

    SECTION(test_push_and_pop_back) {
        ndash::string str;

        REQUIRE_THAT(str.size(), EQ(0));
        REQUIRE_THAT(str[0], EQ('\0'));

        str.push_back('a');
        REQUIRE_THAT(str.size(), EQ(1));
        REQUIRE_THAT(str[0], EQ('a'));
        REQUIRE_THAT(str[1], EQ('\0'));

        str.push_back('b');
        REQUIRE_THAT(str.size(), EQ(2));
        REQUIRE_THAT(str[0], EQ('a'));
        REQUIRE_THAT(str[1], EQ('b'));
        REQUIRE_THAT(str[2], EQ('\0'));

        str.pop_back();
        REQUIRE_THAT(str.size(), EQ(1));
        REQUIRE_THAT(str[0], EQ('a'));
        REQUIRE_THAT(str[1], EQ('\0'));
    };

    SECTION(test_append_count_ch) {
        ndash::string str(5, 'a');

        ndash::string copy;
        copy.append(5, 'a');

        REQUIRE_THAT(copy.size(), EQ(5));

        for (int i = 0; i <= 5; ++i) {
            REQUIRE_THAT(copy[i], EQ(str[i]));
        }

        copy.append(5, 'b');
        REQUIRE_THAT(copy.size(), EQ(10));

        for (int i = 0; i < 5; ++i) {
            REQUIRE_THAT(copy[i], EQ('a'));
        }
        for (int i = 5; i < 10; ++i) {
            REQUIRE_THAT(copy[i], EQ('b'));
        }
        REQUIRE_THAT(copy[10], EQ('\0'));
    };

    SECTION(test_append_cstr_count) {
        auto app = "_str bad";
        auto expected = "test_str";
        ndash::string str("test");

        REQUIRE_THAT(str.size(), EQ(4));

        str.append(app, 4);

        REQUIRE_THAT(str.size(), EQ(8));
        for (int i = 0; i < 9; ++i) {
            REQUIRE_THAT(str[i], EQ(expected[i]));
        }
    };

    SECTION(test_append_cstr) {
        auto app = "_str good";
        auto expected = "test_str good";
        ndash::string str("test");

        REQUIRE_THAT(str.size(), EQ(4));

        str.append(app);

        REQUIRE_THAT(str.size(), EQ(13));
        for (int i = 0; i < 14; ++i) {
            REQUIRE_THAT(str[i], EQ(expected[i]));
        }
    };

    SECTION(test_append_string) {
        auto expected0 = "test_str ";
        auto expected1 = "test_str good";
        auto expected2 = "test_str good great";

        ndash::string str("test");
        REQUIRE_THAT(str.size(), EQ(4));

        ndash::string full("_str ");
        str.append(full);

        REQUIRE_THAT(str.size(), EQ(9));
        for (int i = 0; i < 10; ++i) {
            REQUIRE_THAT(str[i], EQ(expected0[i]));
        }

        ndash::string partial("bad good great");
        str.append(partial, 4, 4);

        REQUIRE_THAT(str.size(), EQ(13));
        for (int i = 0; i < 14; ++i) {
            REQUIRE_THAT(str[i], EQ(expected1[i]));
        }

        ndash::string partial2("bad good great");
        str.append(partial, 8);

        REQUIRE_THAT(str.size(), EQ(19));
        for (int i = 0; i < 20; ++i) {
            REQUIRE_THAT(str[i], EQ(expected2[i]));
        }
    };

    SECTION(test_plus_equals) {
        auto expected0 = "test";
        auto expected1 = "test_";
        auto expected2 = "test_str ";
        auto expected3 = "test_str good";

        ndash::string str {};
        REQUIRE(str.empty());

        ndash::string test = "test";
        str += test;

        REQUIRE_THAT(str.size(), EQ(4));
        REQUIRE_THAT(str.data(), NEQ(test.data()));
        for (int i = 0; i <= 4; ++i) {
            REQUIRE_THAT(str[i], EQ(expected0[i]));
        }

        str += '_';
        REQUIRE_THAT(str.size(), EQ(5));
        for (int i = 0; i <= 5; ++i) {
            REQUIRE_THAT(str[i], EQ(expected1[i]));
        }

        str += "str ";
        REQUIRE_THAT(str.size(), EQ(9));
        for (int i = 0; i <= 9; ++i) {
            REQUIRE_THAT(str[i], EQ(expected2[i]));
        }

        str += { 'g', 'o', 'o', 'd' };
        REQUIRE_THAT(str.size(), EQ(13));
        for (int i = 0; i <= 13; ++i) {
            REQUIRE_THAT(str[i], EQ(expected3[i]));
        }
    };

    SECTION(test_find) {
        ndash::string str("the test tests the find operator");
        ndash::string test("test");

        REQUIRE_THAT(str.find("the"), EQ(0));
        REQUIRE_THAT(str.find("the", 0), EQ(0));
        REQUIRE_THAT(str.find("the", 1), EQ(15));

        REQUIRE_THAT(str.find("tests", 0), EQ(9));
        REQUIRE_THAT(str.find("tests", 0, 4), EQ(4));

        REQUIRE_THAT(str.find(test, 0), EQ(4));
        REQUIRE_THAT(str.find(test, 5), EQ(9));

        REQUIRE_THAT(str.find('t'), EQ(0));
        REQUIRE_THAT(str.find('t', 0), EQ(0));
        REQUIRE_THAT(str.find('t', 1), EQ(4));
    };

    SECTION(test_rfind) {
        ndash::string str("the test tests the find operator");
        ndash::string test("test");

        REQUIRE_THAT(str.rfind("the"), EQ(15));
        REQUIRE_THAT(str.rfind("the", 15), EQ(15));
        REQUIRE_THAT(str.rfind("the", 14), EQ(0));

        REQUIRE_THAT(str.rfind("tests"), EQ(9));
        REQUIRE_THAT(str.rfind("tests", ndash::string::npos, 4), EQ(9));
        REQUIRE_THAT(str.rfind("tests", 8, 4), EQ(4));

        REQUIRE_THAT(str.rfind(test), EQ(9));
        REQUIRE_THAT(str.rfind(test, 8), EQ(4));

        REQUIRE_THAT(str.rfind('t'), EQ(29));
        REQUIRE_THAT(str.rfind('t', 29), EQ(29));
        REQUIRE_THAT(str.rfind('t', 28), EQ(15));
    };

    SECTION(test_string_compare) {
        auto test1 = "And not";
        auto test2 = "nd";
        auto test3 = "znd";

        ndash::string str(test2);
        ndash::string str2(test3);

        REQUIRE_THAT(str.compare(test1), GT(0));
        REQUIRE_THAT(str.compare(test2), EQ(0));
        REQUIRE_THAT(str.compare(test3), LT(0));

        REQUIRE_THAT(str2.compare(1, 2, test1), GT(0));
        REQUIRE_THAT(str2.compare(1, 2, test2), EQ(0));
        REQUIRE_THAT(str2.compare(1, 2, test3 + 1), EQ(0));

        REQUIRE_THAT(str2.compare(1, 2, test1 + 1), EQ(0));
        REQUIRE_THAT(str2.compare(1, 2, test1 + 4), NEQ(0));
        REQUIRE_THAT(str2.compare(1, 2, test1 + 4, 1), EQ(0));
    };

    SECTION(test_string_swap) {
        auto expected = "test";
        ndash::string str(expected);
        ndash::string swapped;

        REQUIRE_THAT(str.size(), EQ(4));
        REQUIRE_THAT(swapped.size(), EQ(0));

        str.swap(swapped);

        REQUIRE_THAT(str.size(), EQ(0));
        REQUIRE_THAT(swapped.size(), EQ(4));
        REQUIRE_THAT(str.data(), NEQ(swapped.data()));

        for (int i = 0; i <= 4; ++i) {
            REQUIRE_THAT(swapped[i], EQ(expected[i]));
        }
    };

    SECTION(test_iterators) {
        auto expected = "test";
        ndash::string str(expected);

        REQUIRE_THAT(str.size(), EQ(4));

        auto index = 0;
        for (auto it = str.begin(); it != str.end(); ++it, ++index) {
            REQUIRE_THAT(*it, EQ(expected[index]));
        }

        REQUIRE_THAT(str.begin()[1], EQ(expected[1]));
        REQUIRE_THAT(*(str.begin() + 2), EQ(expected[2]));

        for (auto it = str.rbegin(); it != str.rend(); ++it) {
            REQUIRE_THAT(*it, EQ(expected[--index]));
        }

        REQUIRE_THAT(str.rbegin()[1], EQ(expected[2]));
        REQUIRE_THAT(*(str.rbegin() + 2), EQ(expected[1]));
    };

    SECTION(test_string_addition) {
        auto test1 = "test1";
        auto test2 = "test2";
        ndash::string str1(test1);
        ndash::string str2(test2);

        auto expected_str = "test1test2";

        REQUIRE_THAT(str1 + str2, EQ(expected_str));
        REQUIRE_THAT(str1 + test2, EQ(expected_str));
        REQUIRE_THAT(test1 + str2, EQ(expected_str));

        REQUIRE_THAT(str1 + 'a', EQ("test1a"));
        REQUIRE_THAT('a' + str1, EQ("atest1"));
    };

    SECTION(test_comparison_operators) {
        auto test1 = "test1";
        auto test2 = "test2";
        ndash::string str1(test1);
        ndash::string str2(test2);

        REQUIRE_THAT(str1, EQ(str1));
        REQUIRE_THAT(str1, NEQ(str2));
        REQUIRE_THAT(str1, LEQ(str2));
        REQUIRE_THAT(str1, LT(str2));
        REQUIRE_THAT(str2, GEQ(str1));
        REQUIRE_THAT(str2, GT(str1));

        REQUIRE_THAT(str1, EQ(test1));
        REQUIRE_THAT(str1, NEQ(test2));
        REQUIRE_THAT(str1, LEQ(test2));
        REQUIRE_THAT(str1, LT(test2));
        REQUIRE_THAT(str2, GEQ(test1));
        REQUIRE_THAT(str2, GT(test1));

        REQUIRE_THAT(test1, EQ(str1));
        REQUIRE_THAT(test1, NEQ(str2));
        REQUIRE_THAT(test1, LEQ(str2));
        REQUIRE_THAT(test1, LT(str2));
        REQUIRE_THAT(test2, GEQ(str1));
        REQUIRE_THAT(test2, GT(str1));
    };
}
