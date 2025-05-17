#ifndef NDSTRING_H
#define NDSTRING_H

#include <cstddef>
#include <cstring>
#include <ostream>

#include "swap.h"
#include "vector.h"

namespace ndash {

constexpr inline size_t strlen(const char* s) {
    if (!s) return 0;
    auto it = s;
    for (; *it; ++it);
    return it - s;
}

// String class
class string {
public:
    // Special value indicating no position
    static constexpr const size_t npos = size_t(-1);

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////// Constructors/Destructors ///////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Default construct
    //
    // Initializes string to empty string
    string()
        : _data() {
        _data.push_back('\0');
    }

    // Constructs a string with `count` copies of `ch`
    string(size_t count, char ch)
        : _data(count, ch) {
        _data.push_back('\0');
    }

    // Constructs a vector with the contents of the range [`first`, `last`)
    template <typename InputIt>
    string(InputIt first, InputIt last)
        : _data(first, last) {
        _data.push_back('\0');
    }

    // Constructs string from cstring with defined size
    string(const char* s, size_t count)
        : _data(s, s + count) {
        _data.push_back('\0');
    }

    // Constructs string from cstring
    string(const char* s)
        : string(s, ndash::strlen(s)) {}

    // Constructs a vector from the initializer list
    string(std::initializer_list<char> list)
        : _data(list) {
        _data.push_back('\0');
    }

    // Copy constructor
    string(const string& other)
        : _data(other._data) {}

    // Copy move constructor
    string(string&& other)
        : _data(std::move(other._data)) {}

    // Assignment operator
    string& operator=(const string& other) {
        if (&other != this) {
            _data = other._data;
        }
        return *this;
    }

    // Move assignment operator
    string& operator=(string&& other) {
        if (&other != this) {
            _data = std::move(other._data);
        }
        return *this;
    }

    // C str assignment
    string& operator=(const char* s) {
        auto s_size = ndash::strlen(s);
        resize(s_size);
        memcpy(_data.data(), s, s_size);
        return *this;
    }

    // Char assignment
    string& operator=(char ch) {
        clear();
        push_back(ch);
        return *this;
    }

    // Initializer list assignment
    string& operator=(std::initializer_list<char> ilist) {
        resize(ilist.size());
        int index = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it, ++index) {
            _data[index] = *it;
        }
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////////// Element Access ////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Access specified element
    constexpr char& operator[](size_t pos) { return _data[pos]; }

    // Access specified element by const reference
    constexpr const char& operator[](size_t pos) const { return _data[pos]; }

    // Access first element
    constexpr char& front() { return _data.front(); }

    // Access first element by const reference
    constexpr const char& front() const { return _data.front(); }

    // Access last element
    constexpr char& back() { return _data[_data.size() - 2]; }

    // Access last element by const reference
    constexpr const char& back() const { return _data[_data.size() - 2]; }

    // Directly access data
    constexpr char* data() { return _data.data(); }

    // Directly access data
    constexpr const char* data() const { return _data.data(); }

    // Get c string
    constexpr const char* c_str() const { return _data.data(); }

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// Capacity ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Check if string is empty
    constexpr bool empty() const { return _data.size() == 1; }

    // Number of characters in string
    constexpr size_t size() const { return _data.size() - 1; }

    // Number of characters in string
    constexpr size_t length() const { return _data.size() - 1; }

    // Reserves storage
    void reserve(size_t new_cap) { _data.reserve(new_cap + 1); }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Modifiers ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Clears the string
    void clear() {
        _data.clear();
        _data.push_back('\0');
    }

    // Adds a character to the end of the string
    void push_back(const char ch) {
        _data.back() = ch;
        _data.push_back('\0');
    }

    // Removes the last element
    void pop_back() {
        _data.pop_back();
        _data.back() = '\0';
    }

    // Append `count` copies of `ch` to the end of the string
    string& append(size_t count, char ch) {
        resize(size() + count, ch);
        return *this;
    }

    // Append the first `count` characters from `s` to the string
    string& append(const char* s, size_t count) {
        auto original_size = size();
        resize(size() + count);
        memcpy(_data.data() + original_size, s, count);
        return *this;
    }

    // Append c style string to end
    string& append(const char* s) { return append(s, ndash::strlen(s)); }

    // Append string to end
    string& append(const string& str, size_t pos = 0, size_t count = npos) {
        count = count > str.size() - pos ? str.size() - pos : count;
        return append(str.c_str() + pos, count);
    }

    // Append initializer list to end
    string& append(std::initializer_list<char> ilist) { return append(ilist.begin(), ilist.size()); }

    // Append string `str` to string
    string& operator+=(const string& str) { return append(str); }

    // Append character `ch` to string
    string& operator+=(char ch) {
        push_back(ch);
        return *this;
    }

    // Append null terminated `s` to string
    string& operator+=(const char* s) { return append(s); }

    // Append characters in initializer list `ilist` to string
    string& operator+=(std::initializer_list<char> ilist) { return append(ilist); }

    // Copy substring [ `pos`, `pos + count` ) to character string pointed to by `dest`
    size_t copy(char* dest, size_t count, size_t pos = 0) const {
        if (count == npos) {
            count = _data.size() - pos;
        }
        memcpy(dest, _data.data() + pos, count);
        return count;
    }

    // Resize string to contain `count` characters
    //
    // If `count` is less than size, the string is reduced to the first `count` elements
    // if `count` is greater than size, additional `ch` are appended
    void resize(size_t count, char ch = '\0') {
        if (count > size()) {
            auto original_size = size();
            _data.resize(count + 1);
            if (ch) {
                memset(_data.data() + original_size, ch, count - original_size);
            }
        } else if (count < size()) {
            _data.resize(count + 1);
            _data.back() = '\0';
        }
    }

    // Swap with another string
    void swap(string& other) { ndash::swap(_data, other._data); }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Iterators ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Iterator begin
    constexpr auto begin() { return _data.begin(); }
    constexpr auto begin() const { return _data.begin(); }

    // Iterator end
    constexpr auto end() { return _data.end() - 1; }
    constexpr auto end() const { return _data.end() - 1; }

    // Iterator rbegin
    constexpr auto rbegin() { return _data.rbegin() + 1; }
    constexpr auto rbegin() const { return _data.rbegin() + 1; }

    // Iterator rend
    constexpr auto rend() { return _data.rend(); }
    constexpr auto rend() const { return _data.rend(); }

    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////////////// Search ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Find first instance of string after `pos`
    constexpr size_t find(const string& str, size_t pos = 0) const { return find(str.c_str(), pos, str.size()); }

    // Find first substring equal to [ `s`, `s` + `count` ) after `pos`
    constexpr size_t find(const char* s, size_t pos, size_t count) const {
        for (; pos < size() - count; ++pos) {
            size_t i = 0;
            for (; i < count && _data[pos + i] == s[i]; ++i);
            if (i == count) return pos;
        }
        return npos;
    }

    // Find first instance of c_str `s` after `pos`
    constexpr size_t find(const char* s, size_t pos = 0) const { return find(s, pos, strlen(s)); }

    // Find the first instance of `ch` after `pos`
    constexpr size_t find(char ch, size_t pos = 0) const {
        for (; pos < size(); ++pos) {
            if (_data[pos] == ch) return pos;
        }
        return npos;
    }

    // Find last instance of string before `pos`
    constexpr size_t rfind(const string& str, size_t pos = npos) const { return rfind(str.c_str(), pos, str.size()); }

    // Find last substring equal to [ `s`, `s` + `count` ) before `pos`
    constexpr size_t rfind(const char* s, size_t pos, size_t count) const {
        if (pos > size()) pos = size() - count + 1;
        pos++;
        while (pos-- > 0) {
            size_t i = 0;
            for (; i < count && _data[pos + i] == s[i]; ++i);
            if (i == count) return pos;
        }
        return npos;
    }

    // Find last instance of c_str `s` before `pos`
    constexpr size_t rfind(const char* s, size_t pos = npos) const { return rfind(s, pos, strlen(s)); }

    // Find the last instance of `ch` before `pos`
    constexpr size_t rfind(char ch, size_t pos = npos) const {
        if (pos > size()) pos = size();
        pos++;
        while (pos-- > 0) {
            if (_data[pos] == ch) return pos;
        }
        return npos;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Operations //////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Compares the string to the characters in `str`
    constexpr int compare(const string& str) const { return compare(0, size(), str.c_str(), str.size()); }

    // Compares a [ `pos1`, `pos1 + count1` ) substring of this string to the characters in `str`
    constexpr int compare(size_t pos1, size_t count1, const string& str) const {
        return compare(pos1, count1, str.c_str(), str.size());
    }

    // Compares a [ `pos1`, `pos1 + count1` ) substring of this string to the characters
    // in the range [ `pos2`, `pos2 + count2` ) of `str`
    constexpr int compare(size_t pos1, size_t count1, const string& str, size_t pos2, size_t count2) const {
        return compare(pos1, count1, str.c_str() + pos2, count2);
    }

    // Compares the string to the characters in `s`
    constexpr int compare(const char* s) const { return compare(0, size(), s, strlen(s)); }

    // Compares a [ `pos1`, `pos1 + count1` ) substring of this string to the characters in `s`
    constexpr int compare(size_t pos1, size_t count1, const char* s) const {
        return compare(pos1, count1, s, strlen(s));
    }

    // Compares a [ `pos1`, `pos1 + count1` ) substring of this string to the characters
    // in the range [ `s`, `s + count2` )
    constexpr int compare(size_t pos1, size_t count1, const char* s, size_t count2) const {
        if (count1 > size() - pos1) count1 = size() - pos1;

        size_t i = 0;
        for (; i < count1 - 1 && i < count2 - 1 && _data[pos1 + i] == s[i]; ++i);
        return _data[pos1 + i] - s[i];
    }

    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////// Non-member functions //////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Add string to string
    friend string operator+(const string& lhs, const string& rhs) {
        string copy(lhs);
        return copy.append(rhs);
    }

    // Add string and c string
    friend string operator+(const string& lhs, const char* rhs) {
        string copy(lhs);
        return copy.append(rhs);
    }

    // Add string and char
    friend string operator+(const string& lhs, char rhs) {
        string copy(lhs);
        copy.push_back(rhs);
        return copy;
    }

    // Add c string and string
    friend string operator+(const char* lhs, const string& rhs) {
        string copy(lhs);
        return copy.append(rhs);
    }

    // Add char and string
    friend string operator+(char lhs, const string& rhs) {
        string copy(1, lhs);
        copy.append(rhs);
        return copy;
    }

    friend constexpr bool operator==(const string& a, const string& b) { return a.compare(b) == 0; }
    friend constexpr int operator<=>(const string& a, const string& b) { return a.compare(b); }

    friend constexpr bool operator==(const string& a, const char* b) { return a.compare(b) == 0; }
    friend constexpr int operator<=>(const string& a, const char* b) { return a.compare(b); }

    friend constexpr bool operator==(const char* a, const string& b) { return b.compare(a) == 0; }
    friend constexpr int operator<=>(const char* a, const string& b) { return -b.compare(a); }

    friend std::ostream& operator<<(std::ostream& os, const ndash::string& str) {
        os.write(str.c_str(), str.size());
        return os;
    }

    // Swap specialization
    friend void swap(string& a, string& b) { a.swap(b); }

private:
    vector<char> _data;
};

}   // namespace ndash

#endif   // NDSTRING_H
