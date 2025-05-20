#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include "iterator.h"
#include "swap.h"

namespace ndash {

// Linked list that only supports forward traversal
// Unlike std::forward_list, the list size and a pointer to the tail is kept
template <class T>
class forward_list {
private:
    struct Node {
        T val;
        Node* next;
    };

public:
    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////// Constructors/Destructors ///////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Default constructor
    //
    // Initializes list with size 0
    constexpr forward_list()
        : head(nullptr)
        , tail(nullptr)
        , _size(0) {
        before.next = head;
    };

    // Constructs a list with `count` size, all default initialized
    forward_list(size_t count)
        : forward_list() {
        while (count-- > 0) emplace_back();
    }

    // Constructs a list with `count` size, all set to `value`
    forward_list(size_t count, const T& value)
        : forward_list() {
        while (count-- > 0) emplace_back(value);
    }

    // Constructs a list with the contents of the range [`first`, `last`)
    template <typename ForwardIt>
    forward_list(ForwardIt first, ForwardIt last)
    requires forward_iterator<ForwardIt>
        : forward_list() {
        for (; first != last; ++first) {
            emplace_back(*first);
        }
    }

    // Constructs a vector from the initializer list
    forward_list(std::initializer_list<T> list)
        : forward_list(list.begin(), list.end()) {};

    // Copy constructor
    forward_list(const forward_list& other)
        : forward_list() {
        auto other_node = other.head;
        while (other_node) {
            emplace_back(other_node->val);
            other_node = other_node->next;
        }
    }

    // Move constructor
    forward_list(forward_list&& other)
        : head(other.head)
        , tail(other.tail)
        , _size(other._size) {
        before.next = head;

        other.head = nullptr;
        other.tail = nullptr;
        other._size = 0;
    }

    // Copy assignment operator
    forward_list& operator=(const forward_list& other) {
        if (&other != this) {
            forward_list temp(other);
            this->swap(temp);
        }
        return *this;
    }

    // Move assignment operator
    forward_list& operator=(forward_list&& other) {
        if (&other != this) {
            this->swap(other);
            other.head = nullptr;
            other.tail = nullptr;
            other._size = 0;
        }
        return *this;
    }

    // Initializer list assignment
    forward_list& operator=(std::initializer_list<T> ilist) {
        clear();
        for (auto it = ilist.begin(); it != ilist.end(); ++it) {
            emplace_back(*it);
        }
        return *this;
    }

    ~forward_list() { clear(); };

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Iterators ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

private:
    // Forward list iterator struct
    template <class T2>
    struct Iterator {
        using value_type = T2;
        using reference = T2&;
        using pointer = T2*;
        using difference_type = ptrdiff_t;

        constexpr Iterator(Node* ptr)
        requires(!std::is_const_v<T2>)
            : _node(ptr) {}

        constexpr Iterator(const Node* ptr)
        requires std::is_const_v<T2>
            : _node(const_cast<Node*>(ptr)) {}

        template <class U>
        constexpr Iterator(const Iterator<U>& other)
        requires std::is_same_v<T2, const U>
            : _node(other._node) {}

        constexpr reference operator*() const { return _node->val; }
        constexpr pointer operator->() const { return &_node->val; }

        constexpr Iterator& operator++() {
            _node = _node->next;
            return *this;
        }

        constexpr Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool constexpr operator==(const Iterator& a, const Iterator& b) { return a._node == b._node; }
        friend bool constexpr operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }

        Node* _node;
    };


public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;

    static_assert(forward_iterator<iterator>);
    static_assert(forward_iterator<const_iterator>);

    // Iterator before begin
    constexpr iterator before_begin() { return iterator(&before); }
    constexpr const_iterator cbefore_begin() const { return const_iterator(&before); }

    // Iterator begin
    constexpr iterator begin() { return iterator(head); }
    constexpr const_iterator begin() const { return const_iterator(head); }

    // Iterator end
    constexpr iterator end() { return iterator(nullptr); }
    constexpr const_iterator end() const { return const_iterator(nullptr); }

    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////////// Element Access ////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Access first element
    constexpr T& front() { return head->val; }

    // Access first element by const reference
    constexpr const T& front() const { return head->val; }

    // Access last element
    constexpr T& back() { return tail->val; }

    // Access last element by const reference
    constexpr const T& back() const { return tail->val; }

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// Capacity ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Check if container is empty
    constexpr bool empty() const { return !_size; }

    // Get number of elements in list
    constexpr size_t size() const { return _size; }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// Modifiers ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Clears the contents of the list
    void clear() {
        while (_size) pop_front();
    }

    // Insert a copy of `value` after `pos`
    iterator insert_after(const_iterator pos, const T& value) { return emplace_after(pos, value); }

    // Moves `value` to node after `pos`
    iterator insert_after(const_iterator pos, T&& value) { return emplace_after(pos, value); }

    // Emplaces value after `pos`
    template <class... Args>
    iterator emplace_after(const_iterator pos, Args&&... args) {
        Node* next = new Node { T(std::forward<Args>(args)...), nullptr };

        if (_size == 0) {
            head = next;
            tail = next;
            ++_size;
            before.next = head;
            return iterator(head);
        }

        ++_size;
        if (pos._node->next == nullptr) {
            pos._node->next = next;
            tail = next;
            return iterator(tail);
        }

        next->next = pos._node->next;
        pos._node->next = next;

        // If we inserted new head, update
        if (next->next == head) {
            head = next;
            before.next = next;
        }

        return iterator(next);
    }

    // Erase element after `pos`
    iterator erase_after(const_iterator pos) {
        auto next = pos._node->next;
        if (next) {
            pos._node->next = next->next;
            if (next == head) head = next->next;
            if (next == tail && _size > 1) {
                if (_size > 1)
                    tail = pos._node;
                else
                    tail = nullptr;
            }

            delete next;
            --_size;
        }
        return iterator(pos._node->next);
    }

    // Adds an element to the end of the list
    iterator push_back(const T& value) { return emplace_back(value); }

    // Adds an element to the end of the list using move semantics
    iterator push_back(T&& value) { return emplace_back(std::move(value)); }

    // Creates element at back of list
    template <class... Args>
    iterator emplace_back(Args&&... args) {
        return emplace_after(const_iterator(tail), std::forward<Args>(args)...);
    }

    // Adds an element to the front of the list
    iterator push_front(const T& value) { return emplace_front(value); }

    // Adds an element to the front of the list using move semantics
    iterator push_front(T&& value) { return emplace_front(std::move(value)); }

    // Creates element at front of list
    template <class... Args>
    iterator emplace_front(Args&&... args) {
        return emplace_after(const_iterator(&before), std::forward<Args>(args)...);
    }

    // Removes the first element
    void pop_front() { erase_after(&before); }

    // Swap with another forward list
    void swap(forward_list& other) {
        ndash::swap(before, other.before);
        ndash::swap(head, other.head);
        ndash::swap(tail, other.tail);
        ndash::swap(_size, other._size);
    }

private:
    Node before;
    Node* head;
    Node* tail;
    size_t _size;
};


}   // namespace ndash

#endif   // FORWARD_LIST_H
