#pragma once

#include <memory>
#include <list>

template<typename T, std::size_t Count>
class custom_allocator {
    std::size_t availabled = 0;
    std::list<T*> parts;

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

    T *allocate(std::size_t n) {
        if (n <= this->availabled) {
            auto currentData = this->parts.back() + (Count - this->availabled);
            this->availabled -= n;
            return currentData;
        } else {
            auto bytes = n * sizeof(T) * Count;
            auto data = reinterpret_cast<T*>(std::malloc(bytes));
            if (!data)
                throw std::bad_alloc();
            this->availabled = n * Count - n;
            this->parts.push_back(data);
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
        while (!this->parts.empty()) {
            std::free(this->parts.front());
            this->parts.pop_front();
        }
    }
};
