// generic_lambdas.cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For std::sort
#include <utility>   // For std::forward

struct Point {
    int x, y;
};

int main() {
    // --- 1. Basic Generic Lambda ---
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << "--- Basic Generic Lambda ---" << std::endl;
    std::cout << "add(5, 3) = " << add(5, 3) << std::endl;
    std::cout << "add(3.14, 2.71) = " << add(3.14, 2.71) << std::endl;
    std::cout << "add(std::string("Hello, "), std::string("World!")) = "
              << add(std::string("Hello, "), std::string("World!")) << std::endl;
    std::cout << "add(5, 3.14) (int, double) = " << add(5, 3.14) << std::endl;

    // --- 2. Generic Lambda with explicit return type ---
    auto multiply = [](auto a, auto b) -> double {
        return static_cast<double>(a * b);
    };
    std::cout << "\n--- Generic Lambda with explicit return type ---" << std::endl; // Corrected newline
    std::cout << "multiply(4, 5) = " << multiply(4, 5) << std::endl;
    std::cout << "multiply(2.5, 3) = " << multiply(2.5, 3) << std::endl;

    // --- 3. Using generic lambdas with STL algorithms ---
    std::cout << "\n--- Generic Lambdas with STL Algorithms ---" << std::endl; // Corrected newline
    std::vector<Point> points = {{1, 5}, {3, 2}, {0, 8}};

    std::sort(points.begin(), points.end(), [](const auto& p1, const auto& p2) {
        return p1.x < p2.x;
    });

    std::cout << "Points sorted by x: ";
    for (const auto& p : points) {
        std::cout << "{" << p.x << "," << p.y << "} ";
    }
    std::cout << std::endl;

    std::sort(points.begin(), points.end(), [](auto p1, auto p2) {
        return p1.y < p2.y;
    });
    std::cout << "Points sorted by y: ";
    for (const auto& p : points) {
        std::cout << "{" << p.x << "," << p.y << "} ";
    }
    std::cout << std::endl;

    // --- 4. Generic Lambdas and Perfect Forwarding (more advanced) ---
    std::cout << "\n--- Generic Lambdas and Perfect Forwarding ---" << std::endl; // Corrected newline
    
    auto call_with_args = [](auto&& func, auto&&... args) {
        std::cout << "Calling function with forwarded arguments..." << std::endl;
        return std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...);
    };

    auto sample_func = [](int i, const std::string& s) {
        // Corrected the string literal here
        std::cout << "sample_func called with int: " << i << " and string: \"" << s << "\"" << std::endl; // Corrected quotes
        return i + s.length();
    };

    int x = 10;
    std::string str = "test";
    auto result = call_with_args(sample_func, x, str);
    std::cout << "Result from call_with_args: " << result << std::endl;

    return 0;
}

/*
Explanation:
Generic Lambdas were introduced in C++14. They allow the use of `auto` in
the parameter list of a lambda expression. This makes the lambda behave like a
function template, where the types of the parameters are deduced by the compiler
when the lambda is called.

Syntax:
`[](auto param1, auto param2, ...)`

Benefits:
1.  Versatility: A single generic lambda can operate on arguments of different
    types, reducing code duplication. For example, a lambda to add two numbers
    can work for `int`, `double`, `float`, etc., without needing separate
    overloads or templates.
2.  Conciseness: Simplifies writing lambdas that need to handle various related
    types, especially in conjunction with STL algorithms.
3.  Template-like Behavior: The lambda effectively becomes a function object with
    a templated `operator()`.

Return Type Deduction:
-   Just like in C++11 lambdas, the return type of a generic lambda is deduced
    from its `return` statements if they are consistent.
-   If needed, an explicit trailing return type can be specified:
    `[](auto x) -> double { return x * 0.5; }`

Use with STL Algorithms:
Generic lambdas are very convenient with STL algorithms like `std::sort`,
`std::transform`, etc., as they can adapt to the types of elements in
different containers.

Perfect Forwarding in Generic Lambdas:
For more advanced scenarios, generic lambdas can use `auto&&` for their parameters
to deduce them as forwarding references (universal references). This, combined
with `std::forward`, allows the lambda to pass its arguments to another function
while preserving their value categories (lvalue/rvalue). This is useful when
writing wrapper lambdas or higher-order functions.

Example:
auto f = [](auto x, auto y) { return x + y; };
f(1, 2);       // x, y are int
f(1.5, 2.5);   // x, y are double
f(std::string("a"), std::string("b")); // x, y are std::string

How to compile:
g++ -std=c++14 generic_lambdas.cpp -o generic_lambdas_example
./generic_lambdas_example
*/
