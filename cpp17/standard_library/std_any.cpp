// std_any.cpp
#include <iostream>
#include <any>      // Key header for std::any
#include <string>
#include <vector>
#include <typeinfo> // For typeid

struct MyCustomType {
    int value;
    std::string name;

    MyCustomType(int v, std::string n) : value(v), name(std::move(n)) {}

    void print() const {
        std::cout << "MyCustomType { value: " << value << ", name: \"" << name << "\" }" << std::endl;
    }
};

// Non-member function to print MyCustomType, for consistency
std::ostream& operator<<(std::ostream& os, const MyCustomType& mct) {
    os << "MyCustomType(value=" << mct.value << ", name=\"" << mct.name << "\")"; // Corrected quotes
    return os;
}


int main() {
    std::cout << "--- std::any (C++17) ---" << std::endl;

    // 1. Creating and assigning to std::any
    std::cout << "\n1. Creating and assigning:" << std::endl; // Corrected newline formatting
    std::any a; 

    std::cout << "a.has_value() initially: " << std::boolalpha << a.has_value() << std::endl;

    a = 10; 
    std::cout << "a holds int: " << std::any_cast<int>(a) << " (type: " << a.type().name() << ")" << std::endl;

    a = std::string("Hello Any!"); 
    std::cout << "a holds string: \"" << std::any_cast<const std::string&>(a) << "\"" << " (type: " << a.type().name() << ")" << std::endl;

    a = 3.14f; 
    std::cout << "a holds float: " << std::any_cast<float>(a) << " (type: " << a.type().name() << ")" << std::endl;

    MyCustomType custom_obj(123, "TestObject");
    a = custom_obj; 
    std::cout << "a holds MyCustomType. Type from typeid: " << a.type().name() << std::endl;
    std::any_cast<const MyCustomType&>(a).print();


    // 2. Checking if std::any holds a value and what type
    std::cout << "\n2. Checking value and type:" << std::endl; // Corrected newline formatting
    if (a.has_value()) {
        std::cout << "a contains a value." << std::endl;
        if (a.type() == typeid(MyCustomType)) {
            std::cout << "a currently holds MyCustomType." << std::endl;
        } else if (a.type() == typeid(std::string)) {
            std::cout << "a currently holds std::string." << std::endl;
        } else {
            std::cout << "a holds type: " << a.type().name() << std::endl;
        }
    }

    // 3. Accessing the contained value with std::any_cast
    std::cout << "\n3. Accessing with std::any_cast:" << std::endl; // Corrected newline formatting
    a = 42; 

    try {
        int int_val = std::any_cast<int>(a);
        std::cout << "Successfully cast to int: " << int_val << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << "Caught bad_any_cast for int: " << e.what() << std::endl;
    }

    try {
        int& int_ref = std::any_cast<int&>(a); 
        int_ref = 43; 
        std::cout << "Successfully cast to int&, modified to: " << std::any_cast<int>(a) << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << "Caught bad_any_cast for int&: " << e.what() << std::endl;
    }
    
    try {
        const int& const_int_ref = std::any_cast<const int&>(a);
        std::cout << "Successfully cast to const int&: " << const_int_ref << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << "Caught bad_any_cast for const int&: " << e.what() << std::endl;
    }

    try {
        std::cout << "Trying to cast int to double: ";
        double d_val = std::any_cast<double>(a); 
        std::cout << d_val << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << "Caught bad_any_cast: " << e.what() << std::endl;
    }

    double* ptr_double = std::any_cast<double>(&a); 
    if (ptr_double) {
        std::cout << "Pointer cast to double succeeded (value: " << *ptr_double << ")" << std::endl;
    } else {
        std::cout << "Pointer cast to double failed (returned nullptr), any holds: " << a.type().name() << std::endl;
    }

    int* ptr_int = std::any_cast<int>(&a); 
    if (ptr_int) {
        std::cout << "Pointer cast to int succeeded (value: " << *ptr_int << ")" << std::endl;
        *ptr_int = 44; 
        std::cout << "Value in 'a' after modification via ptr_int: " << std::any_cast<int>(a) << std::endl;
    } else {
        std::cout << "Pointer cast to int failed." << std::endl;
    }

    // 4. reset() and emplace()
    std::cout << "\n4. reset() and emplace():" << std::endl; // Corrected newline formatting
    std::cout << "a.has_value() before reset: " << a.has_value() << std::endl;
    a.reset(); 
    std::cout << "a.has_value() after reset: " << a.has_value() << std::endl;

    a.emplace<MyCustomType>(789, "Emplaced Object");
    std::cout << "a after emplace<MyCustomType>: ";
    std::any_cast<MyCustomType&>(a).print();

    // 5. Storing in a container
    std::cout << "\n5. Storing in a container:" << std::endl; // Corrected newline formatting
    std::vector<std::any> any_vector;
    any_vector.push_back(100);
    any_vector.push_back(std::string("Vector Element"));
    any_vector.push_back(MyCustomType(55, "InVector"));
    any_vector.emplace_back(std::in_place_type<MyCustomType>, 66, "EmplacedInVector");

    for (const auto& item : any_vector) {
        if (item.type() == typeid(int)) {
            std::cout << "Vector item - int: " << std::any_cast<int>(item) << std::endl;
        } else if (item.type() == typeid(std::string)) {
            std::cout << "Vector item - string: \"" << std::any_cast<const std::string&>(item) << "\"" << std::endl;
        } else if (item.type() == typeid(MyCustomType)) {
            std::cout << "Vector item - ";
            std::any_cast<const MyCustomType&>(item).print();
        } else {
            std::cout << "Vector item - unknown type: " << item.type().name() << std::endl;
        }
    }

    std::cout << "\nstd::any example finished." << std::endl; // Corrected newline formatting
    return 0;
}

/*
Explanation:
`std::any` (from the `<any>` header) is a C++17 feature that provides a
type-safe container for single values of any copy-constructible type.
It allows you to store values of different types in a way that the type can be
retrieved or checked at runtime.

Key Features:

1.  Holding Values:
    -   An `std::any` object can be empty or can hold a value of some type.
    -   When a value is assigned to `std::any`, `std::any` makes a copy of that value
        (or moves from it if it's an rvalue). The stored object is dynamically
        allocated if necessary (small objects might be stored internally using
        small object optimization - SOO).

2.  Creation and Assignment:
    -   `std::any a;` (empty)
    -   `a = 10;` (holds an int)
    -   `a = std::string("hello");` (holds a string)
    -   `a.emplace<MyType>(args...);` (constructs MyType in-place)

3.  Checking State:
    -   `a.has_value()`: Returns `true` if `a` holds a value, `false` if empty.
    -   `a.type()`: Returns `std::type_info` object for the contained type.
        If empty, it returns `typeid(void)`.

4.  Accessing the Value (`std::any_cast`):
    -   `std::any_cast<T>(a)`:
        -   If `a` holds a `T`, returns a copy of the value.
        -   If `a` does not hold `T`, throws `std::bad_any_cast`.
    -   `std::any_cast<T&>(a)` or `std::any_cast<const T&>(a)`:
        -   If `a` holds `T`, returns a reference (or const reference) to the value.
        -   If `a` does not hold `T`, throws `std::bad_any_cast`.
    -   `std::any_cast<T>(&a)`: (Note: pointer to `a`)
        -   If `a` holds `T`, returns a pointer to `T` (`T*`).
        -   If `a` does not hold `T`, returns `nullptr`. This version does not throw.

5.  `reset()`:
    -   `a.reset()`: Destroys the contained object (if any) and makes `a` empty.

Use Cases:
-   Storing heterogeneous collections where types are not known until runtime.
-   Interfacing with dynamic languages or systems where types can vary.
-   Implementing generic data structures or message passing systems.

`std::any` vs. `std::variant`:
-   `std::variant<T1, T2, ...>`: Type-safe union over a *closed* (compile-time known)
    set of types. More type information is available at compile time. Usually preferred
    if the set of possible types is fixed and known.
-   `std::any`: Can hold (almost) *any* type. The set of types is open. Type checking
    is primarily a runtime operation via `any_cast` and `type()`.

Considerations:
-   Performance: `std::any` might involve dynamic memory allocation for types that
    don't fit its internal small object optimization buffer. Type checking and
    casting also have runtime overhead.
-   Type Erasure: `std::any` erases the static type information, relying on runtime
    checks. This can make code harder to reason about compared to `std::variant` or
    templates if static type safety is paramount.
-   The contained type must be CopyConstructible. C++20 relaxes some of these constraints
    for `std::any`'s `emplace` and constructor with `std::in_place_type`.

How to compile:
g++ -std=c++17 std_any.cpp -o std_any_example
./std_any_example
*/
