#include <cstddef>

#include "pair.h"
#include "test_framework.h"
#include "vector.h"

TEST_CASE(Vector) {
    SECTION(test_empty_vector) {
        ndash::vector<int> vec {};

        REQUIRE(vec.empty());
        REQUIRE_THAT(vec.size(), EQ(0));
        REQUIRE_THAT(vec.capacity(), EQ(0));
        REQUIRE_THAT(vec.data(), EQ(nullptr));
    };

    SECTION(test_count_constructor) {
        constexpr const int vec_size = 4;
        ndash::vector<int> vec(vec_size);

        REQUIRE_THAT(vec.size(), EQ(vec_size));
        REQUIRE_THAT(vec.capacity(), EQ(vec_size));

        for (int i = 0; i < vec_size; ++i) {
            REQUIRE_THAT(vec[i], EQ(0));
        }
    };

    SECTION(test_count_constructor_with_value) {
        constexpr const size_t vec_size = 4;
        ndash::vector<int> vec(vec_size, 10);

        REQUIRE_THAT(vec.size(), EQ(vec_size));
        REQUIRE_THAT(vec.capacity(), EQ(vec_size));

        for (size_t i = 0; i < vec_size; ++i) {
            REQUIRE_THAT(vec[i], EQ(10));
        }
    };

    SECTION(test_initializer_list_constructor) {
        ndash::vector<int> vec({ 0, 1, 2, 3, 4 });

        REQUIRE_THAT(vec.size(), EQ(5));
        REQUIRE_THAT(vec.capacity(), EQ(5));

        for (size_t i = 0; i < vec.size(); ++i) {
            REQUIRE_THAT(vec[i], EQ((int) i));
        }
    };

    SECTION(test_copy_constructors) {
        constexpr const int original_size = 10;

        ndash::vector<char> vec(original_size, 'a');
        ndash::vector<char> copy1(vec);

        REQUIRE_THAT(copy1.size(), EQ(original_size));
        REQUIRE_THAT(copy1.capacity(), EQ(original_size));

        vec[0] = 'b';

        for (int i = 0; i < original_size; ++i) {
            REQUIRE_THAT(copy1[i], EQ('a'));
        }

        ndash::vector<char> copy2(std::move(copy1));

        REQUIRE_THAT(copy2.size(), EQ(original_size));
        REQUIRE_THAT(copy2.capacity(), EQ(original_size));

        for (int i = 0; i < original_size; ++i) {
            REQUIRE_THAT(copy2[i], EQ('a'));
        }

        REQUIRE_THAT(copy1.data(), EQ(nullptr));
    };

    SECTION(test_assignment_operators) {
        constexpr const int original_size = 3;
        constexpr const int copy_size = 10;

        ndash::vector<char> vec(original_size, 'a');
        ndash::vector<char> copy1(copy_size, 'b');

        REQUIRE_THAT(copy1.size(), EQ(copy_size));
        REQUIRE_THAT(copy1.size(), NEQ(vec.size()));
        copy1 = vec;
        REQUIRE_THAT(copy1.size(), EQ(vec.size()));
        REQUIRE_THAT(copy1.data(), NEQ(vec.data()));

        for (auto i = 0u; i < copy1.size(); ++i) {
            REQUIRE_THAT(copy1[i], EQ(vec[i]));
        }

        ndash::vector<char> copy2(copy_size, 'b');
        REQUIRE_THAT(copy2.size(), EQ(copy_size));
        REQUIRE_THAT(copy2.size(), NEQ(vec.size()));
        copy2 = std::move(vec);
        REQUIRE_THAT(copy2.size(), EQ(original_size));
        REQUIRE_THAT(copy2.data(), NEQ(vec.data()));

        for (auto i = 0u; i < copy2.size(); ++i) {
            REQUIRE_THAT(copy2[i], EQ('a'));
        }

        REQUIRE_THAT(vec.size(), EQ(0));
        REQUIRE_THAT(vec.capacity(), EQ(0));
        REQUIRE_THAT(vec.data(), EQ(nullptr));

        ndash::vector<char> copy3(copy_size, 'b');
        REQUIRE_THAT(copy3.size(), EQ(copy_size));
        REQUIRE_THAT(copy3.size(), NEQ(original_size));
        copy3 = { 'a', 'b', 'c' };
        REQUIRE_THAT(copy3.size(), EQ(original_size));

        for (auto i = 0u; i < copy3.size(); ++i) {
            REQUIRE_THAT(copy3[i], EQ('a' + (char) i));
        }

        auto& copy4 = copy3;
        copy3 = copy4;

        REQUIRE_THAT(copy3.size(), EQ(original_size));
        for (auto i = 0u; i < copy3.size(); ++i) {
            REQUIRE_THAT(copy3[i], EQ('a' + (char) i));
        }
    };

    SECTION(test_push_back_and_pop_back) {
        ndash::vector<int> vec {};

        vec.push_back(1);
        REQUIRE(!vec.empty());
        REQUIRE_THAT(vec.size(), EQ(1));
        REQUIRE_THAT(vec.capacity(), EQ(8));
        REQUIRE_THAT(vec.data(), NEQ(nullptr));

        REQUIRE_THAT(vec.front(), EQ(1));
        REQUIRE_THAT(vec.back(), EQ(1));
        REQUIRE_THAT(&vec.front(), EQ(&vec.back()));

        vec.push_back(2);
        REQUIRE_THAT(vec.size(), EQ(2));
        REQUIRE_THAT(vec.capacity(), EQ(8));

        REQUIRE_THAT(vec.front(), EQ(1));
        REQUIRE_THAT(vec.back(), EQ(2));
        REQUIRE_THAT(&vec.front(), NEQ(&vec.back()));

        vec.pop_back();
        REQUIRE_THAT(vec.size(), EQ(1));
        REQUIRE_THAT(vec.capacity(), EQ(8));
        REQUIRE_THAT(vec.data(), NEQ(nullptr));

        REQUIRE_THAT(vec.front(), EQ(1));
        REQUIRE_THAT(vec.back(), EQ(1));
        REQUIRE_THAT(&vec.front(), EQ(&vec.back()));
    };

    SECTION(test_resizes) {
        ndash::vector<int> vec {};
        for (size_t i = 0; i < 8; ++i) {
            vec.push_back(i);

            REQUIRE_THAT(vec.size(), EQ(i + 1));
            REQUIRE_THAT(vec.capacity(), EQ(8));
            REQUIRE_THAT(vec.front(), EQ(0));
            REQUIRE_THAT(vec.back(), EQ((int) i));
        }

        vec.push_back(8);

        REQUIRE_THAT(vec.size(), EQ(9));
        REQUIRE_THAT(vec.capacity(), EQ(16));
        REQUIRE_THAT(vec.front(), EQ(0));
        REQUIRE_THAT(vec.back(), EQ(8));
    };

    SECTION(test_clear_and_reserve) {
        ndash::vector<int> vec(10);

        REQUIRE_THAT(vec.size(), EQ(10));
        REQUIRE_THAT(vec.capacity(), EQ(10));

        vec.clear();

        REQUIRE_THAT(vec.size(), EQ(0));
        REQUIRE_THAT(vec.capacity(), EQ(10));
        REQUIRE(vec.begin() == vec.end());

        vec.reserve(5);

        REQUIRE_THAT(vec.size(), EQ(0));
        REQUIRE_THAT(vec.capacity(), EQ(10));

        vec.reserve(50);

        REQUIRE_THAT(vec.size(), EQ(0));
        REQUIRE_THAT(vec.capacity(), EQ(50));
    };

    SECTION(test_non_primitive) {
        struct non_primitive {
            int a;
            char b;
        };

        ndash::vector<non_primitive> vec {};

        vec.push_back({ 0, 'a' });
        vec.emplace_back(1, 'b');

        REQUIRE_THAT(vec.size(), EQ(2));
        REQUIRE_THAT(vec.capacity(), EQ(8));
        for (int i = 0; i < 2; ++i) {
            REQUIRE_THAT(vec[i].a, EQ(0 + i));
            REQUIRE_THAT(vec[i].b, EQ('a' + i));
        }

        ndash::vector<non_primitive> copy(vec);

        REQUIRE_THAT(copy.size(), EQ(2));
        REQUIRE_THAT(copy.capacity(), EQ(8));
        for (int i = 0; i < 2; ++i) {
            REQUIRE_THAT(copy[i].a, EQ(0 + i));
            REQUIRE_THAT(copy[i].b, EQ('a' + i));
        }
    };

    SECTION(test_iterators) {
        ndash::vector<int> vec = { 0, 1, 2, 3, 4 };
        int i = 0;
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            REQUIRE_THAT(*it, EQ(i++));
        }

        for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
            REQUIRE_THAT(*it, EQ(--i));
        }

        REQUIRE_THAT(*(vec.begin() + 3), EQ(3));
        REQUIRE_THAT(*(vec.rbegin() + 3), EQ(1));

        REQUIRE_THAT(*(vec.end() - 1), EQ(vec.back()));
        REQUIRE_THAT(*(vec.rend() - 1), EQ(vec.front()));
    };

    SECTION(test_resize) {
        ndash::vector<int> vec(5);
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            *it = 10;
        }

        REQUIRE_THAT(vec.size(), EQ(5));
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            REQUIRE_THAT(*it, EQ(10));
        }

        vec.resize(10);
        REQUIRE_THAT(vec.size(), EQ(10));
        for (auto it = vec.begin(); it != vec.begin() + 5; ++it) {
            REQUIRE_THAT(*it, EQ(10));
        }
        for (auto it = vec.begin() + 5; it != vec.end(); ++it) {
            REQUIRE_THAT(*it, EQ(0));
        }

        vec.resize(20, 100);
        REQUIRE_THAT(vec.size(), EQ(20));
        for (auto it = vec.begin(); it != vec.begin() + 5; ++it) {
            REQUIRE_THAT(*it, EQ(10));
        }
        for (auto it = vec.begin() + 5; it != vec.begin() + 10; ++it) {
            REQUIRE_THAT(*it, EQ(0));
        }
        for (auto it = vec.begin() + 10; it != vec.end(); ++it) {
            REQUIRE_THAT(*it, EQ(100));
        }

        vec.resize(3);
        REQUIRE_THAT(vec.size(), EQ(3));
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            REQUIRE_THAT(*it, EQ(10));
        }

        vec.resize(5);
        REQUIRE_THAT(vec.size(), EQ(5));
        for (auto it = vec.begin(); it != vec.begin() + 3; ++it) {
            REQUIRE_THAT(*it, EQ(10));
        }
        for (auto it = vec.begin() + 3; it != vec.end(); ++it) {
            REQUIRE_THAT(*it, EQ(0));
        }
    };

    SECTION(test_vector_of_pairs) {
        ndash::vector<ndash::pair<int, int>> vec = {
            { 1, 4 },
            { 2, 5 },
            { 3, 6 }
        };

        int index = 1;
        for (auto it = vec.begin(); it != vec.end(); ++it, ++index) {
            REQUIRE_THAT(it->first, EQ(index));
            REQUIRE_THAT(it->second, EQ(index + 3));
        }
    };
}
