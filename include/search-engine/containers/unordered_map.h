#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <cassert>
#include <cmath>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include "forward_list.h"
#include "hash.h"
#include "iterator.h"
#include "pair.h"
#include "swap.h"
#include "vector.h"

namespace ndash {

template <class Key, class T, class Hash = ndash::hash<Key>>
class unordered_map {
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = typename ndash::pair<const Key, T>;
    using bucket_type = typename ndash::forward_list<value_type>;
    using bucket_container = typename ndash::vector<bucket_type>;

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////// Constructors/Destructors ///////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Default constructor
    //
    // Sets max load factor to 2.0 and buckets to `DEFAULT_BUCKET_COUNT`
    unordered_map()
        : unordered_map(DEFAULT_BUCKET_COUNT) {}

    // Sets max load factor to 2.0 and buckets to `bucket_count`
    unordered_map(size_t bucket_count)
        : _hash_function()
        , _size(0)
        , _ml(2.0)
        , _buckets(bucket_count) {}

    // Constructs map with contents of the range [ `first`, `last` )
    template <class ForwardIt>
    unordered_map(ForwardIt first, ForwardIt last, size_t bucket_count = DEFAULT_BUCKET_COUNT)
    requires forward_iterator<ForwardIt>
        : unordered_map(bucket_count) {
        insert(first, last);
    }

    unordered_map(std::initializer_list<value_type> list, size_t bucket_count = DEFAULT_BUCKET_COUNT)
        : unordered_map(bucket_count) {
        insert(list);
    }

    // Copy constructor
    unordered_map(const unordered_map& other)
        : _hash_function(other._hash_function)
        , _size(0)
        , _ml(other._ml)
        , _buckets(other._buckets.size()) {
        insert(other.begin(), other.end());
    }

    // Move constructor
    unordered_map(unordered_map&& other)
        : _hash_function(other._hash_function)
        , _size(other._size)
        , _ml(other._ml)
        , _buckets(std::move(other._buckets)) {
        other._size = 0;
    }

    // Assignment operator
    unordered_map& operator=(const unordered_map& other) {
        if (&other != this) {
            _hash_function = other._hash_function;
            _ml = other._ml;

            _buckets.clear();
            _buckets.resize(other._buckets.size());
            insert(other.begin(), other.end());
        }
        return *this;
    }

    // Move assignment operator
    unordered_map& operator=(unordered_map&& other) {
        if (&other != this) {
            _hash_function = other._hash_function;
            _ml = other._ml;
            _size = other._size;
            _buckets = std::move(other._buckets);
            other._size = 0;
        }
        return *this;
    }

    // Initializer list assignment operator
    unordered_map& operator=(std::initializer_list<value_type> ilist) {
        clear();
        insert(ilist);
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Iterators ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

private:
    template <class T2>
    struct Iterator {
        using value_type = T2;
        using reference = T2&;
        using pointer = T2*;
        using difference_type = ptrdiff_t;

        using bucket_iterator = std::conditional_t<std::is_const_v<T2>, typename bucket_type::const_iterator,
                                                   typename bucket_type::iterator>;

        constexpr Iterator(bucket_container& buckets, size_t bucket, bucket_iterator bucket_it)
        requires(!std::is_const_v<T2>)
            : _buckets(buckets)
            , _bucket(bucket)
            , _bucket_it(bucket_it) {}

        constexpr Iterator(const bucket_container& buckets, size_t bucket, bucket_iterator bucket_it)
        requires std::is_const_v<T2>
            : _buckets(const_cast<bucket_container&>(buckets))
            , _bucket(bucket)
            , _bucket_it(bucket_it) {}

        template <class U>
        constexpr Iterator(const Iterator<U>& other)
        requires std::is_same_v<T2, const U>
            : _buckets(other._buckets)
            , _bucket(other._bucket)
            , _bucket_it(other._bucket_it) {}

        constexpr reference operator*() const { return *_bucket_it; }
        constexpr pointer operator->() const { return &(*_bucket_it); }

        constexpr Iterator& operator++() {
            ++_bucket_it;
            if (_bucket_it == _buckets[_bucket].end()) {
                ++_bucket;
                for (; _bucket != _buckets.size() - 1; ++_bucket) {
                    if (!_buckets[_bucket].empty()) {
                        _bucket_it = _buckets[_bucket].begin();
                        return *this;
                    }
                }
                _bucket_it = _buckets[_bucket].begin();
            }
            return *this;
        }

        constexpr Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool constexpr operator==(const Iterator& a, const Iterator& b) {
            return a._bucket == b._bucket && a._bucket_it == b._bucket_it;
        }
        friend bool constexpr operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }

    private:
        bucket_container& _buckets;
        size_t _bucket;
        bucket_iterator _bucket_it;
    };

public:
    using iterator = Iterator<value_type>;
    using const_iterator = Iterator<const value_type>;

    static_assert(forward_iterator<iterator>);
    static_assert(forward_iterator<const_iterator>);

    // Begin iterator
    iterator begin() {
        if (empty()) return end();
        for (auto i = 0u; i < _buckets.size(); ++i) {
            if (!_buckets[i].empty()) return iterator(_buckets, i, _buckets[i].begin());
        }
        assert(false && "Non-empty map could not find start");
    }
    const_iterator begin() const {
        if (empty()) return end();
        for (auto i = 0u; i < _buckets.size(); ++i) {
            if (!_buckets[i].empty()) return const_iterator(_buckets, i, _buckets[i].begin());
        }
        assert(false && "Non-empty map could not find start");
    }

    // End iterator
    iterator end() { return iterator(_buckets, _buckets.size() - 1, _buckets.back().end()); }
    const_iterator end() const { return const_iterator(_buckets, _buckets.size() - 1, _buckets.back().end()); }

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// Capacity ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Check if map is empty
    inline constexpr bool empty() const { return _size == 0; }

    // Get number of elements in map
    inline constexpr size_t size() const { return _size; }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Modifiers ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Erase all elements from map
    void clear() {
        for (auto it = _buckets.begin(); it != _buckets.end(); ++it) {
            it->clear();
        }
        _size = 0;
    }

    // Inserts `value` into map if it an element with the same key doesn't already exist
    ndash::pair<iterator, bool> insert(const value_type& value) { return emplace(value); }

    // Inserts `value` into map if it an element with the same key doesn't already exist
    ndash::pair<iterator, bool> insert(value_type&& value) { return emplace(value); }

    // Inserts elements from the range [ `first`, `last` )
    template <class ForwardIt>
    void insert(ForwardIt first, ForwardIt last)
    requires forward_iterator<ForwardIt>
    {
        for (; first != last; ++first) {
            emplace(*first);
        }
    }

    // Inserts elements from initializer list `ilist`
    void insert(std::initializer_list<value_type> ilist) { insert(ilist.begin(), ilist.end()); }

    // Emplace an element to the map, returns true if element has been added
    template <class... Args>
    ndash::pair<iterator, bool> emplace(Args&&... args) {
        value_type elem(std::forward<Args>(args)...);
        auto it = find(elem.first);
        if (it != end()) return { it, false };

        ++_size;
        if (size() > max_load_factor() * bucket_count()) {
            rehash(size() * 2);
        }

        auto bucket_pos = bucket(elem.first);
        auto bucket_it = _buckets[bucket_pos].emplace_back(std::move(elem));
        return { iterator(_buckets, bucket_pos, bucket_it), true };
    }

    // Try to emplace an element with key `k`
    template <class... Args>
    ndash::pair<iterator, bool> try_emplace(const Key& k, Args&&... args) {
        auto it = find(k);
        if (it != end()) return { it, false };

        ++_size;
        if (size() > max_load_factor() * bucket_count()) {
            rehash(size() * 2);
        }

        auto bucket_pos = bucket(k);
        auto bucket_it = _buckets[bucket_pos].emplace_back(std::move(value_type(k, T(std::forward<Args>(args)...))));
        return { iterator(_buckets, bucket_pos, bucket_it), true };
    }

    // Try to emplace an element with key `k`
    template <class... Args>
    ndash::pair<iterator, bool> try_emplace(Key&& k, Args&&... args) {
        auto it = find(k);
        if (it != end()) return { it, false };

        ++_size;
        if (size() > max_load_factor() * bucket_count()) {
            rehash(size() * 2);
        }

        auto bucket_pos = bucket(k);
        auto bucket_it
          = _buckets[bucket_pos].emplace_back(std::move(value_type(std::move(k), T(std::forward<Args>(args)...))));
        return { iterator(_buckets, bucket_pos, bucket_it), true };
    }

    // Erase element with key `key`. Returns true if there was an element to be erased
    bool erase(const Key& key) {
        auto bucket_pos = bucket(key);
        auto end = _buckets[bucket_pos].end();
        for (auto it = _buckets[bucket_pos].before_begin();; ++it) {
            auto prev = it++;
            if (it == end) return false;

            if (it->first == key) {
                _buckets[bucket_pos].erase_after(prev);
                --_size;
                return true;
            }
        }
    }

    // Swap contents of the map with those of `other`
    void swap(unordered_map& other) {
        ndash::swap(_hash_function, other._hash_function);
        ndash::swap(_size, other._size);
        ndash::swap(_ml, other._ml);
        ndash::swap(_buckets, other._buckets);
    }

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// Lookup /////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Access or insert a specific element
    T& operator[](const Key& key) { return try_emplace(key).first->second; }

    // Access or insert a specific element
    T& operator[](Key&& key) { return try_emplace(key).first->second; }

    // Find element with specific key. If no key matches, `end()` is returned
    iterator find(const Key& key) {
        auto bucket_pos = bucket(key);
        for (auto it = _buckets[bucket_pos].begin(); it != _buckets[bucket_pos].end(); ++it) {
            if (it->first == key) return iterator(_buckets, bucket_pos, it);
        }
        return end();
    }

    // Find element with specific key. If no key matches, `end()` is returned
    const_iterator find(const Key& key) const {
        auto bucket_pos = bucket(key);
        for (auto it = _buckets[bucket_pos].begin(); it != _buckets[bucket_pos].end(); ++it) {
            if (it->first == key) return const_iterator(_buckets, bucket_pos, it);
        }
        return end();
    }

    // Returns true if `key` in map
    bool contains(const Key& key) const { return find(key) != end(); }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Bucket Interface ////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Get number of buckets
    inline constexpr size_t bucket_count() const { return _buckets.size(); };

    // Get size of a given bucket
    inline constexpr size_t bucket_size(size_t n) const { return _buckets[n].size(); }

    // Get bucket for specific key
    inline constexpr size_t bucket(const Key& key) const { return _hash_function(key) % bucket_count(); }

    ///////////////////////////////////////////////////////////////////////////
    //////////////////////////////// Hash Policy //////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Get the current load factor
    inline constexpr float load_factor() const { return ((float) size()) / bucket_count(); }

    // Get the max load factor
    inline constexpr float max_load_factor() const { return _ml; }

    // Set the max load factor
    inline constexpr void max_load_factor(float ml) { _ml = ml; }

    // Changes number of buckets to a value `n` that is greater or equal to `count` and `size() / max_load_factor()`
    void rehash(size_t count) {
        if (count < size() / max_load_factor()) {
            count = ceil(size() / max_load_factor());
        }

        bucket_container new_buckets(count);
        for (bucket_type& _bucket : _buckets) {
            for (value_type& item : _bucket) {
                auto hash = _hash_function(item.first) % count;
                new_buckets[hash].emplace_back(std::move(item));
            }
        }
        ndash::swap(_buckets, new_buckets);
    }

    // Sets number of buckets needed to accomodate `count` elements
    void reserve(size_t count) { rehash(ceil(count / max_load_factor())); }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Observers ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Get the hash function in use
    inline constexpr Hash hash_function() const { return _hash_function; }

    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////// Non-member functions //////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Swap specialization
    friend void swap(unordered_map& a, unordered_map& b) { a.swap(b); }

private:
    Hash _hash_function;
    size_t _size;
    float _ml;
    bucket_container _buckets;

    static constexpr const int DEFAULT_BUCKET_COUNT = 1024;
};

}   // namespace ndash

#endif   // UNORDERED_MAP_H
