// initializer_lists.cpp
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <initializer_list> // Required for std::initializer_list

// Class that can be initialized with an initializer list
class MyDataStructure {
public:
    std::vector<int> data_vec;
    std::string id;

    // Constructor using std::initializer_list
    MyDataStructure(std::string identifier, std::initializer_list<int> list) : id(identifier) {
        // data_vec = list; // Direct assignment from initializer_list to vector is fine
        // or iterate:
        for (int val : list) {
            data_vec.push_back(val * 10); // Example: transform data during initialization
        }
        std::cout << "MyDataStructure '" << id << "' constructed with {";
        for (size_t i = 0; i < list.size(); ++i) {
            std::cout << *(list.begin() + i) << (i == list.size() - 1 ? "" : ", ");
        }
        std::cout << "}. Stored as: ";
        print();
    }

    // Another constructor for demonstration
    MyDataStructure(std::string identifier) : id(identifier) {
        std::cout << "MyDataStructure '" << id << "' constructed (default)." << std::endl;
    }

    void print() const {
        std::cout << "'" << id << "' data_vec: [";
        for (size_t i = 0; i < data_vec.size(); ++i) {
            std::cout << data_vec[i] << (i == data_vec.size() - 1 ? "" : ", ");
        }
        std::cout << "]" << std::endl;
    }
};

// Function taking std::initializer_list
void print_list(std::initializer_list<double> list) {
    std::cout << "Printing initializer_list<double>: ";
    for (double val : list) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    // 1. Basic array initialization (pre-C++11 style, still valid)
    int arr1[] = {1, 2, 3};
    std::cout << "arr1: " << arr1[0] << std::endl;

    // 2. Uniform Initialization using braces {}
    int x = 42;       // C-style initialization
    int y(42);      // Constructor-style initialization
    int z{42};      // C++11 uniform initialization (brace-initialization)
    // int z_narrowing{3.14}; // Error or warning: narrowing conversion from double to int

    std::cout << "x=" << x << ", y=" << y << ", z=" << z << std::endl;

    std::string s1("hello");
    std::string s2{"world"}; // Uniform initialization for objects
    std::cout << "s1=" << s1 << ", s2=" << s2 << std::endl;


    // 3. Initializing STL containers
    std::vector<int> v = {10, 20, 30, 40, 50}; // Uses std::initializer_list constructor of vector
    std::cout << "Vector v initialized with {}: ";
    for (int val : v) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::map<std::string, int> capitals = {
        {"USA", 1}, // Assuming 1 for Washington D.C. for simplicity
        {"UK", 2},  // London
        {"France", 3} // Paris
        // Each inner {} is an initializer list for std::pair
    };
    std::cout << "Map capitals:" << std::endl;
    for (const auto& pair : capitals) {
        std::cout << pair.first << " -> " << pair.second << std::endl;
    }

    // 4. Using std::initializer_list in custom classes
    MyDataStructure md1("MD1", {1, 2, 3, 4});
    MyDataStructure md2("MD2", {10, 20});
    MyDataStructure md3("MD3"); // Calls different constructor

    // 5. Passing initializer lists to functions
    print_list({1.1, 2.2, 3.3});
    print_list({}); // Empty list

    // 6. Return an initializer list (less common, often a container is returned)
    // std::initializer_list<int> get_list() { return {7,8,9}; } // This is problematic due to lifetime
    // The returned initializer_list would refer to a temporary array that gets destroyed.
    // It's better to return a container like std::vector:
    std::vector<int> get_vector() { return {7, 8, 9}; }
    std::vector<int> returned_vec = get_vector();
    std::cout << "Vector from function returning {}: ";
    for (int val : returned_vec) std::cout << val << " ";
    std::cout << std::endl;


    // 7. Direct initialization vs. copy initialization with {}
    MyDataStructure direct_init("Direct", {5,5,5}); // Direct initialization
    MyDataStructure copy_init = {"Copy", {6,6,6}}; // Copy initialization (also uses initializer_list constructor)

    direct_init.print();
    copy_init.print();
    
    // Note on narrowing conversions:
    // Brace-initialization (uniform initialization) generally disallows "narrowing"
    // conversions, which can help prevent certain types of bugs.
    // int i {3.14};    // Error: narrowing conversion from double to int
    // char c {300};   // Error or warning: narrowing from int to char (if 300 is out of char range)
    
    // int i = 3.14;   // OK (value is truncated, possibly with warning)
    // char c = 300;  // OK (value is truncated/wrapped, possibly with warning)

    return 0;
}

/*
Explanation:
Initializer Lists (std::initializer_list and uniform initialization syntax {}):
C++11 introduced two related concepts: `std::initializer_list` and a unified
brace-initialization syntax `{}`.

1.  `std::initializer_list<T>`:
    -   A lightweight proxy object that provides access to an array of
        constant values of type `T`.
    -   It's typically used as a parameter in constructors or functions to allow
        them to be called with a brace-enclosed list of initializers.
    -   Example: `std::vector<int> v = {1, 2, 3};`
        The `std::vector` constructor that takes `std::initializer_list<int>`
        is called here.
    -   Members: `begin()`, `end()` (to iterate), `size()`.
    -   The lifetime of the underlying array referred to by an initializer_list
        is tied to the lifetime of the original braced-init-list. Care must be
        taken when storing an initializer_list itself (it's usually copied into
        a more permanent container).

2.  Uniform Initialization (Brace Initialization `{}`):
    -   A consistent syntax for initializing objects of any type, using braces `{}`.
    -   Aims to replace various ad-hoc initialization syntaxes (e.g., `()` for
        constructors, `=` for simple types, `=` with `{}` for aggregates).
    -   Examples:
        `int x{0};`
        `std::string s{"hello"};`
        `MyObject obj{arg1, arg2};`
        `int* p = new int[3]{1, 2, 3};`

    -   Priority: When `{}` is used for initialization:
        1.  If the type has a constructor taking `std::initializer_list`, that
            constructor is strongly preferred (even if other constructors match).
        2.  Otherwise, regular constructor overload resolution takes place for
            the elements within the braces.
        3.  If the type is an aggregate type (struct/class with no user-provided
            constructors, no private/protected non-static data members, no base
            classes, no virtual functions), aggregate initialization is performed.

    -   Narrowing Conversions: Brace-initialization generally disallows
        "narrowing" conversions (e.g., `double` to `int`, `int` to `char` if the
        value is out of range for `char`). This is a safety feature.
        `int i {3.14}; // Compile error or warning`
        `int j = 3.14; // OK, j becomes 3 (truncation)`

Benefits:
-   Consistency: Provides a more uniform way to initialize objects.
-   Readability: Clearly indicates initialization.
-   Safety: Helps prevent narrowing conversions.
-   Convenience: Easy way to initialize containers and pass lists of values.
-   Support for `std::initializer_list` allows classes to be "list-initialized"
    in a natural way.

How to compile:
g++ -std=c++11 initializer_lists.cpp -o initializer_lists_example
./initializer_lists_example
*/
