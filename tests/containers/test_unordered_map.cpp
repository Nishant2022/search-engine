#include "ndstring.h"
#include "pair.h"
#include "test_framework.h"
#include "unordered_map.h"
#include "vector.h"

TEST_CASE(UnorderedMap) {
    SECTION(test_empty_map) {
        ndash::unordered_map<int, int> map {};

        REQUIRE(map.empty());
        REQUIRE_THAT(map.size(), EQ(0));
    };

    SECTION(test_bucket_count_constructor) {
        ndash::unordered_map<int, int> map(1);

        REQUIRE(map.empty());
        REQUIRE_THAT(map.size(), EQ(0));

        map.try_emplace(1, 1);
        REQUIRE_THAT(map.size(), EQ(1));
        REQUIRE_THAT(map.load_factor(), EQ(1.0));

        map.try_emplace(2, 2);
        REQUIRE_THAT(map.size(), EQ(2));
        REQUIRE_THAT(map.load_factor(), EQ(2.0));

        map.try_emplace(3, 3);
        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE_THAT(map.load_factor(), LEQ(2.0));
    };

    SECTION(test_iterator_constructor) {
        ndash::vector<ndash::pair<const int, int>> vals = {
            { 1, 1 },
            { 2, 2 },
            { 3, 3 }
        };

        ndash::unordered_map<int, int> map(vals.begin(), vals.end());
        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(2));
        REQUIRE(map.contains(3));
    };

    SECTION(test_ilist_constructor) {
        ndash::unordered_map<int, int> map = {
            { 1, 1 },
            { 2, 2 },
            { 3, 3 }
        };

        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(2));
        REQUIRE(map.contains(3));
    };

    SECTION(test_copy_constructors) {
        ndash::unordered_map<int, int> map = {
            { 1, 1 },
            { 2, 2 },
            { 3, 3 }
        };

        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(2));
        REQUIRE(map.contains(3));

        ndash::unordered_map<int, int> map2(map);

        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(2));
        REQUIRE(map.contains(3));

        REQUIRE_THAT(map2.size(), EQ(3));
        REQUIRE(map2.contains(1));
        REQUIRE(map2.contains(2));
        REQUIRE(map2.contains(3));

        ndash::unordered_map<int, int> map3(std::move(map));

        REQUIRE_THAT(map.size(), NEQ(3));

        REQUIRE_THAT(map3.size(), EQ(3));
        REQUIRE(map3.contains(1));
        REQUIRE(map3.contains(2));
        REQUIRE(map3.contains(3));
    };

    SECTION(test_assignment_operators) {
        ndash::unordered_map<int, int> map = {
            { 1, 1 },
            { 2, 2 },
            { 3, 3 }
        };
        ndash::unordered_map<int, int> map2;
        ndash::unordered_map<int, int> map3;

        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(2));
        REQUIRE(map.contains(3));

        REQUIRE_THAT(map2.size(), NEQ(3));
        REQUIRE(!map2.contains(1));
        REQUIRE(!map2.contains(2));
        REQUIRE(!map2.contains(3));

        REQUIRE_THAT(map3.size(), NEQ(3));
        REQUIRE(!map3.contains(1));
        REQUIRE(!map3.contains(2));
        REQUIRE(!map3.contains(3));

        map2 = map;

        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(2));
        REQUIRE(map.contains(3));

        REQUIRE_THAT(map2.size(), EQ(3));
        REQUIRE(map2.contains(1));
        REQUIRE(map2.contains(2));
        REQUIRE(map2.contains(3));

        map3 = std::move(map);

        REQUIRE_THAT(map.size(), NEQ(3));

        REQUIRE_THAT(map3.size(), EQ(3));
        REQUIRE(map3.contains(1));
        REQUIRE(map3.contains(2));
        REQUIRE(map3.contains(3));

        map2 = {
            { 4, 4 },
            { 5, 5 },
            { 6, 6 },
            { 7, 7 },
            { 8, 8 }
        };
        REQUIRE_THAT(map2.size(), EQ(5));
        REQUIRE(!map2.contains(1));
        REQUIRE(!map2.contains(2));
        REQUIRE(!map2.contains(3));
        REQUIRE(map2.contains(4));
        REQUIRE(map2.contains(5));
        REQUIRE(map2.contains(6));
        REQUIRE(map2.contains(7));
        REQUIRE(map2.contains(8));
    };

    SECTION(test_max_load_factor) {
        ndash::unordered_map<int, int> map {};

        REQUIRE_THAT(map.max_load_factor(), EQ(2.0));

        map.max_load_factor(4.0);

        REQUIRE_THAT(map.max_load_factor(), EQ(4.0));
    };

    SECTION(test_access_operator) {
        ndash::unordered_map<ndash::string, int> map {};

        map["test1"] = 1;
        map["test2"] = 2;

        REQUIRE_THAT(map.size(), EQ(2));
        REQUIRE_THAT(map["test1"], EQ(1));
        REQUIRE_THAT(map["test2"], EQ(2));

        map["test1"] = 3;
        map["test3"] = 4;
        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE_THAT(map["test1"], EQ(3));
        REQUIRE_THAT(map["test2"], EQ(2));
        REQUIRE_THAT(map["test3"], EQ(4));
    };

    SECTION(test_emplace) {
        ndash::unordered_map<ndash::string, int> map {};

        ndash::string test1("test1");
        ndash::string test2("test2");

        auto it = map.emplace(test1, 1);
        REQUIRE(it.second);
        REQUIRE_THAT(it.first->first, EQ(test1));
        REQUIRE_THAT(it.first->second, EQ(1));

        auto it2 = map.emplace(test2, 2);
        REQUIRE(it2.second);
        REQUIRE_THAT(it2.first->first, EQ(test2));
        REQUIRE_THAT(it2.first->second, EQ(2));

        REQUIRE_THAT(map.size(), EQ(2));
        REQUIRE_THAT(map[test1], EQ(1));
        REQUIRE_THAT(map[test2], EQ(2));

        auto it3 = map.emplace(test1, 10);
        REQUIRE(!it3.second);
        REQUIRE_THAT(it3.first->first, EQ(test1));
        REQUIRE_THAT(it3.first->second, EQ(1));

        REQUIRE_THAT(map.size(), EQ(2));
        REQUIRE_THAT(map[test1], EQ(1));
        REQUIRE_THAT(map[test2], EQ(2));
    };

    SECTION(test_iterators) {
        ndash::unordered_map<ndash::string, int> map {};

        map["test1"] = 1;
        map["test2"] = 2;
        map["test3"] = 3;
        map["test4"] = 4;

        int count = 10;
        for (auto it = map.begin(); it != map.end(); ++it) {
            REQUIRE_THAT(it->first, STARTS_WITH("test"));
            REQUIRE_THAT(it->second, GT(0));
            count -= it->second;
        }
        REQUIRE_THAT(count, EQ(0));
    };

    SECTION(test_clear) {
        ndash::unordered_map<ndash::string, int> map {};

        map["test1"] = 1;
        map["test2"] = 2;
        map["test3"] = 3;
        map["test4"] = 4;

        REQUIRE(!map.empty());
        REQUIRE_THAT(map.size(), EQ(4));
        REQUIRE(map.begin() != map.end());

        map.clear();
        REQUIRE(map.empty());
        REQUIRE_THAT(map.size(), EQ(0));
        REQUIRE(map.begin() == map.end());
    };

    SECTION(test_erase) {
        ndash::unordered_map<ndash::string, int> map {};

        map["test1"] = 1;
        map["test2"] = 2;

        REQUIRE_THAT(map.size(), EQ(2));
        REQUIRE(map.find("test1") != map.end());
        REQUIRE(map.find("test2") != map.end());

        bool result;
        result = map.erase("test3");
        REQUIRE(!result);
        REQUIRE_THAT(map.size(), EQ(2));
        REQUIRE(map.find("test1") != map.end());
        REQUIRE(map.find("test2") != map.end());

        result = map.erase("test2");
        REQUIRE(result);
        REQUIRE_THAT(map.size(), EQ(1));
        REQUIRE(map.find("test1") != map.end());
        REQUIRE(map.find("test2") == map.end());

        result = map.erase("test2");
        REQUIRE(!result);
        REQUIRE_THAT(map.size(), EQ(1));
        REQUIRE(map.find("test1") != map.end());
        REQUIRE(map.find("test2") == map.end());
    };

    SECTION(test_swap) {
        ndash::unordered_map<ndash::string, int> map {};
        ndash::unordered_map<ndash::string, int> map2 {};

        map["test1"] = 1;
        map["test2"] = 2;
        map["test3"] = 3;
        map["test4"] = 4;

        int count = 0;
        for (auto it = map.begin(); it != map.end(); ++it) {
            count += it->second;
        }
        REQUIRE_THAT(count, EQ(10));

        count = 0;
        for (auto it = map2.begin(); it != map2.end(); ++it) {
            count += it->second;
        }
        REQUIRE_THAT(count, EQ(0));

        REQUIRE_THAT(map.size(), EQ(4));
        REQUIRE(map2.empty());
        REQUIRE(map2.begin() == map2.end());

        ndash::swap(map, map2);

        REQUIRE_THAT(map2.size(), EQ(4));
        REQUIRE(map.empty());
        REQUIRE(map.begin() == map2.end());

        count = 0;
        for (auto it = map.begin(); it != map.end(); ++it) {
            count += it->second;
        }
        REQUIRE_THAT(count, EQ(0));

        count = 0;
        for (auto it = map2.begin(); it != map2.end(); ++it) {
            count += it->second;
        }
        REQUIRE_THAT(count, EQ(10));
    };

    SECTION(test_find) {
        ndash::unordered_map<ndash::string, int> map {};

        map.try_emplace("test1", 1);
        map.try_emplace("test1", 2);
        map.try_emplace("test2", 2);
        map.try_emplace("test2", 3);
        map.try_emplace("test3", 3);
        map.try_emplace("test3", 4);

        REQUIRE_THAT(map.size(), EQ(3));
        REQUIRE(map.find("test1") != map.end());
        REQUIRE(map.find("test2") != map.end());
        REQUIRE(map.find("test3") != map.end());
        REQUIRE(map.find("test4") == map.end());
        REQUIRE(map.find("test5") == map.end());
    };

    SECTION(test_custom_hash) {
        struct custom_hash {
            constexpr inline size_t operator()(const int& key) const { return key; }
        };

        ndash::unordered_map<int, int, custom_hash> map {
            { 1, 1 },
            { 2, 2 }
        };

        REQUIRE_THAT(map.size(), EQ(2));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(2));

        REQUIRE_THAT(map.bucket(1), EQ(1));
        REQUIRE_THAT(map.bucket(2), EQ(2));
        REQUIRE_THAT(map.bucket(map.bucket_count()), EQ(0));
        REQUIRE_THAT(map.bucket(2 * map.bucket_count() + 1), EQ(1));
    };
}
