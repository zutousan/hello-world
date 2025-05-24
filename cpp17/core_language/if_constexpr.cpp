// if_constexpr.cpp
#include <iostream>
#include <string>
#include <vector>
#include <type_traits> // For std::is_integral, std::is_pointer, etc.
#include <tuple>       // For std::tuple_size, std::get, std::make_tuple

// Helper for static_assert to make the condition dependent
template <typename T>
struct dependent_false : std::false_type {};

// 1. Basic template function where behavior depends on type properties
template<typename T>
auto process_value(T val) {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "Processing an integral type: " << val << std::endl;
        return val * 2;
    } else if constexpr (std::is_floating_point<T>::value) {
        std::cout << "Processing a floating-point type: " << val << std::endl;
        return val / 2.0;
    } else if constexpr (std::is_pointer<T>::value) {
        std::cout << "Processing a pointer type. Value: ";
        if constexpr (std::is_same<T, const char*>::value || std::is_same<T, char*>::value) {
            if (val) std::cout << "\"" << static_cast<const char*>(val) << "\""; // MANUALLY CORRECTED QUOTES
            else std::cout << "(null)";
        } else {
            std::cout << static_cast<const void*>(val); // Print address for other pointer types
        }
        std::cout << std::endl;
        return static_cast<void*>(nullptr); 
    } else {
        // This block is compiled for types not handled above
        std::cout << "Processing an other type (e.g., std::string)." << std::endl;
        return val; 
    }
}

// SFINAE helper to check for a .size() member (C++17 style)
template<typename T, typename = void>
struct has_size_member : std::false_type {};
template<typename T>
struct has_size_member<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

template<typename T>
size_t get_length(const T& collection) {
    if constexpr (std::is_same<T, std::string>::value) {
        return collection.length();
    } else if constexpr (std::is_same<T, const char*>::value || std::is_same<T, char*>::value) {
         return collection ? std::string(collection).length() : 0; // Handle null C-string
    } else if constexpr (has_size_member<T>::value) {
        // Check if T has a .size() member function.
        std::cout << "(Type " << typeid(T).name() << " has .size())" << std::endl;
        return collection.size();
    } else {
        // Corrected static_assert: make it dependent on T to ensure it only fires
        // if this path is actually taken for an unsupported type.
        static_assert(dependent_false<T>::value, "get_length not supported for this type");
        return 0; // Unreachable if static_assert fires
    }
}

// 3. Recursive templates and if constexpr
template<size_t N, typename TupleType>
void print_tuple_elements(const TupleType& tpl) {
    if constexpr (N < std::tuple_size<TupleType>::value) {
        std::cout << "Element " << N << ": " << std::get<N>(tpl) << std::endl;
        print_tuple_elements<N + 1>(tpl); 
    }
}


int main() {
    std::cout << "--- process_value function ---" << std::endl;
    process_value(10);
    process_value(3.14f);
    const char* cstr = "hello";
    process_value(cstr);
    int x = 5;
    int* ptr_x = &x;
    process_value(ptr_x);
    process_value(std::string("world")); 

    std::cout << "\n--- get_length function ---" << std::endl; // MANUALLY CORRECTED NEWLINE
    std::string my_string = "Test String";
    std::vector<int> my_vector = {1, 2, 3, 4, 5};
    const char* my_c_string = "C-Style";

    std::cout << "Length of string \"" << my_string << "\": " << get_length(my_string) << std::endl; // MANUALLY CORRECTED QUOTES
    std::cout << "Length of C-string \"" << my_c_string << "\": " << get_length(my_c_string) << std::endl; // MANUALLY CORRECTED QUOTES
    std::cout << "Length of vector: " << get_length(my_vector) << std::endl;
    
    // The following would trigger the static_assert if uncommented:
    // struct NoSize {}; NoSize ns;
    // std::cout << "Length of NoSize: " << get_length(ns) << std::endl; 
    // std::cout << "Length of int: " << get_length(5) << std::endl;


    std::cout << "\n--- print_tuple_elements (recursive template) ---" << std::endl; // MANUALLY CORRECTED NEWLINE
    auto myTuple = std::make_tuple(100, std::string("Tuple Test"), 2.718);
    print_tuple_elements<0>(myTuple);

    return 0;
}

/*
Explanation:
`if constexpr (condition)` is a C++17 feature that allows for compile-time
conditional compilation of code within a function template or lambda.

Key Characteristics:
1.  Compile-Time Evaluation: The `condition` must be a constant expression.
    The compiler discards the code block of the branch not taken.
2.  No Short-Circuiting (for compilation): Both branches must be syntactically
    valid in the template context, but only the "true" branch needs to be
    semantically valid for the specific template instantiation.
3.  Use in Templates: Simplifies template metaprogramming tasks (e.g., SFINAE,
    tag dispatching, specializations) by allowing different code paths based on
    template parameters (e.g., type traits).
4.  `static_assert(dependent_false<T>::value, "message")`: When using static_assert
    in a discarded branch of `if constexpr`, the condition must be dependent on a
    template parameter `T` to ensure it's only evaluated if that branch is
    instantiated. A simple `static_assert(false, ...)` would make the template
    ill-formed.

Benefits:
-   Readability for conditional compilation logic in templates.
-   Reduced boilerplate code compared to multiple specializations/overloads.
-   Improved error messages (errors in discarded branches usually not reported).

How to compile:
g++ -std=c++17 if_constexpr.cpp -o if_constexpr_example
./if_constexpr_example
*/
