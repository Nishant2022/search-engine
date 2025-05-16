#include "swap.h"
#include "test_framework.h"

TEST_CASE(Swap) {
    SECTION(test_swap) {
        int a = 0, b = 4;
        REQUIRE_THAT(a, EQ(0));
        REQUIRE_THAT(b, EQ(4));

        ndash::swap(a, b);
        REQUIRE_THAT(b, EQ(0));
        REQUIRE_THAT(a, EQ(4));
    };

    SECTION(test_swap_array) {
        char a[] { 1, 2, 3 };
        char b[] { 5, 6, 7 };

        for (int i = 0; i < 3; ++i) {
            REQUIRE_THAT(a[i], EQ(i + 1));
            REQUIRE_THAT(b[i], EQ(i + 5));
        }

        ndash::swap(a, b);

        for (int i = 0; i < 3; ++i) {
            REQUIRE_THAT(b[i], EQ(i + 1));
            REQUIRE_THAT(a[i], EQ(i + 5));
        }
    };
}
