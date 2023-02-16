/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <iostream>

class A {
 public:
    A() { std::cout << "A::A()" << std::endl; }
    ~A() { std::cout << "A::~A()" << std::endl; }
};

int main() {
    A a;
    return 0;
}
