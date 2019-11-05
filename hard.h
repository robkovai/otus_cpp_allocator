#pragma once

struct hard {
    int fa;
    int fi;

    hard(int fa, int fi) : fa(fa), fi(fi) { }
    hard(const hard &) = delete;
    hard(hard &&) noexcept = delete;
    ~hard() { }
};

int factorial(int n) {
    if (n <= 0) return 1;
    return factorial(n - 1) * n;
}

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
