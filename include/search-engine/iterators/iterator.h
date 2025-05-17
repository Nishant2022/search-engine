#ifndef ITERATOR_H
#define ITERATOR_H

#include <concepts>
#include <cstddef>

namespace ndash {

///////////////////////////////////////////////////////////////////////////////
////////////////////////////// Iterator Concepts //////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <typename It>
concept input_iterator = requires(It it) {
    { *it } -> std::convertible_to<typename It::reference>;
    { it.operator->() } -> std::convertible_to<typename It::pointer>;
    { it == it } -> std::convertible_to<bool>;
    { it != it } -> std::convertible_to<bool>;
};

template <typename It>
concept forward_iterator = input_iterator<It> && requires(It it) {
    { ++it } -> std::same_as<It&>;
    { it++ } -> std::same_as<It&>;
};

template <typename It>
concept bidirectional_iterator = forward_iterator<It> && requires(It it) {
    { --it } -> std::same_as<It&>;
    { it-- } -> std::same_as<It&>;
};

template <typename It>
concept random_access_iterator = bidirectional_iterator<It> && requires(It it, It it2, ptrdiff_t n) {
    { it + n } -> std::same_as<It>;
    { it - n } -> std::same_as<It>;
    { it - it2 } -> std::same_as<typename It::difference_type>;
    { it += n } -> std::same_as<It&>;
    { it -= n } -> std::same_as<It&>;
    { it[n] } -> std::same_as<typename It::reference>;
    { it <=> it2 } -> std::same_as<typename It::difference_type>;
};

///////////////////////////////////////////////////////////////////////////////
////////////////////////////// Reverse Iterator ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <bidirectional_iterator It>
class reverse_iterator {
public:
    using value_type = typename It::value;
    using reference = typename It::reference;
    using pointer = typename It::pointer;
    using difference_type = typename It::difference_type;

    constexpr reverse_iterator(const It& base)
        : _base(base) {}

    constexpr reference operator*() const {
        auto temp = _base;
        return *(--temp);
    }
    constexpr pointer operator->() const {
        auto temp = _base;
        return &(*(--temp));
    }

    constexpr reverse_iterator& operator++() {
        --_base;
        return *this;
    }

    constexpr reverse_iterator& operator++(int) {
        reverse_iterator tmp = *this;
        --_base;
        return tmp;
    }

    constexpr reverse_iterator& operator--() {
        ++_base;
        return *this;
    }

    constexpr reverse_iterator& operator--(int) {
        reverse_iterator tmp = *this;
        ++_base;
        return tmp;
    }

    constexpr reverse_iterator& operator+=(ptrdiff_t diff)
    requires random_access_iterator<It>
    {
        _base -= diff;
        return *this;
    }

    constexpr reverse_iterator& operator-=(ptrdiff_t diff)
    requires random_access_iterator<It>
    {
        _base += diff;
        return *this;
    }

    constexpr reference operator[](ptrdiff_t diff)
    requires random_access_iterator<It>
    {
        return _base[-diff - 1];
    }
    constexpr const reference operator[](ptrdiff_t diff) const
    requires random_access_iterator<It>
    {
        return _base[-diff - 1];
    }

    friend constexpr reverse_iterator operator+(const reverse_iterator& a, ptrdiff_t diff)
    requires random_access_iterator<It>
    {
        return reverse_iterator(a._base - diff);
    }

    friend constexpr reverse_iterator operator+(ptrdiff_t diff, const reverse_iterator& a)
    requires random_access_iterator<It>
    {
        return a - diff;
    }

    friend constexpr reverse_iterator operator-(const reverse_iterator& a, ptrdiff_t diff)
    requires random_access_iterator<It>
    {
        return reverse_iterator(a._base + diff);
    }

    friend constexpr ptrdiff_t operator-(const reverse_iterator& a, const reverse_iterator& b)
    requires random_access_iterator<It>
    {
        return b._base - a._base;
    }

    friend bool constexpr operator==(const reverse_iterator& a, const reverse_iterator& b)
    requires random_access_iterator<It>
    {
        return a._base == b._base;
    }

    friend bool constexpr operator!=(const reverse_iterator& a, const reverse_iterator& b)
    requires random_access_iterator<It>
    {
        return !(a == b);
    }

    friend difference_type constexpr operator<=>(const reverse_iterator& a, const reverse_iterator& b)
    requires random_access_iterator<It>
    {
        return a._base - b._base;
    }

private:
    It _base;
};

}   // namespace ndash


#endif   // ITERATOR_H
