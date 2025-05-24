// auto.cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    // Demonstrating 'auto' for type inference

    auto i = 42;          // i is inferred as int
    auto d = 3.14;        // d is inferred as double
    auto s = "hello";     // s is inferred as const char*
    auto b = true;        // b is inferred as bool

    std::cout << "i (int): " << i << std::endl;
    std::cout << "d (double): " << d << std::endl;
    std::cout << "s (const char*): " << s << std::endl;
    std::cout << "b (bool): " << std::boolalpha << b << std::endl;

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto it = numbers.begin(); // it is inferred as std::vector<int>::iterator
    std::cout << "First number in vector: " << *it << std::endl;

    auto sum_lambda = [](int a, int b) { return a + b; };
    auto result = sum_lambda(5, 3); // result is inferred as int
    std::cout << "Lambda result (int): " << result << std::endl;
    
    // Note: 'auto' infers the type from the initializer.
    // It doesn't change type dynamically.
    // For example, the following would be a compile error after uncommenting:
    // i = 3.14159; // Error: i is an int, cannot assign double

    return 0;
}

/*
Explanation:
The 'auto' keyword, introduced in C++11, allows the compiler to automatically
deduce the type of a variable from its initializer. This can make code
shorter, easier to read, and less prone to type-related errors, especially
when dealing with complex types like iterators or lambda expressions.

How to compile:
g++ -std=c++11 auto.cpp -o auto_example
./auto_example
*/
