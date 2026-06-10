#include <algorithm>
#include <stdexcept>
#include <utility>

namespace brownian::data_structures {

template <typename T>
Deque<T>::Deque() = default;

template <typename T>
Deque<T>::Deque(const Deque& other) {
    for (Node* current = other.head; current != nullptr; current = current->next) {
        push_back(current->value);
    }
}

template <typename T>
Deque<T>& Deque<T>::operator=(const Deque& other) {
    if (this == &other) {
        return *this;
    }

    Deque copy(other);
    swap(copy);
    return *this;
}

template <typename T>
Deque<T>::Deque(Deque&& other) noexcept {
    swap(other);
}

template <typename T>
Deque<T>& Deque<T>::operator=(Deque&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    clear();
    swap(other);
    return *this;
}

template <typename T>
Deque<T>::~Deque() {
    clear();
}

template <typename T>
void Deque<T>::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
}

template <typename T>
void Deque<T>::push_front(const T& value) {
    Node* node = new Node{value, nullptr, head};

    if (head != nullptr) {
        head->previous = node;
    } else {
        tail = node;
    }

    head = node;
    ++count;
}

template <typename T>
void Deque<T>::push_back(const T& value) {
    Node* node = new Node{value, tail, nullptr};

    if (tail != nullptr) {
        tail->next = node;
    } else {
        head = node;
    }

    tail = node;
    ++count;
}

template <typename T>
void Deque<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("Deque pop_front on empty container");
    }

    Node* old_head = head;
    head = head->next;

    if (head != nullptr) {
        head->previous = nullptr;
    } else {
        tail = nullptr;
    }

    delete old_head;
    --count;
}

template <typename T>
void Deque<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("Deque pop_back on empty container");
    }

    Node* old_tail = tail;
    tail = tail->previous;

    if (tail != nullptr) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }

    delete old_tail;
    --count;
}

template <typename T>
T& Deque<T>::front() {
    if (empty()) {
        throw std::out_of_range("Deque front on empty container");
    }

    return head->value;
}

template <typename T>
const T& Deque<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Deque front on empty container");
    }

    return head->value;
}

template <typename T>
T& Deque<T>::back() {
    if (empty()) {
        throw std::out_of_range("Deque back on empty container");
    }

    return tail->value;
}

template <typename T>
const T& Deque<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Deque back on empty container");
    }

    return tail->value;
}

template <typename T>
T& Deque<T>::operator[](std::size_t index) {
    return find_node(index)->value;
}

template <typename T>
const T& Deque<T>::operator[](std::size_t index) const {
    return find_node(index)->value;
}

template <typename T>
std::size_t Deque<T>::size() const {
    return count;
}

template <typename T>
std::size_t Deque<T>::get_size() const {
    return count;
}

template <typename T>
bool Deque<T>::empty() const {
    return count == 0;
}

template <typename T>
typename Deque<T>::Node* Deque<T>::find_node(std::size_t index) const {
    if (index >= count) {
        throw std::out_of_range("Deque index is out of range");
    }

    if (index <= count / 2) {
        Node* current = head;
        for (std::size_t current_index = 0; current_index < index; ++current_index) {
            current = current->next;
        }
        return current;
    }

    Node* current = tail;
    for (std::size_t current_index = count - 1; current_index > index; --current_index) {
        current = current->previous;
    }
    return current;
}

template <typename T>
void Deque<T>::swap(Deque& other) noexcept {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(count, other.count);
}

} // namespace brownian::data_structures


