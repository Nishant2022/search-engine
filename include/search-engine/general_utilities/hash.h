#ifndef HASH_H
#define HASH_H

#include <cstddef>

namespace ndash {

constexpr const size_t FNV_OFFSET_BASIS = 14695981039346656037u;
constexpr const size_t FNV_PRIME = 1099511628211u;

// Default hash function
template <class Key>
struct hash {
    constexpr inline size_t operator()(const Key& key, const size_t len = sizeof(key)) const {
        const char* bytes = reinterpret_cast<const char*>(&key);
        size_t hash = FNV_OFFSET_BASIS;

        for (auto i = 0u; i < len; ++i) {
            hash ^= bytes[i];
            hash *= FNV_PRIME;
        }
        return hash;
    }
};

}   // namespace ndash

#endif   // HASH_H
