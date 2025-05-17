#include <cstring>

#include "hash.h"
#include "test_framework.h"

TEST_CASE(Hash) {
    SECTION(test_hash_int) {
        int a = 0, b = 10, c = 100;
        auto int_hash = ndash::hash<int> {};

        auto hash_a = int_hash(a), hash_b = int_hash(b), hash_c = int_hash(c);

        REQUIRE_THAT(hash_a, NEQ(hash_b));
        REQUIRE_THAT(hash_a, NEQ(hash_c));
        REQUIRE_THAT(hash_b, NEQ(hash_c));

        REQUIRE_THAT(hash_a, EQ(int_hash(a)));
        REQUIRE_THAT(hash_b, EQ(int_hash(b)));
        REQUIRE_THAT(hash_c, EQ(int_hash(c)));
    };

    SECTION(test_hash_c_str) {
        char* a = new char[5];
        char* b = new char[5];

        memcpy(a, "test", 5);
        memcpy(b, "test", 5);

        auto pointer_hash = ndash::hash<char*> {};
        REQUIRE_THAT(pointer_hash(a, 5), NEQ(pointer_hash(b, 5)));

        delete[] a;
        delete[] b;
    };
}
