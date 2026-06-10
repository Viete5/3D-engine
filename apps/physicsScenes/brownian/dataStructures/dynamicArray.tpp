#ifndef BROWNIAN_DYNAMIC_ARRAY_TPP
#define BROWNIAN_DYNAMIC_ARRAY_TPP

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace brownian::data_structures {

template <typename T>
DynamicArray<T>::DynamicArray() = default;

template <typename T>
DynamicArray<T>::DynamicArray(std::size_t initial_capacity) {
    reserve(initial_capacity);
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) {
    reserve(other.size);
    for (const T& value : other) {
        push_back(value);
    }
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this == &other) {
        return *this;
    }

    DynamicArray copy(other);
    swap(copy);
    return *this;
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& other) noexcept {
    swap(other);
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
    swap(other);
    return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template <typename T>
void DynamicArray<T>::clear() {
    size = 0;
}

template <typename T>
void DynamicArray<T>::reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity) {
        return;
    }

    T* new_data = new T[new_capacity];
    for (std::size_t index = 0; index < size; ++index) {
        new_data[index] = std::move(data[index]);
    }

    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

template <typename T>
void DynamicArray<T>::resize(std::size_t new_size) {
    if (new_size > capacity) {
        reserve(new_size);
    }

    size = new_size;
}

template <typename T>
void DynamicArray<T>::push_back(const T& value) {
    if (size == capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }

    data[size] = value;
    ++size;
}

template <typename T>
void DynamicArray<T>::erase(std::size_t index) {
    if (index >= size) {
        throw std::out_of_range("DynamicArray erase index is out of range");
    }

    for (std::size_t current = index + 1; current < size; ++current) {
        data[current - 1] = std::move(data[current]);
    }

    --size;
}

template <typename T>
T& DynamicArray<T>::operator[](std::size_t index) {
    if (index >= size) {
        throw std::out_of_range("DynamicArray getter index is out of range");
    }

    return data[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](std::size_t index) const {
    if (index >= size) {
        throw std::out_of_range("DynamicArray getter index is out of range");
    }

    return data[index];
}

template <typename T>
T* DynamicArray<T>::begin() {
    return data;
}

template <typename T>
T* DynamicArray<T>::end() {
    return data + size;
}

template <typename T>
const T* DynamicArray<T>::begin() const {
    return data;
}

template <typename T>
const T* DynamicArray<T>::end() const {
    return data + size;
}

template <typename T>
std::size_t DynamicArray<T>::get_size() const {
    return size;
}

template <typename T>
std::size_t DynamicArray<T>::get_capacity() const {
    return capacity;
}

template <typename T>
bool DynamicArray<T>::empty() const {
    return size == 0;
}

template <typename T>
std::vector<T> DynamicArray<T>::to_std_vector() const {
    std::vector<T> result;
    result.reserve(size);
    for (const T& value : *this) {
        result.push_back(value);
    }
    return result;
}

template <typename T>
void DynamicArray<T>::swap(DynamicArray& other) noexcept {
    std::swap(data, other.data);
    std::swap(size, other.size);
    std::swap(capacity, other.capacity);
}

} // namespace brownian::data_structures

#endif // BROWNIAN_DYNAMIC_ARRAY_TPP
