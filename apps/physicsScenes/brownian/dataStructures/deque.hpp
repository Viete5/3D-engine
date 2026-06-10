#ifndef BROWNIAN_DEQUE_HPP
#define BROWNIAN_DEQUE_HPP

#include <cstddef>

namespace brownian::data_structures {

template <typename T>
class Deque {
public:
    Deque();
    Deque(const Deque& other);
    Deque& operator=(const Deque& other);
    Deque(Deque&& other) noexcept;
    Deque& operator=(Deque&& other) noexcept;
    ~Deque();

    void clear();
    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front();
    void pop_back();

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    std::size_t size() const;
    std::size_t get_size() const;
    bool empty() const;

private:
    struct Node {
        T value;
        Node* previous = nullptr;
        Node* next = nullptr;
    };

    Node* find_node(std::size_t index) const;
    void swap(Deque& other) noexcept;

    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t count = 0;
};

} // namespace brownian::data_structures

#include "deque.tpp"

#endif // BROWNIAN_DEQUE_HPP
