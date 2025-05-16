#include "pair.h"
#include "test_framework.h"

TEST_CASE(Pair) {
    SECTION(test_pair_constructor) {
        ndash::pair<int, bool> p1;

        REQUIRE_THAT(p1.first, EQ(0));
        REQUIRE_THAT(p1.second, EQ(false));

        ndash::pair<int, double> p2 { 42, 3.1415 };

        REQUIRE_THAT(p2.first, EQ(42));
        REQUIRE_THAT(p2.second, EQ(3.1415));

        ndash::pair<char, int> p3(p2);
        REQUIRE_THAT(p3.first, EQ('*'));
        REQUIRE_THAT(p3.second, EQ(3));
    };

    SECTION(test_pair_assignment) {
        ndash::pair<int, double> p1 { 42, 3.1415 };
        ndash::pair<int, double> p2;
        ndash::pair<char, int> p3;


        p2 = p1;
        REQUIRE_THAT(p2.first, EQ(42));
        REQUIRE_THAT(p2.second, EQ(3.1415));

        p3 = p1;
        REQUIRE_THAT(p3.first, EQ('*'));
        REQUIRE_THAT(p3.second, EQ(3));
    };

    SECTION(test_pair_swap) {
        ndash::pair<int,double> p1{21, 2.718};
        ndash::pair<int,double> p2{25, 3.1415};

        REQUIRE_THAT(p1.first, EQ(21));
        REQUIRE_THAT(p1.second, EQ(2.718));
        REQUIRE_THAT(p2.first, EQ(25));
        REQUIRE_THAT(p2.second, EQ(3.1415));

        ndash::swap(p1, p2);
        REQUIRE_THAT(p2.first, EQ(21));
        REQUIRE_THAT(p2.second, EQ(2.718));
        REQUIRE_THAT(p1.first, EQ(25));
        REQUIRE_THAT(p1.second, EQ(3.1415));
    };
}
