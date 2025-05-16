#ifndef PAIR_H
#define PAIR_H

#include <utility>

#include "swap.h"

namespace ndash {

// Pair struct
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////// Constructors/Destructors ///////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Default constructs first and second
    constexpr pair()
        : first()
        , second() {}

    // Constructs first with x and second with y
    constexpr pair(const T1& x, const T2& y)
        : first(x)
        , second(y) {}

    // Construct pair from other values
    template <class U1, class U2>
    constexpr pair(const pair<U1, U2>& p)
        : first(p.first)
        , second(p.second) {}

    // Construct pair from other values with move semantics
    template <class U1, class U2>
    constexpr pair(const pair<U1, U2>&& p)
        : first(std::move(p.first))
        , second(std::move(p.second)) {}

    constexpr pair(const pair& p) = default;
    constexpr pair(pair&& p) = default;


    // Assignment operator
    constexpr pair& operator=(const pair& other) {
        if (&other != this) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    // Move assignment operator
    constexpr pair& operator=(pair&& other) {
        if (&other != this) {
            first = std::move(other.first);
            second = std::move(other.second);
        }
        return *this;
    }

    // Assignment operator with different pair
    template <class U1, class U2>
    constexpr pair& operator=(const pair<U1, U2>& other) {
        if ((void*) &other != (void*) this) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    // Move assignment operator with different pair
    template <class U1, class U2>
    constexpr pair& operator=(pair<U1, U2>&& other) {
        if ((void*) &other != (void*) this) {
            first = std::move(other.first);
            second = std::move(other.second);
        }
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Modifiers ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Swap two pairs
    void swap(pair& other) {
        ndash::swap(first, other.first);
        ndash::swap(second, other.second);
    }

    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////// Non-member functions //////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Swap specialization
    friend void swap(pair& x, pair& y) { x.swap(y); }
};

}   // namespace ndash

#endif   // PAIR_H
