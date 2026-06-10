#ifndef BROWNIAN_DYNAMIC_ARRAY_HPP
#define BROWNIAN_DYNAMIC_ARRAY_HPP

#include <cstddef>
#include <vector>

namespace brownian::data_structures {

template <typename T>
class DynamicArray {
public:
    DynamicArray();
    explicit DynamicArray(std::size_t initial_capacity);
    DynamicArray(const DynamicArray& other);
    DynamicArray& operator=(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;
    DynamicArray& operator=(DynamicArray&& other) noexcept;
    ~DynamicArray();

    void clear();
    void reserve(std::size_t new_capacity);
    void resize(std::size_t new_size);
    void push_back(const T& value);
    void erase(std::size_t index);

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

    std::size_t get_size() const;
    std::size_t get_capacity() const;
    bool empty() const;
    std::vector<T> to_std_vector() const;

private:
    void swap(DynamicArray& other) noexcept;

    T* data = nullptr;
    std::size_t size = 0;
    std::size_t capacity = 0;
};

} // namespace brownian::data_structures

#include "dynamicArray.tpp"

#endif // BROWNIAN_DYNAMIC_ARRAY_HPP
