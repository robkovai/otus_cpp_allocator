#pragma once

#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class custom_container {
    struct container_node {
        template <typename ... Args>
        container_node(Args && ...args): next(nullptr), value(args...) { }
        container_node* next = nullptr;
        T value;
    };
    using node_ptr = container_node*;

    typename Allocator::template rebind<container_node>::other allocator;
    node_ptr first_node = nullptr;
    node_ptr last_node = nullptr;

public:
    struct iterator {
        explicit iterator(node_ptr node = nullptr): node(node) { }
        iterator operator++() {
            this->node = this->node->next;
            return *this;
        }
        const T& operator*() const {
            return this->node->value;
        }
        friend bool operator==(iterator lhs, iterator rhs) {
            return lhs.node == rhs.node;
        }
        friend bool operator!=(iterator lhs, iterator rhs) {
            return !(lhs == rhs);
        }
    private:
        node_ptr node = nullptr;
    };

    custom_container() = default;

    custom_container(const custom_container &other) {
        this->copy(other);
    }

    template <typename OtherAllocator>
    custom_container(const custom_container<T, OtherAllocator> &other) {
        this->copy(other);
    }

    custom_container(custom_container && other) noexcept : first_node(other.first_node), last_node(other.last_node) {
        other.first_node = nullptr;
    }

    template <typename OtherAllocator>
    custom_container(custom_container<T, OtherAllocator> && other) {
        this->move(other);
    }

    ~custom_container() {
        auto node = this->first_node;
        while (node) {
            auto next_node = node->next;
            this->allocator.destroy(node);
            this->allocator.deallocate(node, 1);
            node = next_node;
        }
    }

    iterator begin() {
        return iterator(this->first_node);
    }

    iterator end() {
        return iterator();
    }

    template<typename ... Args>
    void emplace(Args && ... args) {
        auto node = this->allocator.allocate(1);
        this->allocator.construct(node, std::forward<Args>(args)...);
        this->push_back(node);
    }

    bool is_empty() const {
        return this->first_node == nullptr;
    }

private:
    void push_back(node_ptr node) {
        node->next = nullptr;
        if (!this->first_node)
            this->first_node = node;
        if (this->last_node)
            this->last_node->next = node;
        this->last_node = node;
    }

    void copy(const custom_container &other) {
        if (other.is_empty())
            return;
        auto node = other.first_node;
        while (node) {
            this->emplace(node->value);
            node = node->next;
        }
    }

    void move(custom_container && other) {
        if (other.is_empty())
            return;
        auto node = other.first_node;
        while (node) {
            this->emplace(std::move(node->value));
            node = node->next;
        }
    }
};
