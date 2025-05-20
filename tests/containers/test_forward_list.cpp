#include <cstddef>

#include "forward_list.h"
#include "test_framework.h"

TEST_CASE(Vector) {
    SECTION(test_empty_list) {
        ndash::forward_list<int> list {};

        REQUIRE(list.empty());
        REQUIRE_THAT(list.size(), EQ(0));
    };

    SECTION(test_count_constructor) {
        constexpr const int list_size = 4;
        ndash::forward_list<int> list(list_size);

        REQUIRE_THAT(list.size(), EQ(list_size));

        for (auto it = list.begin(); it != list.end(); ++it) {
            REQUIRE_THAT(*it, EQ(0));
        }
    };

    SECTION(test_count_constructor_with_value) {
        constexpr const size_t list_size = 4;
        ndash::forward_list<int> list(list_size, 10);

        REQUIRE_THAT(list.size(), EQ(list_size));

        for (auto it = list.begin(); it != list.end(); ++it) {
            REQUIRE_THAT(*it, EQ(10));
        }
    };

    SECTION(test_initializer_list_constructor) {
        ndash::forward_list<int> list({ 0, 1, 2, 3, 4 });

        REQUIRE_THAT(list.size(), EQ(5));

        int i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i) {
            REQUIRE_THAT(*it, EQ(i));
        }
    };

    SECTION(test_copy_constructors) {
        constexpr const int original_size = 10;

        ndash::forward_list<char> list(original_size, 'a');
        ndash::forward_list<char> copy1(list);

        REQUIRE_THAT(copy1.size(), EQ(original_size));

        list.front() = 'b';

        for (auto it = copy1.begin(); it != copy1.end(); ++it) {
            REQUIRE_THAT(*it, EQ('a'));
        }

        ndash::forward_list<char> copy2(std::move(copy1));

        REQUIRE_THAT(copy2.size(), EQ(original_size));

        for (auto it = copy2.begin(); it != copy2.end(); ++it) {
            REQUIRE_THAT(*it, EQ('a'));
        }

        REQUIRE_THAT(copy1.size(), EQ(0));
    };

    SECTION(test_assignment_operators) {
        constexpr const int original_size = 3;
        constexpr const int copy_size = 10;

        ndash::forward_list<char> list(original_size, 'a');
        ndash::forward_list<char> copy1(copy_size, 'b');

        REQUIRE_THAT(copy1.size(), EQ(copy_size));
        REQUIRE_THAT(copy1.size(), NEQ(list.size()));
        copy1 = list;
        REQUIRE_THAT(copy1.size(), EQ(list.size()));

        auto list_it = list.begin();
        auto copy1_it = copy1.begin();
        for (; list_it != list.end() && copy1_it != copy1.end(); ++list_it, ++copy1_it) {
            REQUIRE_THAT(*copy1_it, EQ(*list_it));
        }

        ndash::forward_list<char> copy2(copy_size, 'b');
        REQUIRE_THAT(copy2.size(), EQ(copy_size));
        REQUIRE_THAT(copy2.size(), NEQ(list.size()));
        copy2 = std::move(list);
        REQUIRE_THAT(copy2.size(), EQ(original_size));

        for (auto it = copy2.begin(); it != copy2.end(); ++it) {
            REQUIRE_THAT(*it, EQ('a'));
        }

        REQUIRE_THAT(list.size(), EQ(0));

        ndash::forward_list<char> copy3(copy_size, 'b');
        REQUIRE_THAT(copy3.size(), EQ(copy_size));
        REQUIRE_THAT(copy3.size(), NEQ(original_size));
        copy3 = { 'a', 'b', 'c' };
        REQUIRE_THAT(copy3.size(), EQ(original_size));

        int i = 0;
        for (auto it = copy3.begin(); it != copy3.end(); ++it, ++i) {
            REQUIRE_THAT(*it, EQ('a' + (char) i));
        }

        auto& copy4 = copy3;
        copy3 = copy4;
        REQUIRE_THAT(copy3.size(), EQ(original_size));

        i = 0;
        for (auto it = copy3.begin(); it != copy3.end(); ++it, ++i) {
            REQUIRE_THAT(*it, EQ('a' + (char) i));
        }
    };

    SECTION(test_push_pop_front) {
        ndash::forward_list<int> list {};

        auto it1 = list.push_front(1);
        REQUIRE(!list.empty());
        REQUIRE_THAT(list.size(), EQ(1));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(1));
        REQUIRE_THAT(&list.front(), EQ(&list.back()));
        REQUIRE_THAT(*it1, EQ(1));

        auto it2 = list.push_front(2);
        REQUIRE_THAT(list.size(), EQ(2));
        REQUIRE_THAT(list.front(), EQ(2));
        REQUIRE_THAT(list.back(), EQ(1));
        REQUIRE_THAT(&list.front(), NEQ(&list.back()));
        REQUIRE_THAT(*it2, EQ(2));
        REQUIRE_THAT(*(++it2), EQ(1));

        list.pop_front();
        REQUIRE_THAT(list.size(), EQ(1));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(1));
        REQUIRE_THAT(&list.front(), EQ(&list.back()));

        auto it3 = list.push_front(3);
        REQUIRE_THAT(list.size(), EQ(2));
        REQUIRE_THAT(list.front(), EQ(3));
        REQUIRE_THAT(list.back(), EQ(1));
        REQUIRE_THAT(&list.front(), NEQ(&list.back()));
        REQUIRE_THAT(*it3, EQ(3));
        REQUIRE_THAT(*(++it3), EQ(1));

        int i = 3;
        for (auto it = list.begin(); it != list.end(); ++it) {
            REQUIRE_THAT(*it, EQ(i));
            i -= 2;
        }
    };

    SECTION(test_push_back) {
        ndash::forward_list<int> list {};

        auto it1 = list.push_back(1);
        REQUIRE(!list.empty());
        REQUIRE_THAT(list.size(), EQ(1));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(1));
        REQUIRE_THAT(&list.front(), EQ(&list.back()));
        REQUIRE_THAT(*it1, EQ(1));
        REQUIRE(++it1 == list.end());

        auto it2 = list.push_back(2);
        REQUIRE_THAT(list.size(), EQ(2));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(2));
        REQUIRE_THAT(&list.front(), NEQ(&list.back()));
        REQUIRE_THAT(*it2, EQ(2));
        REQUIRE(++it2 == list.end());

        auto it3 = list.push_back(3);
        REQUIRE_THAT(list.size(), EQ(3));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(3));
        REQUIRE_THAT(&list.front(), NEQ(&list.back()));
        REQUIRE_THAT(*it3, EQ(3));
        REQUIRE(++it3 == list.end());
    };

    SECTION(test_clear_and_reserve) {
        ndash::forward_list<int> list(10);

        REQUIRE_THAT(list.size(), EQ(10));

        list.clear();

        REQUIRE_THAT(list.size(), EQ(0));
        REQUIRE(list.begin() == list.end());
    };

    SECTION(test_non_primitive) {
        struct non_primitive {
            int a;
            char b;
        };

        ndash::forward_list<non_primitive> list {};

        list.push_back({ 0, 'a' });
        list.emplace_back(1, 'b');

        REQUIRE_THAT(list.size(), EQ(2));
        int i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i) {
            REQUIRE_THAT(it->a, EQ(0 + i));
            REQUIRE_THAT(it->b, EQ('a' + i));
        }

        ndash::forward_list<non_primitive> copy(list);

        REQUIRE_THAT(copy.size(), EQ(2));
        i = 0;
        for (auto it = copy.begin(); it != copy.end(); ++it, ++i) {
            REQUIRE_THAT(it->a, EQ(0 + i));
            REQUIRE_THAT(it->b, EQ('a' + i));
        }
    };

    SECTION(test_emplace_after) {
        ndash::forward_list<int> list {};
        REQUIRE(++list.before_begin() == list.begin());

        list.emplace_after(list.before_begin(), 1);
        REQUIRE_THAT(list.size(), EQ(1));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(1));
        REQUIRE(++list.before_begin() == list.begin());

        list.emplace_back(3);
        REQUIRE_THAT(list.size(), EQ(2));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(3));

        list.emplace_after(list.begin(), 2);
        REQUIRE_THAT(list.size(), EQ(3));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(3));

        int count = 1;
        for (auto it = list.begin(); it != list.end(); ++it, ++count) {
            REQUIRE_THAT(*it, EQ(count));
        }
    };

    SECTION(test_erase_after) {
        ndash::forward_list<int> list {};

        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(4);
        list.push_back(5);

        REQUIRE_THAT(list.size(), EQ(5));
        REQUIRE_THAT(list.front(), EQ(1));
        REQUIRE_THAT(list.back(), EQ(5));

        list.erase_after(list.before_begin());
        REQUIRE_THAT(list.size(), EQ(4));
        REQUIRE_THAT(list.front(), EQ(2));
        REQUIRE_THAT(list.back(), EQ(5));

        list.erase_after(list.begin());
        REQUIRE_THAT(list.size(), EQ(3));
        REQUIRE_THAT(list.front(), EQ(2));
        REQUIRE_THAT(*(++list.begin()), EQ(4));

        list.erase_after(++list.begin());
        REQUIRE_THAT(list.size(), EQ(2));
        REQUIRE_THAT(list.front(), EQ(2));
        REQUIRE_THAT(*(++list.begin()), EQ(4));
        REQUIRE_THAT(list.back(), EQ(4));
    };
}
