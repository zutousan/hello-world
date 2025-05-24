// range_based_for_loop.cpp
#include <iostream>
#include <vector>
#include <string>
#include <map>

int main() {
    // 1. Iterating over a std::vector
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "Iterating over a vector: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // 2. Modifying elements in a std::vector using references
    std::cout << "Modifying vector elements (doubling): ";
    for (int& n : numbers) {
        n *= 2;
    }
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // 3. Iterating over an array
    int arr[] = {10, 20, 30, 40, 50};
    std::cout << "Iterating over an array: ";
    for (int x : arr) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // 4. Iterating over a std::string
    std::string message = "Hello";
    std::cout << "Iterating over a string: ";
    for (char c : message) {
        std::cout << c << "-";
    }
    std::cout << std::endl;

    // 5. Iterating over an initializer list
    std::cout << "Iterating over an initializer list: ";
    for (int val : {100, 200, 300}) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 6. Iterating over a std::map (accessing key-value pairs)
    std::map<std::string, int> scores = {
        {"Alice", 90},
        {"Bob", 85},
        {"Charlie", 92}
    };
    std::cout << "Iterating over a map:" << std::endl;
    for (const auto& pair : scores) { // Use 'auto&' or 'const auto&' to avoid copying pairs
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Using structured bindings (C++17) with range-based for (though this file is C++11 focus)
    // For C++11, you'd use pair.first and pair.second as above.
    // If C++17 were allowed here, it would be:
    // for (auto const& [name, score] : scores) {
    //     std::cout << name << ": " << score << std::endl;
    // }

    return 0;
}

/*
Explanation:
The range-based for loop, introduced in C++11, provides a simpler and more
readable way to iterate over elements in a range, such as containers
(e.g., std::vector, std::map, std::string), arrays, initializer lists, or
any object that defines begin() and end() methods that return iterators
(or for which global begin() and end() are defined).

Syntax:
for (declaration : range_expression) {
    // loop body
}

- declaration: Declares a variable that will take the value of each element
  in the range during iteration.
  - To read elements: `for (type var : range)` or `for (const type& var : range)`
  - To modify elements (if the container and its elements are modifiable):
    `for (type& var : range)`
  - `auto` can be used for type deduction: `for (auto var : range)`,
    `for (const auto& var : range)`, `for (auto& var : range)`
- range_expression: The collection or sequence to iterate over.

Benefits:
- More concise and less error-prone than traditional index-based or
  iterator-based for loops.
- Improves code readability.

How to compile:
g++ -std=c++11 range_based_for_loop.cpp -o range_for_example
./range_for_example
*/
