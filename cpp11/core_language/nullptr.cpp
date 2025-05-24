// nullptr.cpp
#include <iostream>
#include <cstddef> // For NULL (though nullptr is preferred) and std::nullptr_t

// Function overloads to demonstrate ambiguity with NULL
void foo(int n) {
    std::cout << "foo(int) called with: " << n << std::endl;
}

void foo(char* s) {
    std::cout << "foo(char*) called with: " << (s ? s : "null pointer") << std::endl;
}

// Overload for std::nullptr_t
void foo(std::nullptr_t ptr) {
    std::cout << "foo(std::nullptr_t) called" << std::endl;
}

int main() {
    int* p1 = nullptr;       // OK: p1 is a null pointer
    double* p2 = nullptr;    // OK: p2 is a null pointer
    char* p3 = nullptr;      // OK: p3 is a null pointer
    // int val = nullptr;    // Error: nullptr is not an integer

    if (p1 == nullptr) {
        std::cout << "p1 is a null pointer." << std::endl;
    }

    // Demonstrating ambiguity with NULL (which is often just 0)
    // foo(NULL); // This line can be ambiguous and might call foo(int) or cause a compile error
               // depending on the definition of NULL (0 or (void*)0).
               // If NULL is 0, foo(int) is called.
               // If NULL is (void*)0, it might require a cast or be ambiguous.

    std::cout << "Calling foo with nullptr:" << std::endl;
    foo(nullptr); // Clearly calls foo(std::nullptr_t) if available,
                  // or foo(char*) due to pointer conversion rules.

    std::cout << "Calling foo with (char*)nullptr explicitly for pointer version:" << std::endl;
    foo((char*)nullptr); // Explicitly calls foo(char*)

    // Comparing nullptr with other pointer types
    if (p1 == p2) { // Comparing an int* and double* that are both nullptr
        std::cout << "p1 (int* null) and p2 (double* null) are considered equal when null." << std::endl;
    }

    std::nullptr_t my_null = nullptr; // std::nullptr_t is the type of nullptr
    if (my_null == nullptr) {
        std::cout << "my_null (of type std::nullptr_t) is equal to nullptr." << std::endl;
    }
    
    // foo(my_null); // Calls foo(std::nullptr_t)

    return 0;
}

/*
Explanation:
`nullptr` is a keyword introduced in C++11 that represents a null pointer constant.
It is designed to replace the macro `NULL` (which is typically defined as 0 or `(void*)0`).

Problems with NULL:
1. Ambiguity: `NULL` can be ambiguous in function overloading because it can be
   treated as an integer (0) or a pointer (`(void*)0`). This can lead to
   calling the wrong overloaded function.
   Example: `foo(NULL);` could call `foo(int)` if `NULL` is `0`.
2. Type Safety: `nullptr` has a specific type, `std::nullptr_t`, which improves
   type safety. `std::nullptr_t` is implicitly convertible to any pointer type,
   but not to integral types (except bool, where it converts to false).

Benefits of nullptr:
- Unambiguous: `nullptr` clearly indicates a pointer context, resolving ambiguities
  in function overloading. `foo(nullptr);` will prefer pointer overloads or
  overloads specifically for `std::nullptr_t`.
- Type Safe: It prevents accidental conversion to non-pointer types.
- Improved Readability: `nullptr` explicitly states that a null pointer is intended.

`std::nullptr_t` is the type of the `nullptr` literal. It's a distinct type that
can hold only the value `nullptr`.

How to compile:
g++ -std=c++11 nullptr.cpp -o nullptr_example
./nullptr_example
*/
