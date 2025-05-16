#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <utility>

namespace ndash {

template <typename T>
class vector {
public:
    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////// Constructors/Destructors ///////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Default constructor
    //
    // Initializes vector to size 0 with capacity 0
    vector()
        : _size(0)
        , _capacity(0)
        , _data(nullptr) {}

    // Constructs a vector with `count` size and `count` capacity, all default initialized
    vector(size_t count)
        : _size(count)
        , _capacity(count)
        , _data(new T[count] {}) {}

    // Constructs a vector with `count` size and `count` capacity, all set to `value`
    vector(size_t count, const T& value)
        : _size(count)
        , _capacity(count)
        , _data(new T[count]) {
        for (auto i = _data; i < _data + count; ++i) {
            *i = value;
        }
    }

    // Constructs a vector with the contents of the range [`first`, `last`)
    template <typename InputIt>
    vector(InputIt first, InputIt last)
        : vector() {
        while (first != last) {
            push_back(*(first++));
        }
    }

    // Constructs a vector from the initializer list
    vector(std::initializer_list<T> list)
        : _size(0)
        , _capacity(list.size())
        , _data(new T[list.size()]) {
        for (auto it = list.begin(); it != list.end(); ++it) {
            push_back(*it);
        }
    }

    // Copy constructor
    vector(const vector& other)
        : _size(other._size)
        , _capacity(other._capacity)
        , _data(new T[other._capacity]) {
        memcpy(_data, other._data, other._size * sizeof(T));
    }

    // Move constructor
    vector(vector&& other)
        : _size(other._size)
        , _capacity(other._capacity)
        , _data(other._data) {
        other._size = 0;
        other._capacity = 0;
        other._data = nullptr;
    }

    // Assignment operator
    vector& operator=(const vector& other) {
        if (&other != this) {
            vector temp(other);
            this->swap(temp);
        }
        return *this;
    }

    // Move assignment operator
    vector& operator=(vector&& other) {
        if (&other != this) {
            delete[] _data;

            _size = other._size;
            _capacity = other._capacity;
            _data = other._data;

            other._size = 0;
            other._capacity = 0;
            other._data = nullptr;
        }
        return *this;
    }

    vector& operator=(std::initializer_list<T> ilist) {
        reserve(ilist.size());
        _size = ilist.size();
        int i = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it, ++i) {
            _data[i] = *it;
        }
        return *this;
    }

    ~vector() { delete[] _data; }

    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////////// Element Access ////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Access specified element
    constexpr T& operator[](size_t pos) { return _data[pos]; }

    // Access specified element by const reference
    constexpr const T& operator[](size_t pos) const { return _data[pos]; }

    // Access first element
    constexpr T& front() { return *_data; }

    // Access first element by const reference
    constexpr const T& front() const { return *_data; }

    // Access last element
    constexpr T& back() { return *(_data + _size - 1); }

    // Access last element by const reference
    constexpr const T& back() const { return *(_data + _size - 1); }

    // Directly access data
    constexpr T* data() { return _data; }

    // Directly access constant data
    constexpr const T* data() const { return _data; }

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// Capacity ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Check if container is empty
    constexpr bool empty() const { return !_size; }

    // Get number of elements in vector
    constexpr size_t size() const { return _size; }

    // Reserves storage
    //
    // If `new_cap` is less than current capacity, nothing is done
    void reserve(size_t new_cap) {
        if (new_cap < _capacity) return;

        T* copy = new T[new_cap];
        memcpy(copy, _data, sizeof(T) * _size);
        delete[] _data;
        _data = copy;
        _capacity = new_cap;
    }

    // Get number of elements that can be held in vector
    constexpr size_t capacity() const { return _capacity; }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Modifiers ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Clears the contents of the vector
    void clear() { _size = 0; }

    // Adds an element to the end of the vector
    void push_back(const T& value) { emplace_back(value); }

    // Adds an element to the end of the vector using move semantics
    void push_back(T&& value) { emplace_back(std::move(value)); }

    // Creates element at back of vector
    template <class... Args>
    void emplace_back(Args&&... args) {
        [[unlikely]] if (_size == _capacity) { reserve(_capacity ? _capacity * 2 : 8); }
        _data[_size++] = T(std::forward<Args>(args)...);
    }

    // Removes the last element
    void pop_back() { --_size; }

    // Resize vector
    //
    // If count is equal to current size, do nothing
    // If count is greater than current size, container is reduced to the first count elements
    // If count is less than current size, additional copies of T are appended
    void resize(size_t count) {
        if (count > _size) {
            reserve(count);
            while (_size < count) {
                emplace_back();
            }
        } else if (count < _size) {
            _size = count;
        }
    }

    // Resize vector
    //
    // If count is equal to current size, do nothing
    // If count is greater than current size, container is reduced to the first count elements
    // If count is less than current size, additional copies of `value` are appended
    void resize(size_t count, const T& value) {
        if (count > _size) {
            reserve(count);
            while (_size < count) {
                emplace_back(value);
            }
        } else if (count < _size) {
            _size = count;
        }
    }

    // Swap with another vector
    void swap(vector& other) {
        auto temp_size = other._size;
        other._size = _size;
        _size = temp_size;

        auto temp_capacity = other._capacity;
        other._capacity = _capacity;
        _capacity = temp_capacity;

        auto temp_data = other._data;
        other._data = _data;
        _data = temp_data;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Iterators ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Vector iterator struct
    struct Iterator {
        constexpr Iterator(T* ptr)
            : _ptr(ptr) {}

        constexpr T& operator*() const { return *_ptr; }
        constexpr T* operator->() const { return _ptr; }

        constexpr Iterator& operator++() {
            _ptr++;
            return *this;
        }

        constexpr Iterator& operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        constexpr Iterator& operator--() {
            _ptr--;
            return *this;
        }

        constexpr Iterator& operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        constexpr Iterator& operator+=(ptrdiff_t diff) {
            _ptr += diff;
            return *this;
        }

        constexpr Iterator& operator-=(ptrdiff_t diff) {
            _ptr -= diff;
            return *this;
        }

        constexpr T& operator[](ptrdiff_t diff) { return _ptr[diff]; }
        constexpr const T& operator[](ptrdiff_t diff) const { return _ptr[diff]; }

        friend constexpr Iterator operator+(const Iterator& a, ptrdiff_t diff) { return Iterator(a._ptr + diff); }
        friend constexpr Iterator operator+(ptrdiff_t diff, const Iterator& a) { return a + diff; }
        friend constexpr Iterator operator-(const Iterator& a, ptrdiff_t diff) { return Iterator(a._ptr - diff); }
        friend constexpr Iterator operator-(ptrdiff_t diff, const Iterator& a) { return a - diff; }

        friend bool constexpr operator==(const Iterator& a, const Iterator& b) { return a._ptr == b._ptr; }
        friend bool constexpr operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }
        friend bool constexpr operator<(const Iterator& a, const Iterator& b) { return a._ptr < b._ptr; }
        friend bool constexpr operator<=(const Iterator& a, const Iterator& b) { return a._ptr <= b._ptr; }
        friend bool constexpr operator>(const Iterator& a, const Iterator& b) { return a._ptr > b._ptr; }
        friend bool constexpr operator>=(const Iterator& a, const Iterator& b) { return a._ptr >= b._ptr; }

    private:
        T* _ptr;
    };

    // Vector reverse iterator struct
    struct ReverseIterator {
        constexpr ReverseIterator(T* ptr)
            : _ptr(ptr) {}

        constexpr T& operator*() const { return *_ptr; }
        constexpr T* operator->() const { return _ptr; }

        constexpr ReverseIterator& operator++() {
            _ptr--;
            return *this;
        }

        constexpr ReverseIterator& operator++(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        constexpr ReverseIterator& operator--() {
            _ptr++;
            return *this;
        }

        constexpr ReverseIterator& operator--(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        constexpr ReverseIterator& operator+=(ptrdiff_t diff) {
            _ptr -= diff;
            return *this;
        }

        constexpr ReverseIterator& operator-=(ptrdiff_t diff) {
            _ptr += diff;
            return *this;
        }

        constexpr T& operator[](ptrdiff_t diff) { return _ptr[-diff]; }
        constexpr const T& operator[](ptrdiff_t diff) const { return _ptr[-diff]; }

        friend constexpr ReverseIterator operator+(const ReverseIterator& a, ptrdiff_t diff) {
            return ReverseIterator(a._ptr - diff);
        }
        friend constexpr ReverseIterator operator+(ptrdiff_t diff, const ReverseIterator& a) { return a + diff; }
        friend constexpr ReverseIterator operator-(const ReverseIterator& a, ptrdiff_t diff) {
            return ReverseIterator(a._ptr + diff);
        }
        friend constexpr ReverseIterator operator-(ptrdiff_t diff, const ReverseIterator& a) { return a - diff; }

        friend constexpr bool operator==(const ReverseIterator& a, const ReverseIterator& b) {
            return a._ptr == b._ptr;
        }
        friend constexpr bool operator!=(const ReverseIterator& a, const ReverseIterator& b) { return !(a == b); }
        friend constexpr bool operator<(const ReverseIterator& a, const ReverseIterator& b) { return a._ptr > b._ptr; }
        friend constexpr bool operator<=(const ReverseIterator& a, const ReverseIterator& b) {
            return a._ptr >= b._ptr;
        }
        friend constexpr bool operator>(const ReverseIterator& a, const ReverseIterator& b) { return a._ptr < b._ptr; }
        friend constexpr bool operator>=(const ReverseIterator& a, const ReverseIterator& b) {
            return a._ptr <= b._ptr;
        }

    private:
        T* _ptr;
    };

    // Iterator begin
    constexpr Iterator begin() { return Iterator(_data); }

    // Iterator end
    constexpr Iterator end() { return Iterator(_data + _size); }

    // Iterator rbegin
    constexpr ReverseIterator rbegin() { return ReverseIterator(_data + _size - 1); }

    // Iterator rend
    constexpr ReverseIterator rend() { return ReverseIterator(_data - 1); }

private:
    size_t _size;
    size_t _capacity;
    T* _data;
};

}   // namespace ndash

#endif   // VECTOR_H
