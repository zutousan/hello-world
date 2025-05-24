// concepts.cpp
#include <iostream>
#include <vector>
#include <string>
#include <limits>   // For std::numeric_limits
#include <concepts> // Key header for concepts
#include <list>     // For a non-random access container example

// --- 1. Defining a simple concept ---
// A concept is a compile-time predicate that constrains template parameters.
// `Integral` is a concept that checks if a type T is an integral type.
template<typename T>
concept Integral = std::is_integral_v<T>; // std::is_integral_v is a C++17 helper

// Using the concept with a template function
template<Integral T> // T must satisfy the Integral concept
T add(T a, T b) {
    return a + b;
}

// Another way to use a concept: trailing requires clause
template<typename T>
T subtract(T a, T b) requires Integral<T> { // T must satisfy Integral
    return a - b;
}

// Yet another way: constrained auto (for function parameters or variables)
// This is more common for generic lambdas or very generic functions.
// void print_integral(Integral auto val) { // `Integral auto` is a C++20 feature
//    std::cout << "Integral value: " << val << std::endl;
// }
// For a template function, the above is equivalent to:
template<Integral T>
void print_integral(T val) {
    std::cout << "Integral value: " << val << std::endl;
}


// --- 2. Combining concepts ---
template<typename T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

template<SignedIntegral T>
T negate_val(T val) { // Renamed from 'negate' to avoid conflict if any std::negate is brought in
    return -val;
}

template<typename T>
concept UnsignedIntegral = Integral<T> && !std::is_signed_v<T>; // or std::is_unsigned_v

template<UnsignedIntegral T>
T safe_double_val(T val) { // Renamed from 'safe_double'
    if (val > (std::numeric_limits<T>::max() / 2)) {
        return std::numeric_limits<T>::max();
    }
    return val * 2;
}


// --- 3. Concept requiring specific operations (syntax with `requires` expression) ---
// This concept checks if a type T supports equality comparison (==) and
// inequality comparison (!=) that return something convertible to bool.
template<typename T>
concept EqualityComparable = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>; // Check if a == b compiles and result is convertible to bool
    { a != b } -> std::convertible_to<bool>; // Check if a != b compiles and result is convertible to bool
};

template<EqualityComparable T>
bool are_equal(T v1, T v2) {
    std::cout << "Checking equality for EqualityComparable types." << std::endl;
    return v1 == v2;
}

struct NoCompare {}; // This type does not support == or !=

// --- 4. Concept requiring nested types or members ---
template<typename C>
concept Container = requires(C c) {
    typename C::value_type;       // Requires C to have a nested type ::value_type
    typename C::iterator;         // Requires C to have a nested type ::iterator
    { c.begin() } -> std::same_as<typename C::iterator>; // begin() returns iterator
    { c.end() }   -> std::same_as<typename C::iterator>; // end() returns iterator
    { c.size() }  -> std::convertible_to<std::size_t>; // size() returns something convertible to size_t
};

template<Container C>
void print_container_size(const C& c) {
    std::cout << "Container size: " << c.size() << std::endl;
    // Accessing value_type (example)
    using ElemType = typename C::value_type;
    if constexpr (std::is_same_v<ElemType, int>) {
        std::cout << "(Container of ints)" << std::endl;
    }
}

// --- 5. Standard Library Concepts ---
// C++20 provides many predefined concepts in <concepts> and other headers
// (e.g., <iterator>, <ranges>).
// Examples: std::integral, std::signed_integral, std::unsigned_integral,
// std::floating_point, std::same_as, std::convertible_to, std::invocable, etc.

template<std::integral T> // Using standard concept directly
T multiply_std_integral(T a, T b) {
    return a * b;
}

template<typename T>
    requires std::floating_point<T> // Using standard concept in requires clause
T divide_std_fp(T a, T b) {
    if (b == 0) return std::numeric_limits<T>::quiet_NaN();
    return a / b;
}


int main() {
    std::cout << "--- Concepts (C++20) ---" << std::endl;

    std::cout << "\n1. Basic Integral concept:" << std::endl;
    std::cout << "add(5, 3) = " << add(5, 3) << std::endl;
    // std::cout << "add(5.5, 3.3) = " << add(5.5, 3.3) << std::endl; // Compile error: double is not Integral
    print_integral(100);
    // print_integral("hello"); // Compile error: const char* is not Integral

    std::cout << "subtract(10, 7) = " << subtract(10, 7) << std::endl;

    std::cout << "\n2. Combined concepts (SignedIntegral, UnsignedIntegral):" << std::endl;
    std::cout << "negate_val(5) = " << negate_val(5) << std::endl;
    // std::cout << "negate_val(5u) = " << negate_val(5u) << std::endl; // Compile error: unsigned int is not SignedIntegral
    unsigned int u_val = 100;
    std::cout << "safe_double_val(" << u_val << "u) = " << safe_double_val(u_val) << "u" << std::endl;
    u_val = std::numeric_limits<unsigned int>::max() - 10;
    std::cout << "safe_double_val(" << u_val << "u) = " << safe_double_val(u_val) << "u" << std::endl;


    std::cout << "\n3. EqualityComparable concept:" << std::endl;
    std::cout << "are_equal(10, 10): " << std::boolalpha << are_equal(10, 10) << std::endl;
    std::cout << "are_equal(std::string(\"abc\"), std::string(\"def\")): "
              << are_equal(std::string("abc"), std::string("def")) << std::endl;
    // are_equal(NoCompare{}, NoCompare{}); // Compile error: NoCompare does not satisfy EqualityComparable

    std::cout << "\n4. Container concept:" << std::endl;
    std::vector<int> my_vec = {1, 2, 3};
    std::list<std::string> my_list = {"a", "b"};
    print_container_size(my_vec);
    print_container_size(my_list);
    // struct NotAContainer {};
    // print_container_size(NotAContainer{}); // Compile error

    std::cout << "\n5. Standard Library concepts:" << std::endl;
    std::cout << "multiply_std_integral(6, 7) = " << multiply_std_integral(6, 7) << std::endl;
    std::cout << "divide_std_fp(10.0, 4.0) = " << divide_std_fp(10.0, 4.0) << std::endl;
    // divide_std_fp(10, 4); // Compile error: int is not floating_point


    std::cout << "\nConcepts provide clearer template error messages and allow for more expressive template constraints." << std::endl;

    return 0;
}

/*
Explanation:
Concepts are a C++20 feature that allows developers to specify constraints on
template parameters. They act as compile-time predicates (true or false) that
determine if a given type (or set of types) meets certain requirements.
If a type used with a constrained template does not satisfy the concept, the
compiler issues an error, often much clearer than traditional SFINAE errors.

Key Header: `<concepts>` (for standard library concepts, defining your own doesn't strictly need it but it's good practice)
             `<type_traits>` (often used for concept definitions, e.g. std::is_integral_v)
             `<limits>` (used in an example)

Defining Concepts:
- A concept is defined using the `concept` keyword followed by a name and a
  boolean constant expression involving template parameters.
  ```cpp
  template<typename T>
  concept MyConcept = /* boolean expression involving T */;
  ```
- The boolean expression often uses type traits (from `<type_traits>`) or
  `requires` expressions.

`requires` Expressions:
- A `requires` expression allows you to specify syntactic requirements that a
  type must satisfy.
  ```cpp
  template<typename T>
  concept Addable = requires(T a, T b) { // requires parameters a and b of type T
      { a + b } -> std::convertible_to<T>; // The expression a + b must be valid,
                                          // and its result must be convertible to T.
      // Other requirements can be listed here:
      // typename T::value_type; // Requires a nested type
      // { obj.member_function() } -> std::same_as<int>; // Requires a member function returning int
  };
  ```
-   Types of requirements in a `requires` expression:
    1.  Simple requirements: `expr;` (asserts `expr` is valid).
    2.  Type requirements: `typename T::nested_type;` (asserts nested type exists).
    3.  Compound requirements: `{ expression } noexcept -> ConceptType;`
        (Asserts `expression` is valid, optionally `noexcept`, and its result type
        satisfies `ConceptType` (e.g., `std::same_as<int>`, `std::convertible_to<bool>`)).
    4.  Nested requirements: `requires ConceptName<T>;` (asserts `T` satisfies another concept).


Using Concepts:
Concepts can be used to constrain template parameters in several ways:

1.  As a type constraint directly on a template parameter:
    `template<MyConcept T> void func(T arg);`
    `template<typename T> requires MyConcept<T> void func(T arg);` (trailing requires clause)
    `template<typename T> void func(T arg) requires MyConcept<T>;` (function requires clause - less common for simple cases)

2.  With `auto` (Constrained `auto`):
    `MyConcept auto my_var = ...;`
    `void func(MyConcept auto arg);` (abbreviated function template)

3.  In `requires` clauses for more complex logical combinations:
    `template<typename T, typename U> requires MyConcept<T> && OtherConcept<U> void func(T t, U u);`

Standard Library Concepts:
C++20 introduces a rich set of predefined concepts in headers like `<concepts>`,
`<iterator>`, `<ranges>`, etc. Examples include:
- `std::integral`, `std::signed_integral`, `std::unsigned_integral`
- `std::floating_point`
- `std::same_as<T, U>`, `std::convertible_to<From, To>`
- `std::equality_comparable`, `std::totally_ordered`
- `std::invocable`, `std::predicate`
- Iterator concepts: `std::input_iterator`, `std::forward_iterator`, etc.
- Range concepts: `std::ranges::range`, `std::ranges::view`, etc.

Benefits of Concepts:
1.  Improved Readability: Template constraints are expressed directly and clearly.
2.  Better Error Messages: When a type doesn't meet the requirements, compilers
    can provide much more precise and understandable error messages, pointing to
    the specific concept or requirement that failed. This is a major improvement
    over the often cryptic error messages from SFINAE-based constraints.
3.  Easier Generic Programming: Simplifies writing and understanding generic code.
4.  Compile-Time Checks: Enforces constraints at compile time, catching errors earlier.
5.  Foundation for Ranges: Concepts are fundamental to the C++20 Ranges library.

Concepts are a powerful addition to C++ that significantly enhance the usability
and robustness of template-based programming.

How to compile:
g++ -std=c++20 concepts.cpp -o concepts_example
(or clang++ -std=c++20)
Ensure your compiler fully supports C++20 concepts.
*/
