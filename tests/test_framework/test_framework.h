#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <cstring>
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


constexpr const int MAX_TESTS = 128;
constexpr const int MAX_SECTIONS_PER_TEST = 128;

struct TestSection {
    const char* name;
    void (*func)();
};

struct TestCase {
    const char* name;
    void (*func)(int);
    TestSection sections[MAX_SECTIONS_PER_TEST] {};
    int num_sections = 0;
};


extern TestCase _cases[];
extern int _case_count;
extern bool section_failed;

#define TEST_CASE(test_name)                                                                      \
    void test_case_##test_name(int test_index);                                                   \
    struct TestCaseRegister_##test_name {                                                         \
        TestCaseRegister_##test_name() {                                                          \
            _cases[_case_count].name = #test_name;                                                \
            _cases[_case_count].func = test_case_##test_name;                                     \
            _case_count++;                                                                        \
        }                                                                                         \
    };                                                                                            \
    __attribute__((used)) static TestCaseRegister_##test_name _test_case_register_##test_name {}; \
    void test_case_##test_name(int _test_index)

#define SECTION(sec_name)                                                                                \
    TestSection& _section_##sec_name = _cases[_test_index].sections[_cases[_test_index].num_sections++]; \
    _section_##sec_name.name = #sec_name;                                                                \
    _section_##sec_name.func = []()

#define REQUIRE(cond)                                                                                      \
    if (!(cond)) {                                                                                         \
        std::cerr << "    REQUIRE failed: " << #cond << " at " << __FILENAME__ << ":" << __LINE__ << "\n"; \
        section_failed = true;                                                                             \
        return;                                                                                            \
    }

#endif   // TEST_FRAMEWORK_H
