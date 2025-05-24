// cpp20/core_language/modules_basic_usage.cpp
#include <iostream>
// No traditional #include for the module content itself.
// Instead, we import the module.
import math_operations; // Imports the 'math_operations' module

int main() {
    std::cout << "--- C++20 Modules (Basic Usage) ---" << std::endl;

    int sum = add(5, 3); // Uses add() from the math_operations module
    std::cout << "add(5, 3) = " << sum << std::endl;

    int difference = subtract_public(10, 4); // Uses subtract_public()
    std::cout << "subtract_public(10, 4) = " << difference << std::endl;

    // int diff_internal = subtract_internal(7, 2); // COMPILE ERROR: subtract_internal is not exported

    std::cout << "Value of PI from module: " << PI << std::endl;

    Point p = {10, 20};
    std::cout << "Point from module: {" << p.x << ", " << p.y << "}" << std::endl;

    std::cout << "\nModules help in organizing code, reducing compilation times (potentially)," << std::endl;
    std::cout << "and avoiding issues related to header files (like macro leakage)." << std::endl;
    std::cout << "Compilation of modules requires specific compiler flags." << std::endl;

    return 0;
}

/*
Explanation:
C++20 Modules are a major feature designed to improve over the traditional
header file (`#include`) system. They offer several benefits:

1.  Improved Compilation Times: Modules are typically compiled once, and their
    compiled form (often an "interface unit") is imported by other parts of the
    code. This can be much faster than repeatedly processing header files.
2.  Encapsulation: Modules provide better encapsulation. By default, declarations
    within a module are not exported. Only entities explicitly marked with `export`
    are visible to importing code. This helps prevent macro leakage, name clashes,
    and unintended dependencies.
3.  Reduced Transitive Includes: Unlike headers, importing a module does not
    implicitly "include" everything that module itself might import or include
    for its implementation details. This leads to more robust and isolated components.
4.  Clearer Dependencies: The `import` keyword makes dependencies explicit and clear.

Key Keywords:
-   `export module <module-name>;`: Declares a module interface unit.
-   `module <module-name>;`: Can declare a module implementation unit (not shown here).
-   `export <declaration>`: Makes the declaration visible to importers.
-   `import <module-name>;`: Imports a compiled module.

Structure:
-   Module Interface Unit: Typically has a `.cppm` or `.ixx` extension (compiler-dependent).
    It contains the `export module` declaration and the declarations that are
    exported.
-   Module Implementation Unit(s) (Optional): Can contain definitions for entities
    declared in the interface unit, or internal details. These are not directly
    imported by users.
-   Module Consumer: A regular .cpp file that uses `import <module-name>;`.

Conceptual Compilation (highly compiler-dependent):

Modules introduce a new compilation model. You typically compile module interface
units first, and then compile files that import them.

Example steps (conceptual, actual commands vary greatly):

1. Compile the module interface unit (e.g., math_module.cppm):
   g++ -std=c++20 -fmodules-ts --precompile math_module.cppm -o math_operations.gcm
   (For GCC, this creates a precompiled module interface file .gcm)
   OR
   clang++ -std=c++20 --precompile math_module.cppm -o math_operations.pcm
   (For Clang, this creates a .pcm file)
   OR
   cl /std:c++20 /experimental:module /interface /TP math_module.cppm (and /module:output for .ifc)

2. Compile the implementation file that imports the module (e.g., modules_basic_usage.cpp):
   g++ -std=c++20 -fmodules-ts math_operations.gcm modules_basic_usage.cpp -o modules_example
   (GCC needs to be told where to find the .gcm)
   OR
   clang++ -std=c++20 -fprebuilt-module-path=. math_modules.pcm modules_basic_usage.cpp -o modules_example
   (Clang needs to find the .pcm)
   OR
   cl /std:c++20 /experimental:module /module:reference math_operations.ifc modules_basic_usage.cpp /link /out:modules_example.exe

Key Points:
- The exact commands and file extensions (`.gcm`, `.pcm`, `.ifc`) are compiler-specific.
- Order matters: module interfaces must be processed before units that import them.
- Build systems (CMake, etc.) are gradually adding robust support for modules,
  which will abstract away these command-line details.
- This example is conceptual. Full, portable module support and tooling are still evolving.
*/
