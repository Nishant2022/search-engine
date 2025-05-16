#ifndef SWAP_H
#define SWAP_H

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <type_traits>

namespace ndash {

// Swap two objects
template <class T>
void swap(T& a, T& b) {
    T temp(std::move(b));
    b = std::move(a);
    a = std::move(temp);
}

// Swap two arrays
template <class T, size_t N>
void swap(T (&a)[N], T (&b)[N]) {
    if constexpr (std::is_trivially_copyable_v<T>) {
        T temp[N];
        std::memcpy(temp, a, sizeof(a));
        std::memcpy(a, b, sizeof(b));
        std::memcpy(b, temp, sizeof(temp));
    } else {
        for (size_t i = 0; i < N; ++i) {
            swap(a[i], b[i]);
        }
    }
}

}   // namespace ndash

#endif   // SWAP_H
