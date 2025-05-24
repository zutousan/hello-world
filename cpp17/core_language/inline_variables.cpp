// inline_variables.cpp
#include <iostream>
#include <string> // Required for MyClassWithInlineStatic

// ---- Scenario 1: Inline static data members ----
// Header file context (e.g., MyClass.h)
struct MyClassWithInlineStatic {
    static const int static_const_int = 10; // Pre-C++17: Allowed for const integral types if defined in class

    // C++17: 'inline' allows definition of static members directly in the class declaration.
    // This includes non-const static members and static members of types that
    // previously required out-of-class definitions.
    // This variable will have only one definition across all translation units.
    inline static double static_double = 3.14;
    inline static std::string static_string = "Hello from inline static";

    // Also works for static const members that are not integral types or need out-of-class definition pre-C++17
    inline static const std::string static_const_string = "Const Inline String";

    void print_statics() const {
        std::cout << "MyClassWithInlineStatic:" << std::endl;
        std::cout << "  static_const_int: " << static_const_int << " (addr: " << &static_const_int << ")" << std::endl;
        std::cout << "  static_double: " << static_double << " (addr: " << &static_double << ")" << std::endl;
        std::cout << "  static_string: "" << static_string << "" (addr: " << &static_string << ")" << std::endl;
        std::cout << "  static_const_string: "" << static_const_string << "" (addr: " << &static_const_string << ")" << std::endl;
    }
};


// ---- Scenario 2: Inline global variables (typically in a header) ----
// Header file context (e.g., shared_globals.h)
namespace SharedGlobals {
    inline int global_counter = 0; // Defined inline, one definition across TUs
    inline const std::string global_app_name = "My Inline App";
}

// Helper functions that might be in different .cpp files, all including the "header"
// (Simulated here in one file for brevity)
void increment_global_counter(const std::string& source_file) {
    SharedGlobals::global_counter++;
    std::cout << source_file << ": Incremented global_counter to " << SharedGlobals::global_counter
              << " (addr: " << &SharedGlobals::global_counter << ")" << std::endl;
}

void print_app_name(const std::string& source_file) {
    std::cout << source_file << ": App Name is "" << SharedGlobals::global_app_name << """
              << " (addr: " << &SharedGlobals::global_app_name << ")" << std::endl;
}


int main() {
    std::cout << "--- Inline Static Data Members ---" << std::endl;
    MyClassWithInlineStatic obj1;
    MyClassWithInlineStatic obj2;

    obj1.print_statics();
    
    std::cout << "\nModifying static_double via obj1..." << std::endl;
    obj1.static_double = 2.718; 
    MyClassWithInlineStatic::static_string = "Modified Hello"; // Can also access via class name

    // obj2 will see the modified values because static members are shared
    std::cout << "\nobj2 after obj1 modified static members:" << std::endl;
    obj2.print_statics();

    std::cout << "\nAccessing static members directly via class name:" << std::endl;
    std::cout << "MyClassWithInlineStatic::static_const_int = " << MyClassWithInlineStatic::static_const_int << std::endl;
    std::cout << "MyClassWithInlineStatic::static_double = " << MyClassWithInlineStatic::static_double << std::endl;


    std::cout << "\n--- Inline Global Variables (simulated from different TUs) ---" << std::endl;
    print_app_name("main.cpp (initial)");
    increment_global_counter("main.cpp");

    // Simulate calls from other "files"
    // In a real multi-file project, these would be in separate .cpp files
    // that include the header with the inline global variables.
    std::cout << "\nSimulating calls from other translation units:" << std::endl;
    print_app_name("file1.cpp");
    increment_global_counter("file1.cpp");
    
    print_app_name("file2.cpp");
    increment_global_counter("file2.cpp");

    std::cout << "\nFinal global_counter value: " << SharedGlobals::global_counter << std::endl;
    std::cout << "Note: All addresses for the same inline variable should be identical across 'files'." << std::endl;

    return 0;
}

/*
Explanation:
Inline Variables, introduced in C++17, simplify the definition of static class
members and global variables that might be shared across multiple translation
units (typically in header files).

The `inline` keyword, when applied to a variable:
-   Allows the variable to be defined in a header file.
-   Ensures that even if this header is included in multiple source files (translation units),
    there will be only one unique definition of the variable in the entire program.
    This resolves the "One Definition Rule" (ODR) violations that would occur if
    a regular global variable or static class member were defined in a header.

Key Use Cases:

1.  Inline Static Data Members:
    -   Before C++17, non-const static data members of a class had to be declared
        in the class (in the header) and then defined in exactly one source file (.cpp).
        `// MyClass.h`
        `struct MyClass { static int count; };`
        `// MyClass.cpp`
        `int MyClass::count = 0;`
    -   `static const` integral members could be initialized in-class, but other
        `static const` members (like `std::string`) often needed out-of-class definitions.
    -   With C++17 `inline`, you can define these static members directly within the
        class definition in the header file:
        ```cpp
        // MyClass.h (C++17)
        struct MyClass {
            inline static int count = 0;
            inline static std::string name = "default";
            inline static const double PI = 3.14159;
        };
        ```
    -   This simplifies code organization, especially for header-only libraries.

2.  Inline Global Variables:
    -   Similar to static data members, if you wanted a global variable accessible
        across multiple translation units, you'd typically declare it as `extern`
        in a header and define it in one source file.
        `// globals.h`
        `extern int global_value;`
        `// globals.cpp`
        `int global_value = 100;`
    -   With C++17 `inline`, you can define the global variable in the header:
        ```cpp
        // globals.h (C++17)
        namespace AppConfig {
            inline int version_major = 1;
            inline const std::string app_title = "My Application";
        }
        ```
    -   This is useful for global constants or configuration variables that need
        to be shared.

How it Works (Linker Magic):
The `inline` keyword for variables instructs the linker to consolidate all inline
definitions of a particular variable from different translation units into a
single definition in the final program. If the definitions are different (which
they shouldn't be for the same variable), the behavior is undefined.

Benefits:
-   Simplified Code: Reduces the need for separate .cpp file definitions for
    static members or global variables intended for header use.
-   Header-Only Libraries: Makes it easier to write header-only libraries that
    use static members or global-like constants/variables.
-   ODR Compliance: Provides a standard way to define variables in headers without
    violating the One Definition Rule.

`inline` vs. `static` at Namespace/Global Scope:
-   `static` at namespace/global scope gives a variable internal linkage, meaning
    each translation unit gets its own separate instance of the variable.
-   `inline` at namespace/global scope gives a variable external linkage but
    allows it to be defined in multiple translation units, with the linker ensuring
    only one instance exists program-wide.

How to compile:
g++ -std=c++17 inline_variables.cpp -o inline_variables_example
./inline_variables_example
*/
