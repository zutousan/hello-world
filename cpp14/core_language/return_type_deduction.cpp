// return_type_deduction.cpp
#include <iostream>
#include <string>
#include <vector> // Required for one of the examples

// 1. Basic return type deduction
auto add(int a, int b) {
    return a + b;
}

auto concatenate(const std::string& s1, const std::string& s2) {
    return s1 + s2;
}

auto multiply(int x, double y) {
    return x * y;
}

// 2. Return type deduction with multiple return statements
auto get_value(bool condition, int x) {
    if (condition) {
        return x;
    } else {
        return 0;
    }
}

// 3. Return type deduction with recursion
auto factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// 4. Using `decltype(auto)` for more precise deduction
int global_var = 100;
int& get_global_ref() { return global_var; }

auto returns_copy() {
    return get_global_ref();
}

decltype(auto) returns_ref() {
    return get_global_ref();
}

const int const_global_var = 200;
const int& get_const_global_ref() { return const_global_var; }

auto returns_const_copy() {
    return get_const_global_ref();
}

decltype(auto) returns_const_ref() {
    return get_const_global_ref();
}

std::vector<int> my_vector = {1,2,3,4,5};

decltype(auto) get_element_ref(std::vector<int>& vec, size_t index) {
    return vec[index];
}

auto get_element_copy(std::vector<int>& vec, size_t index) {
    return vec[index];
}

int main() {
    std::cout << "--- Basic Return Type Deduction ---" << std::endl;
    std::cout << "add(5, 3) = " << add(5, 3) << std::endl;
    std::cout << "concatenate(\"C++\", \"14\") = " << concatenate("C++", "14") << std::endl;
    std::cout << "multiply(4, 2.5) = " << multiply(4, 2.5) << std::endl;
    std::cout << "get_value(true, 10) = " << get_value(true, 10) << std::endl;
    std::cout << "get_value(false, 10) = " << get_value(false, 10) << std::endl;
    std::cout << "factorial(5) = " << factorial(5) << std::endl;

    // Corrected newline formatting in the following cout statement
    std::cout << "\n--- decltype(auto) for Precise Deduction ---" << std::endl; // MANUALLY CORRECTED
    
    int r_copy = returns_copy();
    std::cout << "returns_copy() (original global_var): " << global_var << ", copy: " << r_copy << std::endl;
    
    int& r_ref = returns_ref();
    r_ref = 150;
    std::cout << "returns_ref() changed global_var to: " << global_var << std::endl;
    global_var = 100; // Reset

    int rc_copy = returns_const_copy();
    std::cout << "returns_const_copy() (original const_global_var): " << const_global_var << ", copy: " << rc_copy << std::endl;
    
    const int& rc_ref = returns_const_ref();
    std::cout << "returns_const_ref() value: " << rc_ref << std::endl;

    // Corrected newline formatting in the following cout statement
    std::cout << "\n--- decltype(auto) with std::vector element ---" << std::endl; // MANUALLY CORRECTED
    int& element_ref = get_element_ref(my_vector, 0);
    int element_copy = get_element_copy(my_vector, 1);

    std::cout << "Original my_vector[0]: " << my_vector[0] << std::endl;
    element_ref = 99;
    std::cout << "Modified my_vector[0] via reference: " << my_vector[0] << std::endl;

    std::cout << "Original my_vector[1]: " << my_vector[1] << std::endl;
    element_copy = 88;
    std::cout << "my_vector[1] after modifying copy: " << my_vector[1] << std::endl;
    std::cout << "Value of copy: " << element_copy << std::endl;

    return 0;
}

/*
Explanation:
C++14 introduced function return type deduction. This allows the compiler to
deduce the return type of a function automatically from its `return` statements,
so you don't always have to specify it explicitly.

Syntax:
Use `auto` as the return type specifier:
`auto function_name(parameters) { ... return expression; ... }`

Rules and Behavior:

1.  Single Return Statement:
    If the function has only one `return expression;`, the type of `expression`
    becomes the function's return type (after normal decay rules, similar to
    `auto` variable type deduction - e.g., references are removed, const/volatile
    is stripped unless it's a reference to const).

2.  Multiple Return Statements:
    If there are multiple `return` statements, they must all deduce to the
    *exact same type*. If there's any inconsistency, it results in a
    compile-time error.

3.  Recursive Functions:
    For a recursive function, at least one `return` statement must allow the
    type to be deduced without a recursive call (typically a base case).
    All other `return` statements (including recursive ones) must be consistent
    with this deduced type.

4.  `decltype(auto)` for Return Types:
    -   Using just `auto` for return type deduction follows template type deduction
        rules, which means it always deduces by value (stripping references and
        top-level cv-qualifiers).
    -   C++14 also allows `decltype(auto)` as a return type specifier.
        When `decltype(auto)` is used, the type is deduced using the rules of
        `decltype` on the returned expression. This is more precise and can
        deduce reference types (`T&` or `T&&`) and preserve const/volatile
        qualifiers.

Benefits:
-   Conciseness: Reduces verbosity.
-   Maintainability: If the type of the returned expression changes, the function
    signature might not need to be updated manually.

Drawbacks:
-   Readability: Can sometimes make it harder for readers to immediately know
    the function's return type.
-   Interface Stability: The return type becomes part of the function's interface.
    If it changes due to an implementation detail, it can break client code.

Use `auto` when the return type is obvious or internal to an implementation.
Use `decltype(auto)` when you need to preserve reference and cv-qualifiers of the returned expression.
For library interfaces, an explicit return type is often still the best practice.

How to compile:
g++ -std=c++14 return_type_deduction.cpp -o return_type_deduction_example
./return_type_deduction_example
*/
