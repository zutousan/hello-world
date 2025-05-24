// nested_namespaces.cpp
#include <iostream>

// Pre-C++17 way of defining nested namespaces
namespace OldWay {
    namespace Inner {
        void greet_old() {
            std::cout << "Hello from OldWay::Inner!" << std::endl;
        }
        namespace EvenDeeper {
            const int value = 10;
        }
    }
}

// C++17 way using the nested namespace definition syntax
namespace NewWay::Inner { // Defines NewWay, and Inner inside NewWay
    void greet_new() {
        std::cout << "Hello from NewWay::Inner!" << std::endl;
    }
    // You can continue nesting further
    namespace EvenDeeper {
        const int value = 20;
        void print_value() {
             // Accessing parent namespace elements
            std::cout << "NewWay::Inner::EvenDeeper::value = " << value << std::endl;
        }
    }
} // Closes Inner, then NewWay

// You can also reopen and add to nested namespaces using the new syntax
namespace NewWay::Inner {
    void farewell_new() {
        std::cout << "Goodbye from NewWay::Inner!" << std::endl;
    }
}

// Example of extending a very deeply nested namespace
namespace A::B::C::D {
    int deep_value = 100;
}
// Later, possibly in another part of the code or header
namespace A::B::C::D {
    void show_deep_value(){
        std::cout << "A::B::C::D::deep_value = " << deep_value << std::endl;
    }
}


int main() {
    std::cout << "--- Nested Namespaces ---" << std::endl;

    // Accessing elements from old-style nested namespaces
    OldWay::Inner::greet_old();
    std::cout << "OldWay::Inner::EvenDeeper::value = " << OldWay::Inner::EvenDeeper::value << std::endl;

    // Accessing elements from C++17-style nested namespaces
    NewWay::Inner::greet_new();
    NewWay::Inner::farewell_new(); // From the reopened namespace
    NewWay::Inner::EvenDeeper::print_value();
    std::cout << "NewWay::Inner::EvenDeeper::value (accessed directly) = " << NewWay::Inner::EvenDeeper::value << std::endl;

    // Using declaration for convenience
    std::cout << "\n--- Using declarations ---" << std::endl; // MANUALLY CORRECTED NEWLINE
    {
        using namespace NewWay::Inner; // Brings greet_new, farewell_new, and EvenDeeper into scope
        greet_new();
        EvenDeeper::print_value(); // EvenDeeper is now directly accessible
        // std::cout << value << std::endl; // Error: EvenDeeper::value is not directly in NewWay::Inner
        std::cout << EvenDeeper::value << std::endl; // Correct
    }
    
    // Accessing deeply nested namespace
    A::B::C::D::show_deep_value();
    A::B::C::D::deep_value = 200; // Modifying
    A::B::C::D::show_deep_value();


    // Alias for nested namespace
    namespace NIED = NewWay::Inner::EvenDeeper;
    std::cout << "Value via alias NIED::value = " << NIED::value << std::endl;


    return 0;
}

/*
Explanation:
Nested namespace definitions are a C++17 feature that provides a more concise
syntax for defining namespaces within other namespaces.

Pre-C++17 Syntax:
To define a namespace `Inner` within a namespace `Outer`, you would write:
```cpp
namespace Outer {
    namespace Inner {
        // ... declarations ...
    }
}
```
This requires multiple `namespace` keywords and curly brace pairs, which can
lead to increased indentation and verbosity, especially for deeply nested namespaces.

C++17 Syntax:
C++17 allows you to define nested namespaces using the `::` separator in a
single namespace declaration:
```cpp
namespace Outer::Inner { // Defines Outer, and then Inner within Outer
    // ... declarations ...
} // This closes Inner, and then Outer
```

Key Features and Benefits:
1.  Conciseness: Reduces the amount of boilerplate code needed for nesting.
    `namespace A::B::C { ... }` is much shorter than the equivalent three
    separate namespace blocks.
2.  Readability: Can improve readability by reducing indentation levels,
    especially for moderately nested namespaces.
3.  Consistency: Aligns the definition syntax more closely with how nested
    namespaces are typically referred to in code (e.g., `A::B::C::my_function()`).

Reopening Namespaces:
The new syntax can also be used to reopen and add to already defined nested
namespaces, just like the old syntax.
```cpp
namespace MyLib::Core { void func1(); }
// ... later ...
namespace MyLib::Core { void func2(); } // Adds func2 to MyLib::Core
```

Example:
```cpp
namespace Company::Product::Module {
    class MyClass { /* ... */ };
    void do_something();
}

// Usage:
Company::Product::Module::MyClass obj;
Company::Product::Module::do_something();
```

This feature is purely a syntactic sugar and does not change the fundamental
behavior or rules of namespaces. It simply offers a more convenient way to
define them.

How to compile:
g++ -std=c++17 nested_namespaces.cpp -o nested_namespaces_example
./nested_namespaces_example
*/
