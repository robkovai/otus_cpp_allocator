#pragma once

#include <memory>

template<typename T, std::size_t Count>
class custom_allocator {
    struct allocator_node {
        T *values;
        allocator_node *prev;
    };
    std::size_t availabled = 0;
    allocator_node *current_node = nullptr;
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = custom_allocator<U, Count>;
    };

    custom_allocator() = default;

    custom_allocator(const custom_allocator &other) = delete;

    custom_allocator(custom_allocator &&other) noexcept : availabled(other.availabled), current_node(other.current_node) {
        other.current_node = nullptr;
    }

    T *allocate(std::size_t n) {
        if (n <= this->availabled) {
            auto currentData = this->current_node->values + (Count - this->availabled);
            this->availabled -= n;
            return currentData;
        } else {
            auto count_values = std::max(n, Count);
            auto data = reinterpret_cast<T*>(std::malloc(count_values * sizeof(T)));
            if (!data)
                throw std::bad_alloc();
            this->availabled = count_values - n;
            this->current_node = new allocator_node{data, this->current_node};
            return data;
        }
    }

    void deallocate(T *, std::size_t) { }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) const {
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U *p) const {
        p->~U();
    }

    ~custom_allocator() {
        while (this->current_node) {
            std::free(this->current_node->values);
            auto prev_node = this->current_node->prev;
            delete this->current_node;
            this->current_node = prev_node;
        }
    }
};
