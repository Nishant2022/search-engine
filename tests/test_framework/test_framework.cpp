#include "test_framework.h"

#include <ctime>
#include <iostream>

TestCase _cases[MAX_TESTS] {};
int _case_count = 0;
bool section_failed = false;

int main() {
    for (int i = 0; i < _case_count; ++i) {
        TestCase& test = _cases[i];
        std::cout << "TEST_CASE: " << _cases[i].name << "\n";
        _cases[i].func(i);
        for (int j = 0; j < test.num_sections; ++j) {
            TestSection& sec = test.sections[j];
            section_failed = false;
            std::cout << "    SECTION: " << sec.name << "\n";

            std::clock_t start = std::clock();
            try {
                sec.func();
            } catch (const std::exception& e) {
                section_failed = true;
                std::cerr << "    Exception: " << e.what() << "\n";
            } catch (...) {
                section_failed = true;
                std::cerr << "    Unknown exception occurred\n";
            }
            std::clock_t end = std::clock();
            double duration = 1000.0 * (end - start) / CLOCKS_PER_SEC;

            if (!section_failed)
                std::cout << "    PASSED in " << duration << " ms\n";
            else {
                std::cout << "    FAILED in " << duration << " ms\n";
                return -1;
            }
        }
        std::cout << std::endl;
    }
}
