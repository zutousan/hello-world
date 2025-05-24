// inline_variables.cpp
#include <iostream>
#include <string> // Required for MyClassWithInlineStatic

// ---- Scenario 1: Inline static data members ----
struct MyClassWithInlineStatic {
    static const int static_const_int = 10; 

    inline static double static_double = 3.14;
    inline static std::string static_string = "Hello from inline static";
    inline static const std::string static_const_string = "Const Inline String";

    void print_statics() const {
        std::cout << "MyClassWithInlineStatic:" << std::endl;
        std::cout << "  static_const_int: " << static_const_int << " (addr: " << &static_const_int << ")" << std::endl;
        std::cout << "  static_double: " << static_double << " (addr: " << &static_double << ")" << std::endl;
        // Corrected quotes and newlines in the following std::cout statements
        std::cout << "  static_string: \"" << static_string << "\" (addr: " << &static_string << ")" << std::endl; // MANUALLY CORRECTED
        std::cout << "  static_const_string: \"" << static_const_string << "\" (addr: " << &static_const_string << ")" << std::endl; // MANUALLY CORRECTED
    }
};

// ---- Scenario 2: Inline global variables (typically in a header) ----
namespace SharedGlobals {
    inline int global_counter = 0; 
    inline const std::string global_app_name = "My Inline App";
}

void increment_global_counter(const std::string& source_file) {
    SharedGlobals::global_counter++;
    // Corrected quotes and newlines
    std::cout << source_file << ": Incremented global_counter to " << SharedGlobals::global_counter
              << " (addr: " << &SharedGlobals::global_counter << ")" << std::endl;
}

void print_app_name(const std::string& source_file) {
    // Corrected quotes and newlines
    std::cout << source_file << ": App Name is \"" << SharedGlobals::global_app_name << "\"" // MANUALLY CORRECTED
              << " (addr: " << &SharedGlobals::global_app_name << ")" << std::endl;
}

int main() {
    std::cout << "--- Inline Static Data Members ---" << std::endl;
    MyClassWithInlineStatic obj1;
    MyClassWithInlineStatic obj2;

    obj1.print_statics();
    
    std::cout << "\nModifying static_double via obj1..." << std::endl; // MANUALLY CORRECTED
    obj1.static_double = 2.718; 
    MyClassWithInlineStatic::static_string = "Modified Hello";

    std::cout << "\nobj2 after obj1 modified static members:" << std::endl; // MANUALLY CORRECTED
    obj2.print_statics();

    std::cout << "\nAccessing static members directly via class name:" << std::endl; // MANUALLY CORRECTED
    std::cout << "MyClassWithInlineStatic::static_const_int = " << MyClassWithInlineStatic::static_const_int << std::endl;
    std::cout << "MyClassWithInlineStatic::static_double = " << MyClassWithInlineStatic::static_double << std::endl;

    std::cout << "\n--- Inline Global Variables (simulated from different TUs) ---" << std::endl; // MANUALLY CORRECTED
    print_app_name("main.cpp (initial)");
    increment_global_counter("main.cpp");

    std::cout << "\nSimulating calls from other translation units:" << std::endl; // MANUALLY CORRECTED
    print_app_name("file1.cpp");
    increment_global_counter("file1.cpp");
    
    print_app_name("file2.cpp");
    increment_global_counter("file2.cpp");

    std::cout << "\nFinal global_counter value: " << SharedGlobals::global_counter << std::endl; // MANUALLY CORRECTED
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
-   Ensures that even if this header is included in multiple source files,
    there will be only one unique definition of the variable in the entire program.
    This resolves "One Definition Rule" (ODR) violations.

Key Use Cases:
1.  Inline Static Data Members: Define static members (const or non-const)
    directly within the class definition in the header file.
2.  Inline Global Variables: Define global variables in a header file, ensuring
    a single instance across the program.

Benefits:
-   Simplified Code: Reduces need for separate .cpp definitions.
-   Header-Only Libraries: Easier to write.
-   ODR Compliance.

How to compile:
g++ -std=c++17 inline_variables.cpp -o inline_variables_example
./inline_variables_example
*/
