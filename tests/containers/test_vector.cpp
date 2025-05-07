#include <cstddef>

#include "search-engine/containers/vector.h"
#include "test_framework.h"

TEST_CASE(Vector) {
    SECTION(test_empty_vector) {
        ndash::vector<int> vec {};

        REQUIRE(vec.empty());
        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 0);
        REQUIRE(vec.data() == nullptr);
    };

    SECTION(test_count_constructor) {
        constexpr const int vec_size = 4;
        ndash::vector<int> vec(vec_size);

        REQUIRE(vec.size() == vec_size);
        REQUIRE(vec.capacity() == vec_size);

        for (int i = 0; i < vec_size; ++i) {
            REQUIRE(vec[i] == 0);
        }
    };

    SECTION(test_count_constructor_with_value) {
        constexpr const size_t vec_size = 4;
        ndash::vector<int> vec(vec_size, 10);

        REQUIRE(vec.size() == vec_size);
        REQUIRE(vec.capacity() == vec_size);

        for (size_t i = 0; i < vec_size; ++i) {
            REQUIRE(vec[i] == 10);
        }
    };

    SECTION(test_initializer_list_constructor) {
        ndash::vector<int> vec({ 0, 1, 2, 3, 4 });

        REQUIRE(vec.size() == 5);
        REQUIRE(vec.capacity() == 5);

        for (size_t i = 0; i < vec.size(); ++i) {
            REQUIRE(vec[i] == (int) i);
        }
    };

    SECTION(test_copy_constructors) {
        constexpr const int original_size = 10;

        ndash::vector<char> vec(original_size, 'a');
        ndash::vector<char> copy1(vec);

        REQUIRE(copy1.size() == original_size);
        REQUIRE(copy1.capacity() == original_size);

        vec[0] = 'b';

        for (int i = 0; i < original_size; ++i) {
            REQUIRE(copy1[i] == 'a');
        }

        ndash::vector<char> copy2(std::move(copy1));

        REQUIRE(copy2.size() == original_size);
        REQUIRE(copy2.capacity() == original_size);

        for (int i = 0; i < original_size; ++i) {
            REQUIRE(copy2[i] == 'a');
        }

        REQUIRE(copy1.data() == nullptr);
    };

    SECTION(test_assignment_operators) {
        constexpr const int original_size = 3;
        constexpr const int copy_size = 10;

        ndash::vector<char> vec(original_size, 'a');
        ndash::vector<char> copy1(copy_size, 'b');

        REQUIRE(copy1.size() == copy_size);
        REQUIRE(copy1.size() != vec.size());
        copy1 = vec;
        REQUIRE(copy1.size() == vec.size());
        REQUIRE(copy1.data() != vec.data());

        for (auto i = 0u; i < copy1.size(); ++i) {
            REQUIRE(copy1[i] == vec[i]);
        }

        ndash::vector<char> copy2(copy_size, 'b');
        REQUIRE(copy2.size() == copy_size);
        REQUIRE(copy2.size() != vec.size());
        copy2 = std::move(vec);
        REQUIRE(copy2.size() == original_size);
        REQUIRE(copy2.data() != vec.data());

        for (auto i = 0u; i < copy2.size(); ++i) {
            REQUIRE(copy2[i] == 'a');
        }

        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 0);
        REQUIRE(vec.data() == nullptr);

        ndash::vector<char> copy3(copy_size, 'b');
        REQUIRE(copy3.size() == copy_size);
        REQUIRE(copy3.size() != original_size);
        copy3 = { 'a', 'b', 'c' };
        REQUIRE(copy3.size() == original_size);

        for (auto i = 0u; i < copy3.size(); ++i) {
            REQUIRE(copy3[i] == 'a' + (char) i);
        }

        auto& copy4 = copy3;
        copy3 = copy4;

        REQUIRE(copy3.size() == original_size);
        for (auto i = 0u; i < copy3.size(); ++i) {
            REQUIRE(copy3[i] == 'a' + (char) i);
        }
    };

    SECTION(test_push_back_and_pop_back) {
        ndash::vector<int> vec {};

        vec.push_back(1);
        REQUIRE(!vec.empty());
        REQUIRE(vec.size() == 1);
        REQUIRE(vec.capacity() == 8);
        REQUIRE(vec.data() != nullptr);

        REQUIRE(vec.front() == 1);
        REQUIRE(vec.back() == 1);
        REQUIRE(&vec.front() == &vec.back());

        vec.push_back(2);
        REQUIRE(vec.size() == 2);
        REQUIRE(vec.capacity() == 8);

        REQUIRE(vec.front() == 1);
        REQUIRE(vec.back() == 2);
        REQUIRE(&vec.front() != &vec.back());

        vec.pop_back();
        REQUIRE(vec.size() == 1);
        REQUIRE(vec.capacity() == 8);
        REQUIRE(vec.data() != nullptr);

        REQUIRE(vec.front() == 1);
        REQUIRE(vec.back() == 1);
        REQUIRE(&vec.front() == &vec.back());
    };

    SECTION(test_resizes) {
        ndash::vector<int> vec {};
        for (size_t i = 0; i < 8; ++i) {
            vec.push_back(i);

            REQUIRE(vec.size() == i + 1);
            REQUIRE(vec.capacity() == 8);
            REQUIRE(vec.front() == 0);
            REQUIRE(vec.back() == (int) i);
        }

        vec.push_back(8);

        REQUIRE(vec.size() == 9);
        REQUIRE(vec.capacity() == 16);
        REQUIRE(vec.front() == 0);
        REQUIRE(vec.back() == 8);
    };

    SECTION(test_clear_and_reserve) {
        ndash::vector<int> vec(10);

        REQUIRE(vec.size() == 10);
        REQUIRE(vec.capacity() == 10);

        vec.clear();

        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 10);

        vec.reserve(5);

        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 10);

        vec.reserve(50);

        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 50);
    };

    SECTION(test_non_primitive) {
        struct non_primitive {
            int a;
            char b;
        };

        ndash::vector<non_primitive> vec {};

        vec.push_back({ 0, 'a' });
        vec.emplace_back(1, 'b');

        REQUIRE(vec.size() == 2);
        REQUIRE(vec.capacity() == 8);
        for (int i = 0; i < 2; ++i) {
            REQUIRE(vec[i].a == 0 + i);
            REQUIRE(vec[i].b == 'a' + i);
        }

        ndash::vector<non_primitive> copy(vec);

        REQUIRE(copy.size() == 2);
        REQUIRE(copy.capacity() == 8);
        for (int i = 0; i < 2; ++i) {
            REQUIRE(copy[i].a == 0 + i);
            REQUIRE(copy[i].b == 'a' + i);
        }
    };

    SECTION(test_iterators) {
        ndash::vector<int> vec = { 0, 1, 2, 3, 4 };
        int i = 0;
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            REQUIRE(*it == i++);
        }

        for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
            REQUIRE(*it == --i);
        }

        REQUIRE(*(vec.begin() + 3) == 3);
        REQUIRE(*(vec.rbegin() + 3) == 1);

        REQUIRE(*(vec.end() - 1) == vec.back());
        REQUIRE(*(vec.rend() - 1) == vec.front());
    };
}
