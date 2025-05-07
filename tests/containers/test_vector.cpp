#include "test_framework.h"

TEST_CASE(Vector) {
    SECTION(vector_section) {
        REQUIRE(true);
        REQUIRE(true);
    };
}

TEST_CASE(Vector2) {
    SECTION(vector2_section) {
        REQUIRE(true);
        REQUIRE(1 + 1 == 3);
    };
}
