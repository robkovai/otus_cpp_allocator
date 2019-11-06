#include <iostream>
#include <map>

#include "custom_allocator.h"
#include "custom_container.h"
#include "hard.h"

using namespace std;

const int count_loop = 10;

void make_standard_container_with_standard_allocator() {
    auto container = map<int, hard>();
    for (int i = 0; i <= count_loop; ++i)
        container.emplace(piecewise_construct, forward_as_tuple(i), forward_as_tuple(factorial(i), fibonacci(i)));
}

void make_standard_container_with_custom_allocator() {
    auto container = map<int, hard, less<int>, custom_allocator<pair<const int, hard>, count_loop>>();
    for (int i = 0; i <= count_loop; ++i)
        container.emplace(piecewise_construct, forward_as_tuple(i), forward_as_tuple(factorial(i), fibonacci(i)));
    for (const auto &i : container)
        cout << i.first << " " << i.second.fa << " " << i.second.fi << endl;
}

void make_custom_container_with_standard_allocator() {
    auto container = custom_container<hard>();
    for (int i = 0; i <= count_loop; ++i)
        container.emplace(factorial(i), fibonacci(i));
}

void make_custom_container_with_custom_allocator() {
    auto container = custom_container<hard, custom_allocator<hard, count_loop>>();
    for (int i = 0; i <= count_loop; ++i)
        container.emplace(factorial(i), fibonacci(i));
    for (const auto &i : container)
        cout << i.fa << " " << i.fi << endl;
}

int main() {
    make_standard_container_with_standard_allocator();
    make_standard_container_with_custom_allocator();
    make_custom_container_with_standard_allocator();
    make_custom_container_with_custom_allocator();
    return 0;
}
